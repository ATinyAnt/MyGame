#ifndef __GAMETIME_H__
#define __GAMETIME_H__

#include <time.h>
class CGameTime
{
public:
	CGameTime();
	~CGameTime();
	bool TimeInterval(clock_t tt);				//�Ƿ񵽴���ĳһ��ʱ���
	bool ReachTime(clock_t rt);					//���Ƿ񵽴�ĳ��ʱ��		
	void SetTimenow(){rt_timenow=clock();};		//���õ�ǰʱ��

private:
	clock_t rt_timenow;		//��ǰʱ��
	clock_t tt_timenow;		//��ǰʱ��
};


#endif