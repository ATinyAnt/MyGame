
#ifndef	__GLFONT_H__
#define	__GLFONT_H__

#include "stdafx.h"

/** ���������� */
class GLFont															
{
public:
    /** ���캯������������ */
	GLFont();
	~GLFont();
    ///��Ա����
	bool InitFont(int size=-16);  /**< ��ʼ������ */
	void PrintText(const char *string, float x, float y); /**< ��(x,y)�����string���� */
	
protected:
	HFONT m_hFont;  /**< ������ */
		
};

#endif	// __GLFONT_H__