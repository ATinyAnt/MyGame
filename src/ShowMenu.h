#ifndef __SHOWMENU_H__
#define __SHOWMENU_H__

#include "stdafx.h"
#include "CBMPLoader.h"

#define SMNUM 8

class CShowMenu
{
public:
	CShowMenu();
	~CShowMenu();
	bool LoadTexture();
	int render(int x,int y,int w,int h,int type=0,float r=1.0,float g=1.0,float b=1.0);
	int renderbar(float w,float h);
private:
	CBMPLoader m_texture[SMNUM];
};

#endif