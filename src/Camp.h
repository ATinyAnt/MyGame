#ifndef __CAMP_H__
#define __CAMP_H__

#include "stdafx.h"

#include "Vector.h"
#include "md2.h"
#include "CBMPLoader.h"

#define TXTNUM 3

struct God
{
	CMD2Model md2;
	CMD2Model m_weapon;
	Vector3 m_GodCoord;
}/*god[3]*/;

class CCamp
{

public:
	CCamp();
	~CCamp();
	bool Init();
	bool LoadMonsterData();
	bool LoadTexture();
	void Render();

private:
	CBMPLoader m_texture[TXTNUM];
	God god[3];


	PFNGLMULTITEXCOORD2FARBPROC		m_glMultiTexCoord2fARB2;
	PFNGLACTIVETEXTUREARBPROC		m_glActiveTextureARB2;
};


#endif