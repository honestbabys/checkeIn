#include "include/SocketClient.h"
#include <iostream>

//���������

int main()
{
	std::string strCmd("0"); //�������
	CSocketClient client;
	client.connectServer("127.0.0.1",6000);
	client.sendServer(strCmd.c_str());

	strCmd = "1,2,8:00";
	client.sendServer(strCmd.c_str()); //�ϰ�� 

	strCmd = "1,2,8:30";
	client.sendServer(strCmd.c_str()); //�ִ�һ��

	strCmd = "1,2,17:30"; //�°��
	client.sendServer(strCmd.c_str());

	strCmd = "2,1"; //��ѯ������Ϣ
	client.sendServer(strCmd.c_str());

	return 0;
}
