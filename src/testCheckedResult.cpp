#include "include/CheckedResult.h"
#include "include/CheckedMacroes.h"
#include <string>

int main()
{
	CCheckedResult chkRes;
	CHECKED_DEBUG("��ʼ���д򿨲���........................................");

	/*
	std::string strCmd = "1,3,8:00";
	string strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("����Ϊ3�Ĵ򿨷��صĽ��Ϊ��%s", strResponse.c_str());
	strCmd = "1,2,7:10";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("����2�Ĵ򿨷��صĽ��Ϊ��%s", strResponse.c_str());
	strCmd = "1,2,21:10";
	CHECKED_DEBUG("����2�Ĵ򿨷��صĽ��Ϊ��%s", strResponse.c_str());
	strResponse = chkRes.getCmd(strCmd);
	strCmd = "1,2,22:00";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("����2�Ĵ򿨷��صĽ��Ϊ��%s", strResponse.c_str());
	*/

	std::string strCmd = "1,1,10:00";
	std::string strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("����Ϊ1�򿨷��صĽ��Ϊ��%s", strResponse.c_str());
	strCmd = "1,1,17:31";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("����Ϊ1�򿨷��صĽ��Ϊ��%s", strResponse.c_str());

	CHECKED_DEBUG("�򿨲��Խ���........................................");
	CHECKED_DEBUG("��ʼ���п��ڽ��չʾ................................");
	strCmd = "2";
	strResponse = chkRes.getCmd(strCmd);
	CHECKED_DEBUG("���ڽ��Ϊ��%s", strResponse.c_str());
	system("pause");

	return 0;
}
