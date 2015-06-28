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
	CBMPLoader m_texture[TXNUM];				//��Ϸ��ʼ������,��Ϸ��ť������ʼ��Ϸ�������˳���Ϸ�������ȵ�
	GLFont font;
//	static CInitMenu * _instance;
// 	CBMPLoader m_initbackground;			  
// 	CBMPLoader m_button[2];					//
};



#endif