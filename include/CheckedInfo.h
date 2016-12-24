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
	NORMAL = 0, //正常
	ABSENT = 1, //矿工
	LATE = 2,  //迟到
	EARLY = 3, //早退
	LATE_EARLY = 4, //迟到又早退
	ABNORMAL = 5 //数据异常
};

class CCheckedInfo
{
public:
	CCheckedInfo();
	~CCheckedInfo();
	std::string _strCheckInTime;  // 开始时间,先用string代替
	std::string _strCheckOutTime;  //结束时间
	std::string _strWorkTime;  //工作时间
	std::string _strCheckedId; //考勤人员工号
	std::string _strCheckedName; //考勤人员姓名
	_CheckedType _Type; //考勤类型
};
#endif
