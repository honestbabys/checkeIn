#include "include/EpollSocketServer.h"
#include "include/CheckedResult.h"

//服务端
int main()
{
	CEpollSocketServer server(6000);
	//server接收到的信息未保存，无法处理
	CCheckedResult chkResult;
	//string strResponse = chkResult.getCmd(strCmd);
	return 0;
}
