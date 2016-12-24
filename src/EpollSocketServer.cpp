/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   EpollSocketServer.cpp
* FUNCTION:   the class CEpollSocketServer Implementation file
* AUTHOR:     sam
* DATE:       2016-12-22
* VERSION:    1.0
* NOTES:      Implementation file
****************************************************************/
#include <string.h>
#include "EpollSocketServer.h"
#include "CheckedResult.h"
#include "CheckedMacroes.h"

CEpollSocketServer::CEpollSocketServer(int port):_iListenFd(-1), _iEpollFd(-1)
{
	//��ɶ�Ӧ�����ĳ�ʼ��
	//Ӧ���ڹ��캯���д�socket���Ҽ�أ���ʼ����
	CHECKED_DEBUG ("CEpollSocketServer ctor now");
	_iListenFd = bindSocket(port);
	CHECKED_DEBUG("����ɷ��صľ��Ϊ��%d ", _iListenFd);

	listenSocket();
	doEpoll(_iListenFd);
}

CEpollSocketServer::~CEpollSocketServer()
{
	//listenFd�� epollfd
	//���������йر�socket�׽���
	CHECKED_DEBUG ("CEpollSocketServer dtor now");
	closeHandle();
}

int CEpollSocketServer::bindSocket(int port)
{
	int listenfd;
	struct sockaddr_in servAddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1)
	{
		CHECKED_DEBUG("socket error! ");
		return -1;
	}
	bzero(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(listenfd,(struct sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		CHECKED_DEBUG("bind error! ");
		return -1;
	}
	CHECKED_DEBUG("�����, �˿ں�:%d ", port);
	return listenfd;
}

int CEpollSocketServer::listenSocket()
{
	int iRes = -1;
	if (_iListenFd != -1 )
	{
		iRes = listen(_iListenFd, LISTENQ);
		if (iRes == -1)
		{
			CHECKED_DEBUG("listen error! ");
			return -1;
		}
	}
	CHECKED_DEBUG("listenSocketd()�������");
	return iRes;
}

void CEpollSocketServer::doEpoll(int listenfd)
{
	struct epoll_event events[EPOLLEVENTS];
	int ret = -1;
	char buf[MAXSIZE] = {0};
	_iEpollFd = epoll_create(FDSIZE);
	addEvent(_iEpollFd, listenfd, EPOLLIN|EPOLLET);

	for (; ;)
	{
		ret = epoll_wait(_iEpollFd, events, EPOLLEVENTS, -1);
		if (ret < 0) //epoll_waitʧ��
		{

		}

		handleEvents(_iEpollFd, events, ret, listenfd, buf);
	}
	close(_iEpollFd);
}
/************************************************************************/
/* ʹ��epoll ��ʵ�ַ����ͬʱ���ܶ�ͻ��˳���������ʱ���Ĵ��岽�����£�

��1��ʹ��epoll_create����һ�� epoll �ľ������������������Ϊepollfd��
��2��ʹ��epoll_ctl�ѷ���˼�������������ӵ�epollfdָ���� epoll �ں��¼����У������������˼������������Ƿ�ɶ���
��3��ʹ��epoll_wait�����ȴ�ע��ķ���˼������������ɶ��¼��ķ�����
��4�������µĿͻ��������Ϸ����ʱ������˼������������ɶ�����epoll_wait���أ�Ȼ��ͨ��accept��ȡ�ͻ�����������
��5��ʹ��epoll_ctl�ѿͻ�����������ӵ�epollfdָ���� epoll �ں��¼����У������������˼������������Ƿ�ɶ���
��6�����ͻ��������������ݿɶ�ʱ���򴥷�epoll_wait���أ�Ȼ��ִ�ж�ȡ��                                                                     */
/************************************************************************/

void CEpollSocketServer::handleEvents(int epollfd, struct epoll_event *events,int num,int listenfd, char *buf)
{
	CHECKED_DEBUG("��ʼ��Ӧ�¼�...");
	int fd = -1;
	for (int i=0; i<num; i++)
	{
		fd = events[i].data.fd;
		if ((fd==listenfd) && (events[i].events & EPOLLIN))//�������ӽ���
		{
			CHECKED_DEBUG("���µ����ӽ���");
			hanldeAccpet(epollfd, listenfd); 
		}
		else if (events[i].events & EPOLLIN) //���յ����ݣ���socket
		{
			CHECKED_DEBUG("�����ݿɶ���");
			doRead(epollfd, fd, buf); 
			CCheckedResult chkResult;
			string strResponse = chkResult.getCmd(string(buf));
			write( fd, strResponse.c_str(), strlen(strResponse.c_str()));
		} 
	}
}

void CEpollSocketServer::hanldeAccpet(int epollfd, int listenfd)
{
	struct sockaddr_in cliAddr;
	socklen_t cliAddrLen;

	int clifd = accept(listenfd,(struct sockaddr*)&cliAddr,&cliAddrLen);
	if (clifd == -1)
	{
		CHECKED_DEBUG("���ӽ����쳣");
		return;
	}
	else
	{
		CHECKED_DEBUG("���յ����µĿͻ�������: %s:%d\n",inet_ntoa(cliAddr.sin_addr),cliAddr.sin_port);
		addEvent(epollfd, clifd, EPOLLIN);
	}
}

void CEpollSocketServer::doRead(int epollfd, int fd, char* buf)
{
	bzero(buf, MAXSIZE);

	//�и�bug���ȹ��ܲ�����ɺ��޸ġ�
	//���������ݽ϶�ʱ������һ�ν��ղ���������
	int ret = recv(fd, buf, MAXSIZE-1, 0);
	if (ret < 0)
	{
		/* �Է����� IO �����������������ʾ�����Ѿ�ȫ����ȡ��ϡ��˺� epoll �����ٴδ��� sockfd �ϵ� EPOLLIN �¼�����������һ�ζ����� */
		/*
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
		{
			break;
		}
		*/
		close(fd);
		deleteEvent(epollfd,fd,EPOLLIN);
	}
	else if (ret == 0)
	{
		printf("�Ͽ�һ������\n");
		close(fd);
		deleteEvent(epollfd,fd,EPOLLIN);
	}
	else
	{
		printf("��ȡ�� %d �ֽڣ�����Ϊ: %s\n", ret, buf);
		modifyEvent(epollfd,fd,EPOLLOUT);
	}
}

void CEpollSocketServer::addEvent(int epollfd,int fd,int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}
void CEpollSocketServer::deleteEvent(int epollfd,int fd,int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}

void CEpollSocketServer::modifyEvent(int epollfd,int fd,int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}

void CEpollSocketServer::closeHandle()
{
	if (_iListenFd != -1)
	{
		close(_iListenFd);
		_iListenFd = -1;
	}
	if (_iEpollFd != -1)
	{
		close(_iListenFd);
		_iListenFd = -1;
	}
}
