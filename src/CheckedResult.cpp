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
	//���ܳ��ֵ�ָ���ʽ����
	// 0 ����б�
	// 1, 2, 8:00 1�� 2Ա��ID 8:00��ʱ��
	// 2, (2) ����Ա��������г�Ա����Ӧ�Ĵ򿨼�¼ ��������Ϊȫ���򿨼�¼
	CHECKED_DEBUG("���յ������ʽΪ��%s", strCmd.c_str());
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
 	CHECKED_DEBUG("ָ�����:%d,Ա�����:%s,��ʱ��:%s",iCmdNo, strCheckedNo.c_str(),strCheckedTime.c_str());
	string response = ""; 

	switch(iCmdNo)
	{
	case 0:
		CHECKED_DEBUG("���������Ϣ");
		clearAllCheckedInfo();
		response="OK";
		break;
	case 1:
		CHECKED_DEBUG("ˢ��");
		response = check(strCheckedNo, strCheckedTime);
		break;
	case 2:
		CHECKED_DEBUG("������Ϣ");
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
		CHECKED_DEBUG("���ļ�����");
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
			CHECKED_DEBUG("���ҵ���Ӧ�Ĵ򿨼�¼��Ա��idΪ��%s", strCheckId.c_str());
			if (iterCheckedInfo->second != NULL)
			{
				if (iterCheckedInfo->second->_strCheckInTime == "") //���ϰ�ʱ�䣬����Ϊ�ϰ�ʱ��
				{
					CHECKED_DEBUG("��ǰ���ϰ�ʱ�䣬��Ϊ�ϰ�ʱ�䣬�ϰ�ʱ��Ϊ:%s", strCheckTime.c_str());
					iterCheckedInfo->second->_strCheckInTime  = strCheckTime;
				}
				else //���ϰ�ʱ��
				{   CCheckedTime chkTime(strCheckTime);
					if(chkTime < iterCheckedInfo->second->_strCheckInTime)
					{
						CHECKED_DEBUG("�Ѵ��ڴ�ʱ�䣬���ָ����ʱ�䣬��ʱ��Ϊ:%s", strCheckTime.c_str());
						iterCheckedInfo->second->_strCheckInTime = strCheckTime;
					}
					if (iterCheckedInfo->second->_strCheckOutTime.empty()) //���°�ʱ��
					{
						//��ʱ������ϰ�ʱ�䣬����Ϊ�°�ʱ��
						if (chkTime > iterCheckedInfo->second->_strCheckInTime)
						{
							CHECKED_DEBUG("��ǰ���°�ʱ�䣬��Ϊ�°�ʱ�䣬�°�ʱ��Ϊ:%s", strCheckTime.c_str());
							iterCheckedInfo->second->_strCheckOutTime = strCheckTime;
						}
					}
					else
					{
						//��ʱ������ϰ�ʱ�䣬����Ϊ�°�ʱ��
						if (chkTime > iterCheckedInfo->second->_strCheckOutTime)
						{
							CHECKED_DEBUG("��ǰ�Ѵ����°�ʱ�䣬���ָ����°�ʱ�䣬�°�ʱ��Ϊ:%s", strCheckTime.c_str());
							iterCheckedInfo->second->_strCheckOutTime = strCheckTime;
						}
					}
				}
			}
		}
		else //�޴���Ϣ
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
		else //���δ��ȱ����Ա
		{
			std::vector<std::map<string,string> >::iterator iterEmployee;
			for (iterEmployee=_vecEmployeeInfo.begin(); iterEmployee!=_vecEmployeeInfo.end(); iterEmployee++)
			{
				std::map<string,string>::iterator iterMapEmployee;
				if ((iterMapEmployee =iterEmployee->find(strCheckedNo))!= iterEmployee->end()) //���Ҵ򿨵Ĺ����Ƿ���Ч
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
	CHECKED_DEBUG("��ʼ���й�ʱ����");
	std::map<std::string,CCheckedInfo*>::iterator iterChkInfo;
	for (iterChkInfo=_mapCheckedInfo.begin(); iterChkInfo!=_mapCheckedInfo.end(); iterChkInfo++)
	{
		std::vector<std::map<string,string> >::iterator iterEmployee;
		for (iterEmployee= _vecEmployeeInfo.begin();iterEmployee!=_vecEmployeeInfo.end();iterEmployee++ )
		{
			std::map<string,string>::iterator iterMapEmployee;
			if ((iterMapEmployee =iterEmployee->find(iterChkInfo->first))!= iterEmployee->end()) //���Ҵ򿨵Ĺ����Ƿ���Ч
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
		if ((iterMapEmployee =iterEmployee->find(strCheckedId))!= iterEmployee->end()) //���Ҵ򿨵Ĺ����Ƿ���Ч
		{
			bCheckedSts = true;
		}
	}
	return bCheckedSts;

}

//��ʱ����
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
		if (chkInTime > "17:30") //�ϰ�ʱ������17:30
		{
			strWorkTime = "0";
			type = ABSENT;
		}
		if (chkOutTime < "8:00") //�°�ʱ�������ϰ�ʱ��
		{
			strWorkTime = "0";
			type = ABSENT;
		}
		if (chkInTime > "8:00" && chkOutTime < "17:30") //�ϰ�ʱ������8:00 �°�ʱ������17:30 �ٵ������� 
		{
			type = LATE_EARLY;
		}
		else if (chkInTime <= "8:00" && chkOutTime < "17:30") // ����
		{
			type = EARLY;
		}
		else if ( chkInTime > "8:00" && chkOutTime >= "17:30") //�ٵ�
		{
			type = LATE;
		}
		else
		{
			type = NORMAL; //�����ϰ�
		}
		strWorkTime = (chkOutTime-chkInTime).toHHMMString();
	}
}


