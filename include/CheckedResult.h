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
	NORMAL = 0, //正常
	ABSENT = 1, //矿工
	LATE = 2,  //迟到
	EARLY = 3, //早退
	LATE_EARLY = 4, //迟到又早退
	ABNORMAL = 5 //数据异常
};

typedef struct _CheckedInfo{
	std::string checkInTime;  // 开始时间,先用string代替
	std::string checkOutTime;  //结束时间
	std::string strWorkTime;  //工作时间
	std::string strCheckedId; //考勤人员工号
	std::string strCheckedName; //考勤人员姓名
	_CheckedType Type; //考勤类型
}CheckedInfo;
*/


class CCheckedResult
{
public:
	CCheckedResult();
	~CCheckedResult();
	std::string getCmd(const std::string& strCmd); //外部调用，响应命令

private:
	bool init(); //加载文件中信息，后期考虑RAII实现
	void clearAllCheckedInfo();
	std::string getCheckedInfo(const std::string& strCheckedNo = "");
	std::string check(const std::string& strCheckId, std::string& strCheckTime);  //打卡
	time_t convertToTime(const std::string strTime); //时间操作，废弃，增加时间操作类
	void checkedVerfify(); //根据考勤数据计算考勤工时
	void getWorkTime(const std::string& strCheckedInTime, const std::string& strCheckedOutTime, std::string& strWorkTime, _CheckedType& type); //根据打卡时间计算工时
	bool verifyCheckedId(const std::string& strCheckedId); //校验工号合法
	void clearEmplyoeeInfo();
private:

	std::map<std::string,CCheckedInfo*> _mapCheckedInfo; //保存员工考勤信息 _mapInstCtrlMemory

	std::vector<std::map<std::string,std::string> > _vecEmployeeInfo; //保存员工信息
};


#endif
