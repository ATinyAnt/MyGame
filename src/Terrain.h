

#ifndef __TERRAIN_H__
#define __TERRAIN_H__


#include "stdafx.h"
#include "CBMPLoader.h"
#include "camera.h"

//其它的装饰纹理
struct decorate
{
	char *name;
	bool hasfile;
	CBMPLoader texture;
};


/** 地形类 */
class CTerrain
{
public:
	
	/** 构造函数 */
	CTerrain();

	~CTerrain();

	/** 初始化地形 */
	bool init();

	/** 渲染地形 */
	void render(int num);

	/** 设置是否使用细节纹理标志 */ 
	void setDetail(bool _bDetail) { m_bDetail = _bDetail;}
		
	/** 获得地面高度 */
	float getAveHeight(float x, float z);
	
	/** 得到当前Terrain指针 */
	static CTerrain* GetTerrainPointer() { return m_pTerrain;}

	void buildwall(int wallname);			//建造城墙

	//void buildwall2();

	void buildcube();			//建造立方体，用于将区域隔开

	//void LoadCubeData();
	


private:

	/** 设置地形的大小 */
	void setSize(unsigned  int width, unsigned  int cell); 

	/** 载入'.raw'高度图 */
	bool loadRawFile(const char* fileName);

	/** 装载纹理 */
	bool loadTexture();

	//装载其它的纹理
	bool loadDecorate();

	/** 获得点(x,y)的高度信息 */
	int getHeight(int x, int y);

	/** 设置纹理坐标 */
	void setTexCoord(float x, float z);

	/** 设置雾效 */
	void initFog();

                      
	
public:
	static CTerrain*  m_pTerrain;        /**< 当前Terrain指针 */
	unsigned  int     m_nWidth;          /**< 地形网格数 */
	unsigned  int     m_nCellWidth;      /**< 每一格宽度 */
   	BYTE*             m_pHeightMap;      /**< 存放高度信息 */
	CBMPLoader        m_texture[4];      /**< 地面纹理和细节纹理 */
	//CBMPLoader        m_decorate[4];	 //其它的装饰纹理	
	bool              m_bDetail;         /**< 是否使用细节纹理标志 */
	int               m_DetailScale;     /**< 缩放比例 */
//     int				  m_textureindex[4]; //纹理索引
// 	char			  *decoratename[4];
	decorate		  m_decorate[4];	 //其它的装饰纹理

private:
	PFNGLMULTITEXCOORD2FARBPROC		m_glMultiTexCoord2fARB ;
	PFNGLACTIVETEXTUREARBPROC		m_glActiveTextureARB;
};

#define WALL  0
#define WALL2 1

#endif //__TERRAIN_H__

