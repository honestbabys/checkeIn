#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "include/SocketClient.h"
#include "include/CheckedMacroes.h"
#include <iostream>
using namespace std;

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

string CSocketClient::sendServer(const std::string& strContent)
{
	int iRet = write(_iClientSock ,strContent.c_str(), strlen(strContent.c_str()));
	if (iRet < 0)
	{

	}
	CHECKED_DEBUG("����ָ��������ɹ�!");
	char buffer[MAXSIZE];
	bzero(buffer,MAXSIZE);
	iRet = read(_iClientSock, buffer, MAXSIZE);
	if(iRet < 0)
	{

	}
	CHECKED_DEBUG("���յ��ķ�������ӦΪ��%s", buffer);
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
