#ifndef __LOG_H__
#define __LOG_H__

#include <time.h>
#include <fstream>
using namespace std;

#define EMPTYLOG 0
#define RETAINLOG 1

class CLog
{
public:
	CLog();
	~CLog();
	void WriteLog(char *logfile,char *log,int flag=RETAINLOG);
private:
};

#endif