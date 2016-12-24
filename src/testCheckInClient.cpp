#include "include/SocketClient.h"
#include <iostream>

//测试命令端

int main()
{
	std::string strCmd("0"); //清空数据
	CSocketClient client;
	client.connectServer("127.0.0.1",6000);
	client.sendServer(strCmd.c_str());

	strCmd = "1,2,8:00";
	client.sendServer(strCmd.c_str()); //上班打卡 

	strCmd = "1,2,8:30";
	client.sendServer(strCmd.c_str()); //又打一次

	strCmd = "1,2,17:30"; //下班打卡
	client.sendServer(strCmd.c_str());

	strCmd = "2,1"; //查询考勤信息
	client.sendServer(strCmd.c_str());

	return 0;
}
