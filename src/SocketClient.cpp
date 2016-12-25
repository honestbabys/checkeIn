/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   SocketClient.cpp
* FUNCTION:   the class CSocketClient Implementation file
* AUTHOR:     sam
* DATE:       2016-12-24
* VERSION:    1.0
* NOTES:      Implementation file
****************************************************************/
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "include/SocketClient.h"
#include "include/CheckedMacroes.h"
#include "include/Exception.h"
#include <iostream>
using namespace std;

CSocketClient::CSocketClient():_iClientSock(-1)
{
	CHECKED_DEBUG ("CSocketClient ctor now");
}

CSocketClient::~CSocketClient()
{
	CHECKED_DEBUG ("CSocketClient dtor now");
	closeHandle();
}

void CSocketClient::connectServer(const char *ip, const int port)
{
	//设置一个socket地址结构client_addr,代表客户机internet地址, 端口
	struct sockaddr_in cliAddr;
	bzero(&cliAddr,sizeof(cliAddr)); 
	cliAddr.sin_family = AF_INET;    //internet协议族
	cliAddr.sin_addr.s_addr = htons(INADDR_ANY);//INADDR_ANY表示自动获取本机地址
	cliAddr.sin_port = htons(0);    //0表示让系统自动分配一个空闲端口
	_iClientSock = socket(AF_INET,SOCK_STREAM,0);
	if (_iClientSock < 0) //失败 异常
	{
		CHECKED_DEBUG("socket error! ");
		throw CException(1001,"socket error! ");
	}
	if(bind(_iClientSock,(struct sockaddr*)&cliAddr,sizeof(cliAddr)))
	{
		CHECKED_DEBUG("socket bind error! ");
		throw CException(1002,"socket error! ");
	}
	struct sockaddr_in servAddr;
	bzero(&servAddr,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_aton(ip,&servAddr.sin_addr);
	servAddr.sin_port = htons(port);
	if(connect(_iClientSock,(struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
	{
		CHECKED_DEBUG("socket connect error! ");
		throw CException(1003,"socket connect error! ");
	}
}

string CSocketClient::sendServer(const std::string& strContent)
{
	int iRet = write(_iClientSock ,strContent.c_str(), strlen(strContent.c_str()));
	if (iRet < 0)
	{
		CHECKED_DEBUG("socket write error! ");
		throw CException(1004,"socket write error! ");
	}
	CHECKED_DEBUG("发送指令到服务器成功!");
	char buffer[MAXSIZE];
	bzero(buffer,MAXSIZE);
	iRet = read(_iClientSock, buffer, MAXSIZE);
	if(iRet < 0)
	{
		CHECKED_DEBUG("socket read error! ");
		throw CException(1005,"socket read error! ");
	}
	CHECKED_DEBUG("接收到的服务器响应为：%s", buffer);
	return string(buffer);
}

void CSocketClient::closeHandle()
{
	if (_iClientSock != -1)
	{
		close(_iClientSock);
		_iClientSock = -1;
	}
}
