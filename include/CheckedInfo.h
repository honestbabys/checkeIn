/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   CheckedTime.h
* FUNCTION:   the class CCheckedInfo declaration file
* AUTHOR:     sam
* DATE:       2016-12-23
* VERSION:    1.0
* NOTES:      header file
****************************************************************/
#ifndef _CHECKEDINO_H
#define _CHECKEDINO_H
#include <iostream>

enum _CheckedType
{
	NORMAL = 0, //����
	ABSENT = 1, //��
	LATE = 2,  //�ٵ�
	EARLY = 3, //����
	LATE_EARLY = 4, //�ٵ�������
	ABNORMAL = 5 //�����쳣
};

class CCheckedInfo
{
public:
	CCheckedInfo();
	~CCheckedInfo();
	std::string _strCheckInTime;  // ��ʼʱ��,����string����
	std::string _strCheckOutTime;  //����ʱ��
	std::string _strWorkTime;  //����ʱ��
	std::string _strCheckedId; //������Ա����
	std::string _strCheckedName; //������Ա����
	_CheckedType _Type; //��������
};
#endif
