#include "include/SocketClient.h"
#include "include/CheckedMacroes.h"
#include <iostream>

//���������

int main()
{
	CSocketClient client;
	client.connectServer("127.0.0.1",6000);
	std::string strCmd = "1,1,10:00";
	std::string strResponse = client.sendServer(strCmd.c_str());
	CHECKED_DEBUG("����Ϊ1�򿨷��صĽ��Ϊ��%s", strResponse.c_str());


	strCmd = "1,1,17:31";
	strResponse = client.sendServer(strCmd.c_str()); //�ϰ��
	CHECKED_DEBUG("�򿨲��Խ���........................................");
	CHECKED_DEBUG("��ʼ���п��ڽ��չʾ................................");

	strCmd = "2";
	strResponse = client.sendServer(strCmd.c_str()); //�ϰ��
	CHECKED_DEBUG("���ڽ��Ϊ��%s", strResponse.c_str());


	return 0;
}
