/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   CheckedTime.cpp
* FUNCTION:   the class CCheckedTime Implementation file
* AUTHOR:     sam
* DATE:       2016-12-22
* VERSION:    1.0
* NOTES:      Implementation file
****************************************************************/

#include <iostream>
#include <cmath>
#include <string.h>
#include<stdlib.h>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include <vector>
#include "include/CheckedTime.h"
#include "include/CheckedMacroes.h"
using namespace std;

CCheckedTime::CCheckedTime():_iMinute(-1),_iHour(-1)
{
	CHECKED_DEBUG("CCheckedTime ctor");
}

CCheckedTime::CCheckedTime( const string& stringTime )
{
	CHECKED_DEBUG("CCheckedTime ctor(stringTime)");
	format( stringTime );
}


CCheckedTime::~CCheckedTime()
{
	CHECKED_DEBUG("CCheckedTime dtor");
	clean();
}

CCheckedTime& operator -( CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget )
{
	if (cSuperTimeSource._iMinute < cSuperTimeTarget._iMinute)
	{
		cSuperTimeSource._iHour -= 1;
		cSuperTimeSource._iMinute += 60;
	}
	cSuperTimeSource._iHour -= cSuperTimeTarget._iHour ;
	cSuperTimeSource._iMinute -= cSuperTimeTarget._iMinute ;
	return cSuperTimeSource;
}

CCheckedTime& operator -( CCheckedTime& cSuperTimeSource, const string stringTime)
{
	CCheckedTime cSuperTimeTarget(stringTime);
	cSuperTimeSource - cSuperTimeTarget;
	return cSuperTimeSource;
}

bool operator >( CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget )
{
	CHECKED_DEBUG("进入重载>运算符判断中");
	if ( cSuperTimeSource.getTimeId() > cSuperTimeTarget.getTimeId() )
	{
		return true;
	}
	return false;
}

bool operator >( CCheckedTime& cSuperTimeSource, const string stringTime)
{
	CHECKED_DEBUG("operator >( CCheckedTime& cSuperTimeSource, const string stringTime)");
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTime );
	return ( cSuperTimeSource > cSuperTimeTarget);
}

bool operator >( const string stringTime , CCheckedTime& cSuperTimeSource )
{
	CHECKED_DEBUG("const string stringTime , CCheckedTime& cSuperTimeSource");
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTime );
	return ( cSuperTimeTarget > cSuperTimeSource );
}

bool operator <( CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget )
{
	return ( cSuperTimeTarget > cSuperTimeSource );
}


bool operator <( CCheckedTime& cSuperTimeSource, const string stringTime)
{
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTime );
	return ( cSuperTimeSource < cSuperTimeTarget);
}

bool operator <( const string stringTime, CCheckedTime& cSuperTimeSource)
{
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTime );
	return ( cSuperTimeTarget < cSuperTimeSource );
}

bool operator ==( CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget)
{
	CHECKED_DEBUG("operator ==( CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget");
	if ( ( !( cSuperTimeSource > cSuperTimeTarget ) ) && ( !( cSuperTimeSource < cSuperTimeTarget ) ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool operator ==( CCheckedTime& cSuperTimeSource, const string stringTime)
{
	CHECKED_DEBUG("operator ==( CCheckedTime& cSuperTimeSource, const string stringTime)");
	if ( !( cSuperTimeSource > stringTime ) && !( cSuperTimeSource < stringTime ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool operator ==(  const string stringTime ,CCheckedTime& cSuperTimeSource)
{
	CHECKED_DEBUG("CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget");
	if (!( cSuperTimeSource > stringTime ) && !( cSuperTimeSource < stringTime ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool operator >=( CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget)
{
	return !( cSuperTimeSource < cSuperTimeTarget);
}

bool operator >=( CCheckedTime& cSuperTimeSource, const string stringTimeMinus)
{
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTimeMinus );
	return !( cSuperTimeSource < cSuperTimeTarget);
}

bool operator >=( const string stringTimeMinus, CCheckedTime& cSuperTimeSource)
{
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTimeMinus );
	return !( cSuperTimeTarget < cSuperTimeSource );
}

bool operator <=( CCheckedTime& cSuperTimeSource, CCheckedTime& cSuperTimeTarget)
{
	return !( cSuperTimeSource > cSuperTimeTarget);
}

bool operator <=( CCheckedTime& cSuperTimeSource, const string stringTimeMinus)
{
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTimeMinus );
	return !( cSuperTimeSource > cSuperTimeTarget);

}

bool operator <=( const string stringTimeMinus, CCheckedTime& cSuperTimeSource)
{
	CCheckedTime cSuperTimeTarget;
	cSuperTimeTarget.format( stringTimeMinus );
	return !( cSuperTimeTarget > cSuperTimeSource );
}
CCheckedTime& CCheckedTime::operator =( CCheckedTime& cSuperTimeSource)
{
	if ( this == &cSuperTimeSource )
	{
		return *this;
	}
	_iHour = cSuperTimeSource._iHour;
	_iMinute = cSuperTimeSource._iMinute;
	return *this;
}


CCheckedTime&  CCheckedTime::format(const string& stringTime)
{
	CHECKED_DEBUG("CCheckedTime::format(const string& stringTime)");
	vector<string> vectorTime = getTokenList(stringTime, ":");
	_iHour = toInt(vectorTime[0]);
	_iMinute = toInt(vectorTime[1]);
	return *this;
}


string CCheckedTime::toHHMMString()
{
	char cRet[30];
	sprintf( cRet,"%01dH%02dM", _iHour, _iMinute);
	return string( cRet,strlen( cRet ) );
}

void CCheckedTime::clean( )
{
	_iHour = 0;
	_iMinute = 0;
}

void CCheckedTime::clearMinute()
{
	_iMinute = 0;
}
void CCheckedTime::clearHour()
{
	_iHour = 0;
}

void CCheckedTime::setMinute( int Minute )
{
	_iMinute = Minute;
}
void CCheckedTime::setHour( int Hour )
{
	_iHour = Hour;
}


int CCheckedTime::getMinute()
{
	return _iMinute;
}
int CCheckedTime::getHour()
{
	return _iHour;
}

int CCheckedTime::toInt(const string& s)
{
	int retn = atoi(s.c_str());
	return retn;
}

vector<string> CCheckedTime::getTokenList(const string& val, const string& token)
{
	vector<string> slist;
	string tmp = val;
	int pos = 0;
	while(pos != string::npos)
	{
		pos = tmp.find(token);
		if (pos != string::npos)
		{
			if (pos!=0)
			{
				slist.push_back(tmp.substr(0,pos));
			}
			tmp = tmp.substr(pos + token.length(), tmp.length()-pos-token.length());
		}
	}
	if (tmp.length()>0)
	{
		slist.push_back(tmp);
	}
	return slist;
}

int CCheckedTime::getTimeId()
{
	int iRet = 0;
	iRet = _iHour * 10000 + _iMinute * 100 ;
	return iRet;
}
