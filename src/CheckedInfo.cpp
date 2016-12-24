/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   CheckedInfo.cpp
* FUNCTION:   the class CCheckedInfo Implementation file
* AUTHOR:     sam
* DATE:       2016-12-24
* VERSION:    1.0
* NOTES:      Implementation file
****************************************************************/

#include "include/CheckedInfo.h"
#include "include/CheckedMacroes.h"

CCheckedInfo::CCheckedInfo():_strCheckInTime(""),_strCheckOutTime(""),_strWorkTime(""),_strCheckedId(""),_strCheckedName(""),_Type(NORMAL)
{
	CHECKED_DEBUG("CCheckedInfo ctor now()");
}

CCheckedInfo::~CCheckedInfo()
{
	CHECKED_DEBUG("CCheckedInfo dtor now")
}

