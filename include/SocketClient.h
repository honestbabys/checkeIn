#ifndef _SOCKETCLIENT_H
#define _SOCKETCLIENT_H
#include <string>

class CSocketClient
{
public:
	CSocketClient();
	~CSocketClient();
	int sendServer(const std::string& strContent); //sendServer必须返回响应结果给客户端
	void connectServer(const char *ip, int port); //socket句柄应该保存起来
private:
	
	void closeHandle();
private:
	int _iClientSock;
	const static int MAXSIZE = 1024;
};
#endif
