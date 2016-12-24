/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   CheckedMacroes.h
* FUNCTION:   macros defines file
* AUTHOR:     sam
* DATE:       2016-12-22
* VERSION:    1.0
* NOTES:      header file
****************************************************************/
#include <stdio.h>

#ifndef _CHECKEDMACROES_H
#define _CHECKEDMACROES_H
#define CHECKED_TRACE(STR) printf("Filename %s Line %d tracing %s\r\n",__FILE__,__LINE__,STR);  
#define CHECKED_DEBUG(...) printf("Filename %s Line %d debug ",__FILE__,__LINE__); \
	printf(__VA_ARGS__); \
	printf("\r\n");
#endif

