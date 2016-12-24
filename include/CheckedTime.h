/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   CheckedTime.h
* FUNCTION:   the class CCheckedTime declaration file
* AUTHOR:     sam
* DATE:       2016-12-22
* VERSION:    1.0
* NOTES:      header file
****************************************************************/

#include <string>
#include <map>
#include <vector>

#ifndef _CHECKEDTIME_H
#define _CHECKEDTIME_H

using namespace std;

class CCheckedTime
{
public:

	CCheckedTime();
	CCheckedTime( const string& stringTime );
	virtual ~CCheckedTime();

	friend CCheckedTime& operator -( CCheckedTime&, CCheckedTime& );
	friend CCheckedTime& operator -( CCheckedTime& cSuperTimeSource, const string stringTimeMinus);

	friend bool operator >( CCheckedTime&, CCheckedTime& );
	friend bool operator >( CCheckedTime& cSuperTimeSource, const string stringTimeMinus);
	friend bool operator >( const string stringTimeMinus, CCheckedTime& cSuperTimeSource);

	friend bool operator <( CCheckedTime&, CCheckedTime& );
	friend bool operator <( CCheckedTime& cSuperTimeSource, const string stringTimeMinus);
	friend bool operator <( const string stringTimeMinus, CCheckedTime& cSuperTimeSource);

	friend bool operator ==( CCheckedTime&, CCheckedTime& );
	friend bool operator ==( CCheckedTime& cSuperTimeSource, const string stringTimeMinus);
	friend bool operator ==( const string stringTimeMinus, CCheckedTime& cSuperTimeSource);

	friend bool operator >=( CCheckedTime&, CCheckedTime& );
	friend bool operator >=( CCheckedTime& cSuperTimeSource, const string stringTimeMinus);
	friend bool operator >=( const string stringTimeMinus, CCheckedTime& cSuperTimeSource);

	friend bool operator <=( CCheckedTime&, CCheckedTime& );
	friend bool operator <=( CCheckedTime& cSuperTimeSource, const string stringTimeMinus);
	friend bool operator <=( const string stringTimeMinus, CCheckedTime& cSuperTimeSource);

	CCheckedTime& operator =( CCheckedTime& );

	CCheckedTime&  format( const string& stringTime );

	string toHHMMString();
	void clearMinute();
	void clearHour();

	void setMinute( int Minute );
	void setHour( int Hour );  

	int getMinute();
	int getHour();  

	static int toInt(const std::string& s);
	static std::vector<string> getTokenList(const string& val, const string& token);
	//减法有bug，另外缺少一个格式化成符合要求的xxHxx的函数，明天改

private:
	int getTimeId();
	
	void clean();

private:
	int _iHour;
	int _iMinute;
};

#endif
