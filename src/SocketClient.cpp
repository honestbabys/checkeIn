#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "include/SocketClient.h"

CSocketClient::CSocketClient():_iClientSock(-1)
{

}

CSocketClient::~CSocketClient()
{
	closeHandle();
}

void CSocketClient::connectServer(const char *ip, const int port)
{
	//����һ��socket��ַ�ṹclient_addr,����ͻ���internet��ַ, �˿�
	struct sockaddr_in cliAddr;
	bzero(&cliAddr,sizeof(cliAddr)); 
	cliAddr.sin_family = AF_INET;    //internetЭ����
	cliAddr.sin_addr.s_addr = htons(INADDR_ANY);//INADDR_ANY��ʾ�Զ���ȡ������ַ
	cliAddr.sin_port = htons(0);    //0��ʾ��ϵͳ�Զ�����һ�����ж˿�
	_iClientSock = socket(AF_INET,SOCK_STREAM,0);
	if (_iClientSock < 0) //ʧ�� �쳣
	{

	}
	if(bind(_iClientSock,(struct sockaddr*)&cliAddr,sizeof(cliAddr)))
	{

	}
	struct sockaddr_in servAddr;
	bzero(&servAddr,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_aton(ip,&servAddr.sin_addr);
	servAddr.sin_port = htons(port);
	if(connect(_iClientSock,(struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
	{

	}
}

int CSocketClient::sendServer(const std::string& strContent)
{
	send(_iClientSock,strContent.c_str(),strlen(strContent.c_str()),0);
	char buffer[MAXSIZE];
	bzero(buffer,MAXSIZE);

	int length = 0;
	while( length = recv(_iClientSock,buffer,MAXSIZE,0))
	{
		if(length < 0)
		{
			break;
		}
		bzero(buffer,MAXSIZE);    
	}
	return 0;
}

void CSocketClient::closeHandle()
{
	if (_iClientSock != -1)
	{
		close(_iClientSock);
		_iClientSock = -1;
	}
}
