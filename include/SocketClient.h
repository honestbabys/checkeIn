#ifndef _SOCKETCLIENT_H
#define _SOCKETCLIENT_H
#include <string>

class CSocketClient
{
public:
	CSocketClient();
	~CSocketClient();
	int sendServer(const std::string& strContent); //sendServer���뷵����Ӧ������ͻ���
	void connectServer(const char *ip, int port); //socket���Ӧ�ñ�������
private:
	
	void closeHandle();
private:
	int _iClientSock;
	const static int MAXSIZE = 1024;
};
#endif
