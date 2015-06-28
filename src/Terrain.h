

#ifndef __TERRAIN_H__
#define __TERRAIN_H__


#include "stdafx.h"
#include "CBMPLoader.h"
#include "camera.h"

//������װ������
struct decorate
{
	char *name;
	bool hasfile;
	CBMPLoader texture;
};


/** ������ */
class CTerrain
{
public:
	
	/** ���캯�� */
	CTerrain();

	~CTerrain();

	/** ��ʼ������ */
	bool init();

	/** ��Ⱦ���� */
	void render(int num);

	/** �����Ƿ�ʹ��ϸ�������־ */ 
	void setDetail(bool _bDetail) { m_bDetail = _bDetail;}
		
	/** ��õ���߶� */
	float getAveHeight(float x, float z);
	
	/** �õ���ǰTerrainָ�� */
	static CTerrain* GetTerrainPointer() { return m_pTerrain;}

	void buildwall(int wallname);			//�����ǽ

	//void buildwall2();

	void buildcube();			//���������壬���ڽ��������

	//void LoadCubeData();
	


private:

	/** ���õ��εĴ�С */
	void setSize(unsigned  int width, unsigned  int cell); 

	/** ����'.raw'�߶�ͼ */
	bool loadRawFile(const char* fileName);

	/** װ������ */
	bool loadTexture();

	//װ������������
	bool loadDecorate();

	/** ��õ�(x,y)�ĸ߶���Ϣ */
	int getHeight(int x, int y);

	/** ������������ */
	void setTexCoord(float x, float z);

	/** ������Ч */
	void initFog();

                      
	
public:
	static CTerrain*  m_pTerrain;        /**< ��ǰTerrainָ�� */
	unsigned  int     m_nWidth;          /**< ���������� */
	unsigned  int     m_nCellWidth;      /**< ÿһ���� */
   	BYTE*             m_pHeightMap;      /**< ��Ÿ߶���Ϣ */
	CBMPLoader        m_texture[4];      /**< ���������ϸ������ */
	//CBMPLoader        m_decorate[4];	 //������װ������	
	bool              m_bDetail;         /**< �Ƿ�ʹ��ϸ�������־ */
	int               m_DetailScale;     /**< ���ű��� */
//     int				  m_textureindex[4]; //��������
// 	char			  *decoratename[4];
	decorate		  m_decorate[4];	 //������װ������

private:
	PFNGLMULTITEXCOORD2FARBPROC		m_glMultiTexCoord2fARB ;
	PFNGLACTIVETEXTUREARBPROC		m_glActiveTextureARB;
};

#define WALL  0
#define WALL2 1

#endif //__TERRAIN_H__

