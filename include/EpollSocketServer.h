/****************************************************************
* (C) Copyright by sam. All rights reserved.
* FILENAME:   EpollSocketServer.h
* FUNCTION:   the class CEpollSocketServer declaration file
* AUTHOR:     sam
* DATE:       2016-12-22
* VERSION:    1.0
* NOTES:      header file
****************************************************************/
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>

#ifndef _EPOLLSOCKETSERVER_H
#define _EPOLLSOCKETSERVER_H

class CEpollSocketServer
{
public:
	CEpollSocketServer(int port);
	~CEpollSocketServer();

private:
	int bindSocket(int port);
	int listenSocket();
	void doEpoll(int listenfd);
	void handleEvents(int epollfd, struct epoll_event *events,int num,int listenfd,char *buf);
	void hanldeAccpet(int epollfd, int listenfd);
	void doRead(int epollfd, int fd, char* buf);
	void doWrite(int epollfd,int fd,char *buf);
	void addEvent(int epollfd,int fd,int state); 
	void modifyEvent(int epollfd,int fd,int state);
	void deleteEvent(int epollfd,int fd,int state);
	void closeHandle();

private:
	int _iListenFd;
	int _iEpollFd; 
	std::string _strMsg;
	const static int EPOLLEVENTS = 100;
	const static int FDSIZE = 1000;
	const static int MAXSIZE = 1024;
	const static int LISTENQ = 5;
};

#endif
