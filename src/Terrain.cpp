

#include "Terrain.h"
//#include "CBMPLoader.h"




/** 当前CTerrain指针 */
CTerrain* CTerrain::m_pTerrain = NULL;


/** 构造函数 */
CTerrain::CTerrain():m_bDetail(true),m_DetailScale(128)
{
	/** 设置地形大小 */
	setSize(MAP_WIDTH,CELL_WIDTH);
	
	/** 为地形高程分配内存,并初始化 */
	m_pHeightMap = new BYTE[m_nWidth * m_nWidth];
	for(int i=0; i<m_nWidth * m_nWidth; i++)
		m_pHeightMap[i] = 0;

	m_pTerrain = this;

	
		
}

/** 析构函数 */
CTerrain::~CTerrain()
{
	/** 删除内存和指针 */
	if(m_pHeightMap)
	{
		delete[] m_pHeightMap;
	    m_pHeightMap = 0;
	}
	
	/** 删除纹理对象及其占用内存 */	
	for(int i=0; i<4; i++)
	{
		m_texture[i].FreeImage();
	    glDeleteTextures(1,&m_texture[i].ID);
	}	
}

/** 初始化雾效 */
void CTerrain::initFog()
{
	float fogColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

	//glEnable(GL_FOG);

	glFogi(GL_FOG_MODE, GL_EXP);             /** 设置雾效的模式 */
	glFogfv(GL_FOG_COLOR, fogColor);         /** 指定雾的颜色 */
	glFogf(GL_FOG_DENSITY, 0.001f);          /** 设置雾的浓度 */
	glFogf(GL_FOG_START, 1.0);               /** 设置线性雾的开始位置 */
	glFogf(GL_FOG_END, 1000.0);              /** 设置线性雾的结束位置 */
	glHint(GL_FOG_HINT, GL_DONT_CARE);       /** 规定雾化效果的质量 */

}

/** 初始化地形 */
bool CTerrain::init()
{
	
	m_glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	m_glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

	if(!m_glActiveTextureARB || !m_glMultiTexCoord2fARB)
	{
		//输出错误信息
		MessageBox(NULL, "不支持多重纹理！", "错误", MB_OK);
		exit(0);
		//setDetail(false);
	}

	/** 载入高度文件 */
	//loadRawFile("data/terrain.raw");

	/** 载入纹理 */
	loadTexture();
	
	/** 初始化雾效 */
    //initFog();
	
	m_decorate[WALL].hasfile=true;
	m_decorate[WALL].name="../data/wall.bmp";

	m_decorate[WALL2].hasfile=true;
	m_decorate[WALL2].name="../data/wall2.bmp";

	loadDecorate();
	return true;
}

/** 设置地形大小 */
void CTerrain::setSize(unsigned  int width, unsigned  int cell)
{
	m_nWidth = width;
	m_nCellWidth = cell; 
}

/** 获得高度图中高度值 */
int CTerrain::getHeight(int x,int y)
{
	if(!m_pHeightMap)
		return 0;
	int xx = x % m_nWidth;
	int yy = y % m_nWidth;
    /** 返回当前高度 */
	return m_pHeightMap[(xx + yy * m_nWidth)];
}

/** 获得地面高度 */
float CTerrain::getAveHeight(float x,float z)
{
	float CameraX, CameraZ;

	CameraX = x / m_nCellWidth;
	CameraZ = z / m_nCellWidth;

	/** 计算高程坐标(Col0, Row0)，(Col1, Row1) */
	int col0 = int(CameraX);
	int row0 = int(CameraZ);
	unsigned int col1 = col0 + 1;
	unsigned int row1 = row0 + 1;

	/** 确保单元坐标不超过高程以外 */
	if (col1 > m_nWidth) col1 = 0;
	if (row1 > m_nWidth) row1 = 0;

	/** 获取单元的四个角的高度 */
	float h00 = (float)(m_pHeightMap[col0*m_nCellWidth + row0*m_nCellWidth*m_nWidth]);
	float h01 = (float)(m_pHeightMap[col1*m_nCellWidth + row0*m_nCellWidth*m_nWidth]);
	float h11 = (float)(m_pHeightMap[col1*m_nCellWidth + row1*m_nCellWidth*m_nWidth]);
	float h10 = (float)(m_pHeightMap[col0*m_nCellWidth + row1*m_nCellWidth*m_nWidth]);

	/** 计算机摄像机相对于单元格的位置 */
	float tx = CameraX - int(CameraX);
	float ty = CameraZ - int(CameraZ);

	/** 进行双线性插值得到地面高度 */
	float txty = tx * ty;

	float final_height	= h00 * (1.0f - ty - tx + txty)
						+ h01 * (tx - txty)
						+ h11 * txty
						+ h10 * (ty - txty);
	return final_height;
}

/** 载入高度图 */
bool CTerrain::loadRawFile(const char* fileName)
{
	FILE *pFile = NULL;

	/** 打开文件 */
	pFile = fopen( fileName, "rb" );

	/** 错误检查 */
	if ( pFile == NULL )	
	{
		/** 输出错误信息，并返回false */
		MessageBox(NULL, "打开高度图文件失败！", "错误", MB_OK);
		exit(0);
	}

	/** 读取高度图文件 */
	fread( m_pHeightMap, 1, m_nWidth*m_nWidth, pFile );

	/** 获取错误代码 */
	int result = ferror( pFile );

	/** 检查错误代码 */
	if (result)
	{
		MessageBox(NULL, "无法获取高度数据！", "错误", MB_OK);
		exit(0);
	}
    
	/** 关闭文件，成功返回 */
	fclose(pFile);
	return true;
}

/** 设置纹理坐标 */
void CTerrain::setTexCoord(float x,float z)
{
	
	float u =  (float)x / (float)m_nWidth;
	float v = -(float)z / (float)m_nWidth;
	
	///设置地面纹理的纹理坐标
	m_glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);

	///设置细节纹理的纹理坐标
	m_glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);
}


/** 载入地面纹理 */
bool CTerrain::loadTexture()
{
	char* fileName[] = {"../data/terrain.bmp","../data/detail.bmp","../data/groundb.bmp","../data/ground.bmp"};
	//sizeof(fileName)/sizeof(char *)的大小
	for(int i=0; i < 4; i++)
	{
		//MessageBox(NULL,"fdf","sdsd",MB_OK);
		if(!m_texture[i].LoadBitmap(fileName[i]) )                    /**< 载入位图文件 */
		{
			MessageBox(NULL,"装载位图文件失败！","CTerrain 错误",MB_OK);       /**< 如果载入失败则弹出对话框 */
			exit(0);
		}
		glGenTextures(1, &m_texture[i].ID);                            /**< 生成一个纹理对象名称 */
		
			
		glBindTexture(GL_TEXTURE_2D, m_texture[i].ID);                 /**< 创建纹理对象 */
		
		/** 控制滤波 */
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		
		/** 创建纹理 */
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_texture[i].imageWidth,
						m_texture[i].imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
						m_texture[i].image);
	}
  	
	return true;

}


bool CTerrain::loadDecorate()
{
	for(int i=0;i<4;i++)
	{
		if (m_decorate[i].hasfile!=true)
		{
			continue;
		}
		else
		{
			if(!m_decorate[i].texture.LoadBitmap(m_decorate[i].name))
			{
				MessageBox(NULL,"装载位图文件失败！","CTerrain 错误",MB_OK);       /**< 如果载入失败则弹出对话框 */
				exit(0);
			}
			glGenTextures(1, &m_decorate[i].texture.ID);                            /**< 生成一个纹理对象名称 */


			glBindTexture(GL_TEXTURE_2D, m_decorate[i].texture.ID);                 /**< 创建纹理对象 */

			/** 控制滤波 */
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

// 			/** 创建纹理 */
// 			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_decorate[i].texture.imageWidth,
// 				m_decorate[i].texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
// 				m_decorate[i].texture.image);
		}
	}
	return true;
}


/** 渲染地形 */
void CTerrain::render(int num)
{
	//m_bDetail=false;
	num=num-1;
	int X = 0, Y = 0;						
	float x, y, z;							
	bool bSwitchSides = false;

// 	/** 检查高度图是否有效 */
// 	if(!m_pHeightMap) 
// 		return;					

	/** 绑定纹理 */
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	m_glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[2*num].ID);
	
	/** 渲染细节纹理 */
	if(m_bDetail)
	{
		m_glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);

		glBindTexture(GL_TEXTURE_2D, m_texture[2*num+1].ID);
		
		/** 变换纹理矩阵 */
		glMatrixMode(GL_TEXTURE);
		    glLoadIdentity();
			glScalef((float)m_DetailScale, (float)m_DetailScale, 1);
		glMatrixMode(GL_MODELVIEW);
	}

	/** 绘制三角形带 */
	glBegin( GL_TRIANGLE_STRIP );			
    int halfWidth=m_nWidth/2;
	/** 从行(X)开始循环 */
	//for ( X = 0; X <= m_nWidth; X += m_nCellWidth )
	for ( X = 0; X <= m_nWidth; X += m_nCellWidth )
	{
		/** 检查该列是否需要从相反顺序绘制 */
		if(bSwitchSides)
		{	
			/** 绘制地形的一列 */
			for ( Y = m_nWidth; Y >= 0; Y -= m_nCellWidth )
			{
				/** 顶点(X, Y, Z) */		
				x = X- halfWidth;
				//x=X;
				y = getHeight( X, Y );	
				z = Y- halfWidth;
				//z=Y;

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);		

				/** 顶点(X + m_nCellWidth, Y, Z) */		
				x = X + m_nCellWidth; 
				y = getHeight( X + m_nCellWidth, Y ); 
				z = Y;

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);			
			}
		}
		else
		{	
			/** 绘制地形的一列 */
			for ( Y = 0; Y <= m_nWidth; Y += m_nCellWidth )
			{
				/** 顶点(X + m_nCellWidth, Y, Z) */	
				x = X + m_nCellWidth; 
				y = getHeight( X + m_nCellWidth, Y ); 
				z = Y;

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);

				/** 顶点 (X, Y, Z) */		
				x = X;							
				y = getHeight( X, Y );	
				z = Y;							

				/** 指定纹理坐标,并发送顶点 */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);		
			}
		}
			
		/** 变换开关 */
		bSwitchSides = !bSwitchSides;
	}
	
	/** 绘制结束 */
	glEnd();

	///关闭纹理单元1
	m_glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);

	//关闭纹理单元0
	m_glActiveTextureARB(GL_TEXTURE0_ARB);		
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();

	if (num==0)
	{
		buildwall(WALL);
	}
	else if (num==1)
	{
		//buildwall2();
		buildwall(WALL2);
		buildcube();
	}
	

}


//第一个场景
void CTerrain::buildwall(int wallname)
{
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glScalef(45,60,10);
	glBindTexture(GL_TEXTURE_2D, m_decorate[wallname].texture.ID);
	glBegin(GL_QUADS);

	float Width=48.0;			//坐标长为470,因为Z放大了10倍
	int temp;
	//float mul=m_decorate[WALL].texture.imageWidth/m_decorate[WALL].texture.imageHeight;
	float m_Width=Width;
	for (int i=0;i<m_Width*30/(2*45*3);i++)			//*30:因为放大了30倍；2：图片的宽是高的2倍；45：因为放大了45倍 
	{
		temp=2*i;
		//北
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f-temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f-temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f-temp, 1.0f,  -m_Width);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f-temp, 1.0f,  -m_Width);
// 

		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f+temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f+temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f+temp, 1.0f,  -m_Width);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f+temp, 1.0f,  -m_Width);


//另一块墙(南)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f-temp, 0.0f,  m_Width);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f-temp, 0.0f,  m_Width);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f-temp, 1.0f,  m_Width);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f-temp, 1.0f,  m_Width);
// 

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f+temp, 0.0f,  m_Width);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f+temp, 0.0f,  m_Width);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f+temp, 1.0f,  m_Width);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f+temp, 1.0f,  m_Width);

		

	}
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//另一面墙


	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_decorate[wallname].texture.ID);
	glScalef(10,60,45);
	glBegin(GL_QUADS);
	for (int i=0;i<m_Width*30/(2*45*3);i++)			//*10:因为放大了10倍；2：图片的宽是高的2倍；15：因为放大了15倍
	{
		//西
		temp=2*i;
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -m_Width, 0.0f,  1.0f-temp);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -m_Width, 0.0f, -1.0f-temp);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -m_Width, 1.0f, -1.0f-temp);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -m_Width, 1.0f,  1.0f-temp);
		

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-m_Width, 0.0f,  3.0f+temp);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-m_Width, 0.0f,  1.0f+temp);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-m_Width, 1.0f,  1.0f+temp);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-m_Width, 1.0f,  3.0f+temp);

		//另一块墙(东)

		glTexCoord2f(0.0f, 0.0f); glVertex3f( m_Width, 0.0f, -1.0f-temp);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( m_Width, 0.0f,  1.0f-temp);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( m_Width, 1.0f,  1.0f-temp);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( m_Width, 1.0f, -1.0f-temp);
		

		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_Width, 0.0f,  1.0f+temp);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_Width, 0.0f,  3.0f+temp);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_Width, 1.0f,  3.0f+temp);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_Width, 1.0f,  1.0f+temp);
	}

	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}





void CTerrain::buildcube()
{
	
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_decorate[WALL2].texture.ID);
	glBegin(GL_QUADS);

// 	LoadCubeCoord();
// 	for(itr=cube.begin();itr!=cube.end();itr++)
// 	{
// 		
// 	}
	for(int r=0;r<5;r++)
	{
		switch(r)
		{
		case 0 :
		case 2 :
		case 4 :
			//x坐标从负坐标的最大坐标开始计算
			for(int i=0;i<2;i++)
			{
				//正面中间三面墙
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-480.0f+(192*r)+(96*i), 0.0f,  60.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-384.0f+(192*r)+(96*i), 0.0f,  60.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-384.0f+(192*r)+(96*i), 60.0f,  60.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-480.0f+(192*r)+(96*i), 60.0f,  60.0);

				//背面中间三面墙
				glTexCoord2f(0.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  -60.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  -60.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  -60.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  -60.0); 

			}
			break;
		case 1 :
		case 3 :
			//z坐标从原点开始计算
			for(int i=0;i<2;i++)
			{
				//正面的四面墙
				glTexCoord2f(0.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  330.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  330.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  330.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  330.0);

				glTexCoord2f(0.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  -210.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  -210.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  -210.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  -210.0);


				//背面四面墙
				glTexCoord2f(0.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  -330.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  -330.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  -330.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  -330.0);

				glTexCoord2f(0.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  210.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  210.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  210.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  210.0);

			}
			break;
		}
	}
 	glEnd();
 	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	//glScalef(10,30,30);
	glBindTexture(GL_TEXTURE_2D, m_decorate[WALL2].texture.ID);
	glBegin(GL_QUADS);
	for (int i=1;i<=2;i++)
	{
		//右面两面墙(前)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -96,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -96,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -96, 60.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -96, 60.0f, 210.0f+(60*i));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( 288,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 288,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 288, 60.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 288, 60.0f, 210.0f+(60*i));



		//右面两面墙(后)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -96,  0.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -96,  0.0f, -210.0f-(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -96, 60.0f, -210.0f-(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -96, 60.0f, -210.0f-(60*(i-1)));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( 288,  0.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 288,  0.0f, -210.0f-(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 288, 60.0f, -210.0f-(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 288, 60.0f, -210.0f-(60*(i-1)));

		//右面两面墙（中）
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 96,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 96,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 96, 60.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 96, 60.0f, -60.0f+(60*i));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( -288,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -288,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -288, 60.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -288, 60.0f, -60.0f+(60*i));

		//左面两面墙（前）
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 96,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 96,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 96, 60.0f, 210.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 96, 60.0f, 210.0f+(60*(i-1)));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( -288,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -288,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -288, 60.0f, 210.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -288, 60.0f, 210.0f+(60*(i-1)));

		//左面两面墙（中）
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -96,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -96,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -96, 60.0f, -60.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -96, 60.0f, -60.0f+(60*(i-1)));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( 288,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 288,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 288, 60.0f, -60.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 288, 60.0f, -60.0f+(60*(i-1)));

		//左面两面墙（后）
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 96,  0.0f, -210.0f-(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 96,  0.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 96, 60.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 96, 60.0f, -210.0f-(60*i));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( -288,  0.0f, -210.0f-(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -288,  0.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -288, 60.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -288, 60.0f, -210.0f-(60*i));

	}

	glEnd();
	glPopMatrix();

}


