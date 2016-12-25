/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   SocketClient.h
* FUNCTION:   the class CSocketClient declaration file
* AUTHOR:     sam
* DATE:       2016-12-24
* VERSION:    1.0
* NOTES:      header file
****************************************************************/
#ifndef _SOCKETCLIENT_H
#define _SOCKETCLIENT_H
#include <string>

class CSocketClient
{
public:
	CSocketClient();
	~CSocketClient();
	std::string sendServer(const std::string& strContent); //sendServer必须返回响应结果给客户端
	void connectServer(const char *ip, int port); //socket句柄应该保存起来

private:
	void closeHandle();
	
private:
	int _iClientSock;
	const static int MAXSIZE = 1024;
};
#endif
