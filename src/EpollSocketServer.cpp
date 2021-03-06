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
#include <fcntl.h>
#include "include/EpollSocketServer.h"
#include "include/CheckedResult.h"
#include "include/CheckedMacroes.h"
#include "include/Exception.h"

CEpollSocketServer::CEpollSocketServer(int port):_iListenFd(-1), _iEpollFd(-1),pResult(NULL)
{
	//完成对应参数的初始化
	//应该在构造函数中打开socket并且监控，开始处理。
	CHECKED_DEBUG ("CEpollSocketServer ctor now");
	pResult = new CCheckedResult();
	try
	{
		_iListenFd = bindSocket(port);
		CHECKED_DEBUG("绑定完成返回的句柄为：%d ", _iListenFd);
		listenSocket();
		doEpoll(_iListenFd);
	}
	catch(CException e)
	{
		CHECKED_DEBUG ("发生异常，异常原因：%s",e.what());
	}
	catch(...)
	{

	}
}

CEpollSocketServer::~CEpollSocketServer()
{
	//listenFd、 epollfd
	//析构函数中关闭socket套接字
	CHECKED_DEBUG ("CEpollSocketServer dtor now");
	closeHandle();
	if(pResult != NULL)
	{
		delete pResult;
		pResult = NULL;
	}
}

int CEpollSocketServer::bindSocket(int port)
{
	int listenfd;
	struct sockaddr_in servAddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1)
	{
		CHECKED_DEBUG("socket error! ");
		throw CException(1001,"socket error! ");
	}
	bzero(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(listenfd,(struct sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		CHECKED_DEBUG("bind error! ");
		throw CException(1002,"socket bind error! ");
	}
	CHECKED_DEBUG("绑定完成, 端口号:%d ", port);
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
			throw CException(1003,"socket listen error! ");
		}
	}
	CHECKED_DEBUG("listenSocketd()调用完成");
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
		if (ret < 0) //epoll_wait失败
		{
			throw CException(1004,"epoll_wait error! ");
		}
		handleEvents(_iEpollFd, events, ret, listenfd, buf);
	}

	close(_iEpollFd);
}
/************************************************************************/
/* 使用epoll 来实现服务端同时接受多客户端长连接数据时，的大体步骤如下：

（1）使用epoll_create创建一个 epoll 的句柄，下例中我们命名为epollfd。
（2）使用epoll_ctl把服务端监听的描述符添加到epollfd指定的 epoll 内核事件表中，监听服务器端监听的描述符是否可读。
（3）使用epoll_wait阻塞等待注册的服务端监听的描述符可读事件的发生。
（4）当有新的客户端连接上服务端时，服务端监听的描述符可读，则epoll_wait返回，然后通过accept获取客户端描述符。
（5）使用epoll_ctl把客户端描述符添加到epollfd指定的 epoll 内核事件表中，监听服务器端监听的描述符是否可读。
（6）当客户端描述符有数据可读时，则触发epoll_wait返回，然后执行读取。                                                                     */
/************************************************************************/
void CEpollSocketServer::handleEvents(int epollfd, struct epoll_event *events,int num,int listenfd, char *buf)
{
	/*
	int fd = -1;
	for (int i=0; i<num; i++)
	{
		fd = events[i].data.fd;
		if ((fd==listenfd) && (events[i].events & EPOLLIN))//有连连接建立
		{
			CHECKED_DEBUG("有新的连接建立");
			hanldeAccpet(epollfd, listenfd); 
		}
		else if (events[i].events & EPOLLIN) //接收到数据，读socket
		{

			doRead(epollfd, fd, buf);

			CCheckedResult chkResult;
			string strResponse = chkResult.getCmd(string(buf));

			string strResponse = "OK";
			write(fd, strResponse.c_str(), strlen(strResponse.c_str()));

		} 
	}
	*/
	for (int i = 0; i < num; ++i)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			struct sockaddr_in client_address;
			socklen_t length = sizeof(client_address);
			int connfd = accept(listenfd, (struct sockaddr *) &client_address,
					&length);
			CHECKED_DEBUG("接收一个请求来自于: %s:%d\n", inet_ntoa(client_address.sin_addr),
					ntohs(client_address.sin_port));

			addEvent(epollfd, connfd, EPOLLIN|EPOLLET);
		}
		else if (events[i].events & EPOLLIN)
		{
			memset(buf, '\0', MAXSIZE);
			int nread = read(sockfd,buf,MAXSIZE);
			if (nread == -1)
			{
				CHECKED_DEBUG("。。。。。。。。。。。。。。。。。。。");
				close(sockfd);
				deleteEvent(epollfd, sockfd, EPOLLIN|EPOLLET);
				break;
			}
			else if (nread == 0)
			{
				CHECKED_DEBUG("断开一个连接\n");
				close(sockfd);
			}
			else
			{
				CHECKED_DEBUG("get %d bytes of content: %s\n", nread, buf);
				string strResp = pResult->getCmd(string(buf));
				CHECKED_DEBUG("返回服务器的响应消息为:%s ", strResp.c_str());
				write(sockfd, strResp.c_str(), strlen(strResp.c_str()));
			}
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
		CHECKED_DEBUG("连接建立异常");
		throw CException(1005,"socket accept error! ");
	}
	else
	{
		CHECKED_DEBUG("接收到的新的客户端连接: %s:%d\n",inet_ntoa(cliAddr.sin_addr),cliAddr.sin_port);
		addEvent(epollfd, clifd, EPOLLIN);
	}
}

void CEpollSocketServer::doRead(int epollfd, int fd, char* buf)
{
	bzero(buf, MAXSIZE);
	int ret = recv(fd, buf, MAXSIZE-1, 0);
	if (ret < 0)
	{
		close(fd);
		deleteEvent(epollfd, fd, EPOLLIN);
	}
	else if (ret == 0)
	{
		printf("断开一个连接\n");
		close(fd);
		deleteEvent(epollfd, fd, EPOLLIN);
	}
	else
	{
		printf("获取到 %d 字节，内容为: %s\n", ret, buf);
		modifyEvent(epollfd,fd,EPOLLOUT);
	}
}

void CEpollSocketServer::addEvent(int epollfd,int fd,int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
	int oldOption = fcntl(fd, F_GETFL);
	int newOption = oldOption | O_NONBLOCK;
	fcntl(fd, F_SETFL, newOption);
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
