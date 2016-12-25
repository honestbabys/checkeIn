/****************************************************************
* (C) Copyright CATTsoft All rights reserved.
* FILENAME:   Exception.hpp
* FUNCTION:   The Class CException declaration file
* AUTHOR:     sam
* DATE:       2016-12-25
* VERSION:    1.0
* NOTES:      header file
****************************************************************/
#ifndef __EXCEPTION__H
#define __EXCEPTION__H

#include <string.h>
class CException
{
public:
	CException(int iErrno, const char* errMsg)
	{
		_iErrno = iErrno;
		if(errMsg != NULL)
		{
			_pErrMsg = new char[strlen(errMsg) + 1];
			strcpy(_pErrMsg,errMsg);
		}
	}
	virtual ~CException()
	{
		if(_pErrMsg != NULL)
		{
			delete _pErrMsg;
			_pErrMsg = NULL;
		}
	}
	virtual int errCode() const{ return _iErrno;}
	virtual const char* what() const {return _pErrMsg; }

private:
	char* _pErrMsg;
	int _iErrno;
};
#endif
