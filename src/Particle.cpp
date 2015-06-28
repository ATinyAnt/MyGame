#include "Particle.h"

/** 构造函数 */
CParticle::CParticle():m_iNum(0),m_pList(NULL)
{	
}

/** 析构函数 */
CParticle::~CParticle()
{
	if(m_pList != NULL)
	{
		delete []m_pList;
		m_pList = NULL;
	}

}

/** 粒子的初始化 */
bool CParticle::Init(int _num)
{
	m_iNum = _num;
	m_pList = new Particle[m_iNum];
	if(m_pList == NULL)
		return false;
	return true;
}

