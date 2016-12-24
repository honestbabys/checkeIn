/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   CheckedResult.cpp
* FUNCTION:   the class CCheckedResult Implementation file
* AUTHOR:     sam
* DATE:       2016-12-23
* VERSION:    1.0
* NOTES:      Implementation file
****************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include "include/CheckedResult.h"
#include "include/CheckedMacroes.h"
using namespace std;

CCheckedResult::CCheckedResult()
{
	CHECKED_DEBUG("CCheckedResult ctor now");
	init();
}

CCheckedResult::~CCheckedResult()
{
	CHECKED_DEBUG("CCheckedResult dtor now");
	clearEmplyoeeInfo();
	clearAllCheckedInfo();
}

string CCheckedResult::getCmd(const string& strCmd) 
{
	//可能出现的指令格式如下
	// 0 清空列表
	// 1, 2, 8:00 1打卡 2员工ID 8:00打卡时间
	// 2, (2) 存在员工编号则列出员工对应的打卡记录 不存在则为全部打卡记录
	CHECKED_DEBUG("接收的命令格式为：%s", strCmd.c_str());
	vector<string> verCmd = CCheckedTime::getTokenList(strCmd, ",");
	vector<string>::iterator iterCmd;
	int iCmdNo = -1;
	string strCheckedNo ="";
	string strCheckedTime = "";
	int iCount = 0;
	for (iterCmd=verCmd.begin(); iterCmd!=verCmd.end(); iterCmd++)
	{
		if (iCount==0)
		{
			iCmdNo = CCheckedTime::toInt(*iterCmd);
		}
		else if (iCount==1)
		{
			strCheckedNo = *iterCmd;
		}
		else if (iCount==2)
		{
			strCheckedTime =  *iterCmd;
		}
		iCount++;
	}
 	CHECKED_DEBUG("指令编码:%d,员工编号:%s,打卡时间:%s",iCmdNo, strCheckedNo.c_str(),strCheckedTime.c_str());
	string response = ""; 

	switch(iCmdNo)
	{
	case 0:
		CHECKED_DEBUG("清除考勤信息");
		clearAllCheckedInfo();
		response="OK";
		break;
	case 1:
		CHECKED_DEBUG("刷卡");
		response = check(strCheckedNo, strCheckedTime);
		break;
	case 2:
		CHECKED_DEBUG("考勤信息");
		response = getCheckedInfo(strCheckedNo);
		break;
	default:
		response = "ERR:COMMAND INVALID";
		break;
	}
	return  response;
}

bool CCheckedResult::init()
{
	CHECKED_DEBUG("CCheckedResult::init() now");
	char buffer[256] = {0};
	std::ifstream myfile ("employee.data");
	if(!myfile)
	{
		CHECKED_DEBUG("打开文件错误！");
	}
	while (!myfile.eof() )
	{
		myfile.getline(buffer,256);
		std::string strContent = string(buffer);
		vector<string> vecEmployee =  CCheckedTime::getTokenList(strContent, ",");
		map<string, string>mapEmployee;
		mapEmployee.insert(make_pair(vecEmployee[0], vecEmployee[1]));
		_vecEmployeeInfo.push_back(mapEmployee);
	}
	return true;
}

string CCheckedResult::check(const std::string& strCheckId, const std::string& strCheckTime)
{
	CHECKED_DEBUG("CCheckedResult::check() now");
	string strRet = "OK";
	vector<string> vectorTime = CCheckedTime::getTokenList(strCheckTime, ":");
	if (vectorTime.size() != 2)
	{
		return "ERR:TIME INVALID";
	}
	if(verifyCheckedId(strCheckId))
	{
		std::map<std::string,CCheckedInfo*>::iterator iterCheckedInfo ;
		if ((iterCheckedInfo = _mapCheckedInfo.find(strCheckId)) != _mapCheckedInfo.end())
		{
			CHECKED_DEBUG("查找到对应的打卡记录，员工id为：%s", strCheckId.c_str());
			if (iterCheckedInfo->second != NULL)
			{
				if (iterCheckedInfo->second->_strCheckInTime == "") //无上班时间，更新为上班时间
				{
					CHECKED_DEBUG("当前无上班时间，打卡为上班时间，上班时间为:%s", strCheckTime.c_str());
					iterCheckedInfo->second->_strCheckInTime  = strCheckTime;
				}
				else //有上班时间
				{   CCheckedTime chkTime(strCheckTime);
					if(chkTime < iterCheckedInfo->second->_strCheckInTime)
					{
						CHECKED_DEBUG("已存在打卡时间，发现更早打卡时间，打卡时间为:%s", strCheckTime.c_str());
						iterCheckedInfo->second->_strCheckInTime = strCheckTime;
					}
					if (iterCheckedInfo->second->_strCheckOutTime.empty()) //无下班时间
					{
						//打卡时间大于上班时间，更新为下班时间
						if (chkTime > iterCheckedInfo->second->_strCheckInTime)
						{
							CHECKED_DEBUG("当前无下班时间，打卡为下班时间，下班时间为:%s", strCheckTime.c_str());
							iterCheckedInfo->second->_strCheckOutTime = strCheckTime;
						}
					}
					else
					{
						//打卡时间大于上班时间，更新为下班时间
						if (chkTime > iterCheckedInfo->second->_strCheckOutTime)
						{
							CHECKED_DEBUG("当前已存在下班时间，发现更晚下班时间，下班时间为:%s", strCheckTime.c_str());
							iterCheckedInfo->second->_strCheckOutTime = strCheckTime;
						}
					}
				}
			}
		}
		else //无打卡信息
		{
			CCheckedInfo *pInfo = new CCheckedInfo();
			pInfo->_strCheckInTime = strCheckTime;
			_mapCheckedInfo.insert(map<string,CCheckedInfo*>::value_type(strCheckId, pInfo));
		}
	}
	else
	{
		strRet = "ERR:ID INVALID";
	}
	return strRet;
}


string CCheckedResult::getCheckedInfo(const string& strCheckedNo)
{
	checkedVerfify();
	std::map<std::string,CCheckedInfo*>::iterator iterCheckedInfo;
	ostringstream os;
	os.clear();
	if (!strCheckedNo.empty())
	{
		if ((iterCheckedInfo=_mapCheckedInfo.find(strCheckedNo)) != _mapCheckedInfo.end())
		{
			os<<"\n\n";
			os<<"ID= "<<iterCheckedInfo->first<<"\n";
			os<<"NAME= "<<iterCheckedInfo->second->_strCheckedName<<"\n";
			os<<"TYPE= "<<iterCheckedInfo->second->_Type<<"\n";
			os<<"CHECK IN= "<<iterCheckedInfo->second->_strCheckInTime<<"\n";
			os<<"CHECK OUT= "<<iterCheckedInfo->second->_strCheckOutTime<<"\n";
			os<<"WORK TIME= "<<iterCheckedInfo->second->_strWorkTime;
			os<<"\n";
		}
		else //针对未打卡缺勤人员
		{
			std::vector<std::map<string,string> >::iterator iterEmployee;
			for (iterEmployee=_vecEmployeeInfo.begin(); iterEmployee!=_vecEmployeeInfo.end(); iterEmployee++)
			{
				std::map<string,string>::iterator iterMapEmployee;
				if ((iterMapEmployee =iterEmployee->find(strCheckedNo))!= iterEmployee->end()) //查找打卡的工号是否有效
				{
					os<<"\n\n";
					os<<"ID= "<<iterMapEmployee->first<<"\n";
					os<<"NAME= "<<iterMapEmployee->second<<"\n";
					os<<"TYPE= "<<"ABSENT"<<"\n";
					os<<"CHECK IN= "<<""<<"\n";
					os<<"CHECK OUT= "<<""<<"\n";
					os<<"WORK TIME= "<<0;
					os<<"\n";
				}

			}

		}
	}
	else
	{
		for (iterCheckedInfo= _mapCheckedInfo.begin();iterCheckedInfo!=_mapCheckedInfo.end();iterCheckedInfo++)
		{
			os<<"\n\n";
			os<<"ID= "<<iterCheckedInfo->first<<"\n";
			os<<"NAME= "<<iterCheckedInfo->second->_strCheckedName<<"\n";
			os<<"TYPE= "<<iterCheckedInfo->second->_Type<<"\n";
			os<<"CHECK IN= "<<iterCheckedInfo->second->_strCheckInTime<<"\n";
			os<<"CHECK OUT= "<<iterCheckedInfo->second->_strCheckOutTime<<"\n";
			os<<"WORK TIME= "<<iterCheckedInfo->second->_strWorkTime;
		}
	}
	string strResponse = os.str();
	return strResponse;
}

void CCheckedResult::clearAllCheckedInfo()
{
	std::map<std::string,CCheckedInfo*>::iterator iterCheckedInfo;
	for (iterCheckedInfo= _mapCheckedInfo.begin();iterCheckedInfo!=_mapCheckedInfo.end();iterCheckedInfo++)
	{
		if ((*iterCheckedInfo).second != NULL)
		{
			delete (*iterCheckedInfo).second;
			(*iterCheckedInfo).second = NULL;
		}
	}
	_mapCheckedInfo.clear();
}

void CCheckedResult::clearEmplyoeeInfo()
{
	_vecEmployeeInfo.clear();
}

void CCheckedResult::checkedVerfify()
{
	CHECKED_DEBUG("开始进行工时计算");
	std::map<std::string,CCheckedInfo*>::iterator iterChkInfo;
	for (iterChkInfo=_mapCheckedInfo.begin(); iterChkInfo!=_mapCheckedInfo.end(); iterChkInfo++)
	{
		std::vector<std::map<string,string> >::iterator iterEmployee;
		for (iterEmployee= _vecEmployeeInfo.begin();iterEmployee!=_vecEmployeeInfo.end();iterEmployee++ )
		{
			std::map<string,string>::iterator iterMapEmployee;
			if ((iterMapEmployee =iterEmployee->find(iterChkInfo->first))!= iterEmployee->end()) //查找打卡的工号是否有效
			{
				iterChkInfo->second->_strCheckedName = iterMapEmployee->second; 
			}
		}
		getWorkTime(iterChkInfo->second->_strCheckInTime, iterChkInfo->second->_strCheckOutTime, iterChkInfo->second->_strWorkTime,iterChkInfo->second->_Type);
	}
}

bool CCheckedResult::verifyCheckedId(const std::string& strCheckedId)
{
	CHECKED_DEBUG("CCheckedResult::verifyCheckedId() now");
	bool bCheckedSts = false;
	std::vector<std::map<string,string> >::iterator iterEmployee;
	for (iterEmployee= _vecEmployeeInfo.begin();iterEmployee!=_vecEmployeeInfo.end();iterEmployee++ )
	{
		std::map<string,string>::iterator iterMapEmployee;
		if ((iterMapEmployee =iterEmployee->find(strCheckedId))!= iterEmployee->end()) //查找打卡的工号是否有效
		{
			bCheckedSts = true;
		}
	}
	return bCheckedSts;

}

//工时计算
void CCheckedResult::getWorkTime(const std::string& strCheckedInTime, const std::string& strCheckedOutTime, string& strWorkTime, _CheckedType& type)
{

	CCheckedTime chkInTime(strCheckedInTime);
	CCheckedTime chkOutTime(strCheckedOutTime);
	if (strCheckedInTime.empty() && strCheckedOutTime.empty())
	{
		strWorkTime = "0";
		type = ABSENT;
	}
	if (strCheckedInTime.empty() || strCheckedOutTime.empty())
	{
		strWorkTime = "0";
		type = ABNORMAL;
	}
	else
	{
		if (chkInTime > "17:30") //上班时间晚于17:30
		{
			strWorkTime = "0";
			type = ABSENT;
		}
		if (chkOutTime < "8:00") //下班时间早于上班时间
		{
			strWorkTime = "0";
			type = ABSENT;
		}
		if (chkInTime > "8:00" && chkOutTime < "17:30") //上班时间晚于8:00 下班时间早于17:30 迟到又早退 
		{
			type = LATE_EARLY;
		}
		else if (chkInTime <= "8:00" && chkOutTime < "17:30") // 早退
		{
			type = EARLY;
		}
		else if ( chkInTime > "8:00" && chkOutTime >= "17:30") //迟到
		{
			type = LATE;
		}
		else
		{
			type = NORMAL; //正常上班
		}
		strWorkTime = (chkOutTime-chkInTime).toHHMMString();
	}
}


