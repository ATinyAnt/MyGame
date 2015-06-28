#include "Log.h"

CLog::CLog()
{

}

CLog::~CLog()
{

}

void CLog::WriteLog(char *logfile,char *log,int flag)
{
	ofstream outlog;
	outlog.open(logfile,/*ios::in|*/ios::out|ios::ate|ios::app/*|ios::_Noreplace*/);
	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X ",localtime(&t) ); 

	outlog<<tmp<<":"<<log<<"\n";
	outlog.close();
}