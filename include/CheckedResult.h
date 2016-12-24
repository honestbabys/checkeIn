/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   CheckedResult.h
* FUNCTION:   the class CCheckedResult declaration file
* AUTHOR:     sam
* DATE:       2016-12-24
* VERSION:    1.0
* NOTES:      header file
****************************************************************/

#include <map>
#include <vector>
#include "CheckedTime.h"
#include "CheckedInfo.h"

#ifndef _CHECKEDRESULT_H
#define _CHECKEDRESULT_H

/*
enum _CheckedType
{
	NORMAL = 0, //����
	ABSENT = 1, //��
	LATE = 2,  //�ٵ�
	EARLY = 3, //����
	LATE_EARLY = 4, //�ٵ�������
	ABNORMAL = 5 //�����쳣
};

typedef struct _CheckedInfo{
	std::string checkInTime;  // ��ʼʱ��,����string����
	std::string checkOutTime;  //����ʱ��
	std::string strWorkTime;  //����ʱ��
	std::string strCheckedId; //������Ա����
	std::string strCheckedName; //������Ա����
	_CheckedType Type; //��������
}CheckedInfo;
*/


class CCheckedResult
{
public:
	CCheckedResult();
	~CCheckedResult();
	std::string getCmd(const std::string& strCmd); //�ⲿ���ã���Ӧ����

private:
	bool init(); //�����ļ�����Ϣ�����ڿ���RAIIʵ��
	void clearAllCheckedInfo();
	std::string getCheckedInfo(const std::string& strCheckedNo = "");
	std::string check(const std::string& strCheckId, std::string& strCheckTime);  //��
	time_t convertToTime(const std::string strTime); //ʱ�����������������ʱ�������
	void checkedVerfify(); //���ݿ������ݼ��㿼�ڹ�ʱ
	void getWorkTime(const std::string& strCheckedInTime, const std::string& strCheckedOutTime, std::string& strWorkTime, _CheckedType& type); //���ݴ�ʱ����㹤ʱ
	bool verifyCheckedId(const std::string& strCheckedId); //У�鹤�źϷ�
	void clearEmplyoeeInfo();
private:

	std::map<std::string,CCheckedInfo*> _mapCheckedInfo; //����Ա��������Ϣ _mapInstCtrlMemory

	std::vector<std::map<std::string,std::string> > _vecEmployeeInfo; //����Ա����Ϣ
};


#endif
