

#include "Terrain.h"
//#include "CBMPLoader.h"




/** ��ǰCTerrainָ�� */
CTerrain* CTerrain::m_pTerrain = NULL;


/** ���캯�� */
CTerrain::CTerrain():m_bDetail(true),m_DetailScale(128)
{
	/** ���õ��δ�С */
	setSize(MAP_WIDTH,CELL_WIDTH);
	
	/** Ϊ���θ̷߳����ڴ�,����ʼ�� */
	m_pHeightMap = new BYTE[m_nWidth * m_nWidth];
	for(int i=0; i<m_nWidth * m_nWidth; i++)
		m_pHeightMap[i] = 0;

	m_pTerrain = this;

	
		
}

/** �������� */
CTerrain::~CTerrain()
{
	/** ɾ���ڴ��ָ�� */
	if(m_pHeightMap)
	{
		delete[] m_pHeightMap;
	    m_pHeightMap = 0;
	}
	
	/** ɾ�����������ռ���ڴ� */	
	for(int i=0; i<4; i++)
	{
		m_texture[i].FreeImage();
	    glDeleteTextures(1,&m_texture[i].ID);
	}	
}

/** ��ʼ����Ч */
void CTerrain::initFog()
{
	float fogColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

	//glEnable(GL_FOG);

	glFogi(GL_FOG_MODE, GL_EXP);             /** ������Ч��ģʽ */
	glFogfv(GL_FOG_COLOR, fogColor);         /** ָ�������ɫ */
	glFogf(GL_FOG_DENSITY, 0.001f);          /** �������Ũ�� */
	glFogf(GL_FOG_START, 1.0);               /** ����������Ŀ�ʼλ�� */
	glFogf(GL_FOG_END, 1000.0);              /** ����������Ľ���λ�� */
	glHint(GL_FOG_HINT, GL_DONT_CARE);       /** �涨��Ч�������� */

}

/** ��ʼ������ */
bool CTerrain::init()
{
	
	m_glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	m_glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

	if(!m_glActiveTextureARB || !m_glMultiTexCoord2fARB)
	{
		//���������Ϣ
		MessageBox(NULL, "��֧�ֶ�������", "����", MB_OK);
		exit(0);
		//setDetail(false);
	}

	/** ����߶��ļ� */
	//loadRawFile("data/terrain.raw");

	/** �������� */
	loadTexture();
	
	/** ��ʼ����Ч */
    //initFog();
	
	m_decorate[WALL].hasfile=true;
	m_decorate[WALL].name="../data/wall.bmp";

	m_decorate[WALL2].hasfile=true;
	m_decorate[WALL2].name="../data/wall2.bmp";

	loadDecorate();
	return true;
}

/** ���õ��δ�С */
void CTerrain::setSize(unsigned  int width, unsigned  int cell)
{
	m_nWidth = width;
	m_nCellWidth = cell; 
}

/** ��ø߶�ͼ�и߶�ֵ */
int CTerrain::getHeight(int x,int y)
{
	if(!m_pHeightMap)
		return 0;
	int xx = x % m_nWidth;
	int yy = y % m_nWidth;
    /** ���ص�ǰ�߶� */
	return m_pHeightMap[(xx + yy * m_nWidth)];
}

/** ��õ���߶� */
float CTerrain::getAveHeight(float x,float z)
{
	float CameraX, CameraZ;

	CameraX = x / m_nCellWidth;
	CameraZ = z / m_nCellWidth;

	/** ����߳�����(Col0, Row0)��(Col1, Row1) */
	int col0 = int(CameraX);
	int row0 = int(CameraZ);
	unsigned int col1 = col0 + 1;
	unsigned int row1 = row0 + 1;

	/** ȷ����Ԫ���겻�����߳����� */
	if (col1 > m_nWidth) col1 = 0;
	if (row1 > m_nWidth) row1 = 0;

	/** ��ȡ��Ԫ���ĸ��ǵĸ߶� */
	float h00 = (float)(m_pHeightMap[col0*m_nCellWidth + row0*m_nCellWidth*m_nWidth]);
	float h01 = (float)(m_pHeightMap[col1*m_nCellWidth + row0*m_nCellWidth*m_nWidth]);
	float h11 = (float)(m_pHeightMap[col1*m_nCellWidth + row1*m_nCellWidth*m_nWidth]);
	float h10 = (float)(m_pHeightMap[col0*m_nCellWidth + row1*m_nCellWidth*m_nWidth]);

	/** ��������������ڵ�Ԫ���λ�� */
	float tx = CameraX - int(CameraX);
	float ty = CameraZ - int(CameraZ);

	/** ����˫���Բ�ֵ�õ�����߶� */
	float txty = tx * ty;

	float final_height	= h00 * (1.0f - ty - tx + txty)
						+ h01 * (tx - txty)
						+ h11 * txty
						+ h10 * (ty - txty);
	return final_height;
}

/** ����߶�ͼ */
bool CTerrain::loadRawFile(const char* fileName)
{
	FILE *pFile = NULL;

	/** ���ļ� */
	pFile = fopen( fileName, "rb" );

	/** ������ */
	if ( pFile == NULL )	
	{
		/** ���������Ϣ��������false */
		MessageBox(NULL, "�򿪸߶�ͼ�ļ�ʧ�ܣ�", "����", MB_OK);
		exit(0);
	}

	/** ��ȡ�߶�ͼ�ļ� */
	fread( m_pHeightMap, 1, m_nWidth*m_nWidth, pFile );

	/** ��ȡ������� */
	int result = ferror( pFile );

	/** ��������� */
	if (result)
	{
		MessageBox(NULL, "�޷���ȡ�߶����ݣ�", "����", MB_OK);
		exit(0);
	}
    
	/** �ر��ļ����ɹ����� */
	fclose(pFile);
	return true;
}

/** ������������ */
void CTerrain::setTexCoord(float x,float z)
{
	
	float u =  (float)x / (float)m_nWidth;
	float v = -(float)z / (float)m_nWidth;
	
	///���õ����������������
	m_glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);

	///����ϸ���������������
	m_glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);
}


/** ����������� */
bool CTerrain::loadTexture()
{
	char* fileName[] = {"../data/terrain.bmp","../data/detail.bmp","../data/groundb.bmp","../data/ground.bmp"};
	//sizeof(fileName)/sizeof(char *)�Ĵ�С
	for(int i=0; i < 4; i++)
	{
		//MessageBox(NULL,"fdf","sdsd",MB_OK);
		if(!m_texture[i].LoadBitmap(fileName[i]) )                    /**< ����λͼ�ļ� */
		{
			MessageBox(NULL,"װ��λͼ�ļ�ʧ�ܣ�","CTerrain ����",MB_OK);       /**< �������ʧ���򵯳��Ի��� */
			exit(0);
		}
		glGenTextures(1, &m_texture[i].ID);                            /**< ����һ������������� */
		
			
		glBindTexture(GL_TEXTURE_2D, m_texture[i].ID);                 /**< ����������� */
		
		/** �����˲� */
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		
		/** �������� */
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
				MessageBox(NULL,"װ��λͼ�ļ�ʧ�ܣ�","CTerrain ����",MB_OK);       /**< �������ʧ���򵯳��Ի��� */
				exit(0);
			}
			glGenTextures(1, &m_decorate[i].texture.ID);                            /**< ����һ������������� */


			glBindTexture(GL_TEXTURE_2D, m_decorate[i].texture.ID);                 /**< ����������� */

			/** �����˲� */
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

// 			/** �������� */
// 			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_decorate[i].texture.imageWidth,
// 				m_decorate[i].texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
// 				m_decorate[i].texture.image);
		}
	}
	return true;
}


/** ��Ⱦ���� */
void CTerrain::render(int num)
{
	//m_bDetail=false;
	num=num-1;
	int X = 0, Y = 0;						
	float x, y, z;							
	bool bSwitchSides = false;

// 	/** ���߶�ͼ�Ƿ���Ч */
// 	if(!m_pHeightMap) 
// 		return;					

	/** ������ */
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	m_glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[2*num].ID);
	
	/** ��Ⱦϸ������ */
	if(m_bDetail)
	{
		m_glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);

		glBindTexture(GL_TEXTURE_2D, m_texture[2*num+1].ID);
		
		/** �任������� */
		glMatrixMode(GL_TEXTURE);
		    glLoadIdentity();
			glScalef((float)m_DetailScale, (float)m_DetailScale, 1);
		glMatrixMode(GL_MODELVIEW);
	}

	/** ���������δ� */
	glBegin( GL_TRIANGLE_STRIP );			
    int halfWidth=m_nWidth/2;
	/** ����(X)��ʼѭ�� */
	//for ( X = 0; X <= m_nWidth; X += m_nCellWidth )
	for ( X = 0; X <= m_nWidth; X += m_nCellWidth )
	{
		/** �������Ƿ���Ҫ���෴˳����� */
		if(bSwitchSides)
		{	
			/** ���Ƶ��ε�һ�� */
			for ( Y = m_nWidth; Y >= 0; Y -= m_nCellWidth )
			{
				/** ����(X, Y, Z) */		
				x = X- halfWidth;
				//x=X;
				y = getHeight( X, Y );	
				z = Y- halfWidth;
				//z=Y;

				/** ָ����������,�����Ͷ��� */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);		

				/** ����(X + m_nCellWidth, Y, Z) */		
				x = X + m_nCellWidth; 
				y = getHeight( X + m_nCellWidth, Y ); 
				z = Y;

				/** ָ����������,�����Ͷ��� */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);			
			}
		}
		else
		{	
			/** ���Ƶ��ε�һ�� */
			for ( Y = 0; Y <= m_nWidth; Y += m_nCellWidth )
			{
				/** ����(X + m_nCellWidth, Y, Z) */	
				x = X + m_nCellWidth; 
				y = getHeight( X + m_nCellWidth, Y ); 
				z = Y;

				/** ָ����������,�����Ͷ��� */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);

				/** ���� (X, Y, Z) */		
				x = X;							
				y = getHeight( X, Y );	
				z = Y;							

				/** ָ����������,�����Ͷ��� */
				//setFogCoord(m_FogDepth, (float)y);
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);		
			}
		}
			
		/** �任���� */
		bSwitchSides = !bSwitchSides;
	}
	
	/** ���ƽ��� */
	glEnd();

	///�ر�����Ԫ1
	m_glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);

	//�ر�����Ԫ0
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


//��һ������
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

	float Width=48.0;			//���곤Ϊ470,��ΪZ�Ŵ���10��
	int temp;
	//float mul=m_decorate[WALL].texture.imageWidth/m_decorate[WALL].texture.imageHeight;
	float m_Width=Width;
	for (int i=0;i<m_Width*30/(2*45*3);i++)			//*30:��Ϊ�Ŵ���30����2��ͼƬ�Ŀ��Ǹߵ�2����45����Ϊ�Ŵ���45�� 
	{
		temp=2*i;
		//��
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f-temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f-temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f-temp, 1.0f,  -m_Width);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f-temp, 1.0f,  -m_Width);
// 

		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f+temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f+temp, 0.0f,  -m_Width);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f+temp, 1.0f,  -m_Width);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f+temp, 1.0f,  -m_Width);


//��һ��ǽ(��)
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
	//��һ��ǽ


	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_decorate[wallname].texture.ID);
	glScalef(10,60,45);
	glBegin(GL_QUADS);
	for (int i=0;i<m_Width*30/(2*45*3);i++)			//*10:��Ϊ�Ŵ���10����2��ͼƬ�Ŀ��Ǹߵ�2����15����Ϊ�Ŵ���15��
	{
		//��
		temp=2*i;
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -m_Width, 0.0f,  1.0f-temp);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -m_Width, 0.0f, -1.0f-temp);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -m_Width, 1.0f, -1.0f-temp);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -m_Width, 1.0f,  1.0f-temp);
		

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-m_Width, 0.0f,  3.0f+temp);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-m_Width, 0.0f,  1.0f+temp);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-m_Width, 1.0f,  1.0f+temp);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-m_Width, 1.0f,  3.0f+temp);

		//��һ��ǽ(��)

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
			//x����Ӹ������������꿪ʼ����
			for(int i=0;i<2;i++)
			{
				//�����м�����ǽ
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-480.0f+(192*r)+(96*i), 0.0f,  60.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-384.0f+(192*r)+(96*i), 0.0f,  60.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-384.0f+(192*r)+(96*i), 60.0f,  60.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-480.0f+(192*r)+(96*i), 60.0f,  60.0);

				//�����м�����ǽ
				glTexCoord2f(0.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  -60.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  -60.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  -60.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  -60.0); 

			}
			break;
		case 1 :
		case 3 :
			//z�����ԭ�㿪ʼ����
			for(int i=0;i<2;i++)
			{
				//���������ǽ
				glTexCoord2f(0.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  330.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  330.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  330.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  330.0);

				glTexCoord2f(0.0f, 0.0f); glVertex3f( -480.0f+(192*r)+(96*i),  0.0f,  -210.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( -384.0f+(192*r)+(96*i),  0.0f,  -210.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( -384.0f+(192*r)+(96*i), 60.0f,  -210.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( -480.0f+(192*r)+(96*i), 60.0f,  -210.0);


				//��������ǽ
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
		//��������ǽ(ǰ)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -96,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -96,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -96, 60.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -96, 60.0f, 210.0f+(60*i));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( 288,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 288,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 288, 60.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 288, 60.0f, 210.0f+(60*i));



		//��������ǽ(��)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -96,  0.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -96,  0.0f, -210.0f-(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -96, 60.0f, -210.0f-(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -96, 60.0f, -210.0f-(60*(i-1)));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( 288,  0.0f, -210.0f-(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 288,  0.0f, -210.0f-(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 288, 60.0f, -210.0f-(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 288, 60.0f, -210.0f-(60*(i-1)));

		//��������ǽ���У�
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 96,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 96,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 96, 60.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 96, 60.0f, -60.0f+(60*i));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( -288,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -288,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -288, 60.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -288, 60.0f, -60.0f+(60*i));

		//��������ǽ��ǰ��
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 96,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 96,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 96, 60.0f, 210.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 96, 60.0f, 210.0f+(60*(i-1)));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( -288,  0.0f, 210.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -288,  0.0f, 210.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -288, 60.0f, 210.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -288, 60.0f, 210.0f+(60*(i-1)));

		//��������ǽ���У�
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -96,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -96,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -96, 60.0f, -60.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -96, 60.0f, -60.0f+(60*(i-1)));

		glTexCoord2f(0.0f, 0.0f); glVertex3f( 288,  0.0f, -60.0f+(60*(i-1)));	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 288,  0.0f, -60.0f+(60*i));	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 288, 60.0f, -60.0f+(60*i));	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 288, 60.0f, -60.0f+(60*(i-1)));

		//��������ǽ����
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


