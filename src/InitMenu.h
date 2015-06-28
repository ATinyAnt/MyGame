#ifndef __INITMENU_H__
#define __INITMENU_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "Font.h"

#define TXNUM 4

class CInitMenu
{
public:
	CInitMenu();
	~CInitMenu();
	bool loadTexture();
	int render(int x,int y,int w,int h,int mx,int my,bool LButtonDown,int type=BEGINGAME);
//	static CInitMenu * Instance();

private:
	CBMPLoader m_texture[TXNUM];				//游戏初始化背景,游戏按钮，“开始游戏”、“退出游戏”。。等等
	GLFont font;
//	static CInitMenu * _instance;
// 	CBMPLoader m_initbackground;			  
// 	CBMPLoader m_button[2];					//
};



#endif