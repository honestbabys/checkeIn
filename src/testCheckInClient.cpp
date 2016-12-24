#include "include/SocketClient.h"
#include "include/CheckedMacroes.h"
#include <iostream>

//测试命令端

int main()
{
	CSocketClient client;
	client.connectServer("127.0.0.1",6000);
	std::string strCmd = "1,1,10:00";
	std::string strResponse = client.sendServer(strCmd.c_str());
	CHECKED_DEBUG("工号为1打卡返回的结果为：%s", strResponse.c_str());


	strCmd = "1,1,17:31";
	strResponse = client.sendServer(strCmd.c_str()); //上班打卡
	CHECKED_DEBUG("打卡测试结束........................................");
	CHECKED_DEBUG("开始进行考勤结果展示................................");

	strCmd = "2";
	strResponse = client.sendServer(strCmd.c_str()); //上班打卡
	CHECKED_DEBUG("考勤结果为：%s", strResponse.c_str());


	return 0;
}
