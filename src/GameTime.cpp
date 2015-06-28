#include "GameTime.h"


CGameTime::CGameTime()
{
	rt_timenow=clock();					//获得当前时间
	tt_timenow=clock();					//获得当前时间
	//timeclip=CLOCKS_PER_SEC/40;			//将1000毫秒除以40
}

CGameTime::~CGameTime()
{
	
}


bool CGameTime::ReachTime(clock_t rt)
{
	clock_t t=clock();
	if (t-rt_timenow >= rt)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CGameTime::TimeInterval(clock_t tt)
{
	static bool firstin=true;
	clock_t t=clock();
	if (firstin)
	{
		tt_timenow=t;
		firstin = false;
		return false;
	}
	else if (t-tt_timenow>=tt)
	{
		firstin=true;
		return true;
	}
	return false;
}