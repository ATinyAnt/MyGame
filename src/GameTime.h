#ifndef __GAMETIME_H__
#define __GAMETIME_H__

#include <time.h>
class CGameTime
{
public:
	CGameTime();
	~CGameTime();
	bool TimeInterval(clock_t tt);				//是否到达了某一个时间段
	bool ReachTime(clock_t rt);					//看是否到达某个时间		
	void SetTimenow(){rt_timenow=clock();};		//设置当前时间

private:
	clock_t rt_timenow;		//当前时间
	clock_t tt_timenow;		//当前时间
};


#endif