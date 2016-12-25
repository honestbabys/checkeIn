#include "include/CheckedResult.h"
#include "include/CheckedMacroes.h"
#include <string>

int main()
{
	CCheckedResult chkRes;
	CHECKED_DEBUG("开始进行打卡测试........................................");

	/*
	std::string strCmd = "1,3,8:00";
	string strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("工号为3的打卡返回的结果为：%s", strResponse.c_str());
	strCmd = "1,2,7:10";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("工号2的打卡返回的结果为：%s", strResponse.c_str());
	strCmd = "1,2,21:10";
	CHECKED_DEBUG("工号2的打卡返回的结果为：%s", strResponse.c_str());
	strResponse = chkRes.getCmd(strCmd);
	strCmd = "1,2,22:00";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("工号2的打卡返回的结果为：%s", strResponse.c_str());
	*/

	std::string strCmd = "1,1,10:00";
	std::string strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("工号为1打卡返回的结果为：%s", strResponse.c_str());
	strCmd = "1,1,17:31";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("工号为1打卡返回的结果为：%s", strResponse.c_str());

	CHECKED_DEBUG("打卡测试结束........................................");
	CHECKED_DEBUG("开始进行考勤结果展示................................");
	strCmd = "2";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("考勤结果为：%s", strResponse.c_str());
	system("pause");

	return 0;
}
