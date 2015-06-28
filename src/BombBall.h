#ifndef __CBOMBBALL_H__
#define __CBOMBBALL_H__

#include "stdafx.h"
#include "CBMPLoader.h"

class CBombBall
{
public:
	CBombBall(){}
	~CBombBall(){}
	void Render(float size,float x,float y,float z,float angle);
	//void Update();
	bool LoadTexture(char* fileName);
	void ShowBitBomb();
private:
	CBMPLoader m_texture;
};

#endif