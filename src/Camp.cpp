
#include "Camp.h"
#include "luadata.h"

CCamp::CCamp()
: m_glActiveTextureARB2(NULL)
, m_glMultiTexCoord2fARB2(NULL)
{

}

CCamp::~CCamp()
{
	for(int i=0; i<TXTNUM; i++)
	{
		m_texture[i].FreeImage();
		glDeleteTextures(1,&m_texture[i].ID);
	}
}

bool CCamp::Init()
{
	m_glActiveTextureARB2	= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	m_glMultiTexCoord2fARB2	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

	if(!m_glActiveTextureARB2 || !m_glMultiTexCoord2fARB2)
	{
		//输出错误信息
		MessageBox(NULL, "不支持多重纹理！", "错误", MB_OK);
		exit(0);
		//setDetail(false);
	}

	/** 载入纹理 */
	LoadTexture();
	LoadMonsterData();
	/** 初始化雾效 */
	//initFog();
	return true;
}


bool CCamp::LoadMonsterData()
{
	LuaData lua;
	lua.InitLuaEnv();
	//lua.SetLuaFile("Lua\\baseinfo.lua");
	if (!lua.LoadLuaFile("baseinfo.lua"))
	{
		MessageBox(NULL,"加载数据文件出错","CCamp 出错",MB_OK);
		exit(0);
	}
	lua.getGlobalProc("getBaseScene0Data");
	lua_pcall(lua.GetLuaEnv(), 0, 21, 0);
	int j=0;
	for(int i=-21;i<0;i=i+7)
	{
		god[-(i/7)-1].m_GodCoord.z=lua_tonumber(lua.GetLuaEnv(),i+6);
		god[-(i/7)-1].m_GodCoord.y=lua_tonumber(lua.GetLuaEnv(),i+5);
		god[-(i/7)-1].m_GodCoord.x=lua_tonumber(lua.GetLuaEnv(),i+4);
		char *Wpmd2file=(char *)lua_tostring(lua.GetLuaEnv(),i+3);
		char *Wptextture=(char *)lua_tostring(lua.GetLuaEnv(),i+2);
		char *Md2file=(char *)lua_tostring(lua.GetLuaEnv(),i+1);
		char *Textture=(char *)lua_tostring(lua.GetLuaEnv(),i);
		god[-(i/7)-1].m_weapon.Load(Wpmd2file,Wptextture);	
		god[-(i/7)-1].md2.Load(Md2file,Textture);
		

	}
	return true;
}

bool CCamp::LoadTexture()
{
	char* fileName[] = {"../data/ground1.bmp","../data/wall1.bmp","../data/zhuangshi.bmp"};
	//sizeof(fileName)/sizeof(char *)的大小
	for(int i=0; i < TXTNUM; i++)
	{
		//MessageBox(NULL,"fdf","sdsd",MB_OK);
		if(!m_texture[i].LoadBitmap(fileName[i]) )                    /**< 载入位图文件 */
		{
			MessageBox(NULL,"装载位图文件失败！","CCamp 错误",MB_OK);       /**< 如果载入失败则弹出对话框 */
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


void CCamp::Render()
{
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	
	glBindTexture(GL_TEXTURE_2D, m_texture[0].ID);
	glBegin(GL_QUADS);
	float coord_x;
	float coord_y=0.0;
	float coord_z;
	float b_coord_x=-200.0;
	float b_coord_z=500.0;
	int len_x=20;
	int len_z=30;

	for (int i=0;i<len_x;++i)
	{
		//coord_y=0.0;
		coord_x=b_coord_x+(i*20);
		//渲染地形
		for(int j=0;j<len_z;++j)
		{
			if(i<5 || i>=15 || j<5 || j>=25)
			{
				coord_y=10.0;
			}
			else
			{
				coord_y=0.0;
			}
			coord_z=b_coord_z+(j*20);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( coord_x,      coord_y, coord_z);	
			glTexCoord2f(1.0f, 0.0f); glVertex3f( coord_x+20.0, coord_y, coord_z);	
			glTexCoord2f(1.0f, 1.0f); glVertex3f( coord_x+20.0, coord_y, coord_z-20.0);	
			glTexCoord2f(0.0f, 1.0f); glVertex3f( coord_x,      coord_y, coord_z-20.0);
		}
		
		
	}
	
	glEnd();

	glBindTexture(GL_TEXTURE_2D, m_texture[1].ID);
	glBegin(GL_QUADS);
	//渲染围墙
	//北、南两面墙

	for (int i=0;i<len_x;++i)
	{
		coord_x=b_coord_x+(i*20);
		for(int k=0;k<4;++k)
		{
			coord_y=10.0*k+10.0;
			//北墙(高)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( coord_x,      coord_y,      b_coord_z-20.0);	
			glTexCoord2f(1.0f, 0.0f); glVertex3f( coord_x+20.0, coord_y,      b_coord_z-20.0);	
			glTexCoord2f(1.0f, 1.0f); glVertex3f( coord_x+20.0, coord_y+10.0, b_coord_z-20.0);	
			glTexCoord2f(0.0f, 1.0f); glVertex3f( coord_x,      coord_y+10.0, b_coord_z-20.0);

			//南墙(高)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( coord_x+20.0, coord_y,       b_coord_z+400.0-20.0+200.0);	
			glTexCoord2f(1.0f, 0.0f); glVertex3f( coord_x,      coord_y,       b_coord_z+400.0-20.0+200.0);	
			glTexCoord2f(1.0f, 1.0f); glVertex3f( coord_x,      coord_y+10.0,  b_coord_z+400.0-20.0+200.0);	
			glTexCoord2f(0.0f, 1.0f); glVertex3f( coord_x+20.0, coord_y+10.0,  b_coord_z+400.0-20.0+200.0);
			
			
			if(i>=5 && i<15 && k<1)
			{
				coord_y=10.0*k;
				//北墙(矮)			
				glTexCoord2f(0.0f, 0.0f); glVertex3f( coord_x,      coord_y,      b_coord_z-20.0+100.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( coord_x+20.0, coord_y,      b_coord_z-20.0+100.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( coord_x+20.0, coord_y+10.0, b_coord_z-20.0+100.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( coord_x,      coord_y+10.0, b_coord_z-20.0+100.0);

				//南墙(矮)
				glTexCoord2f(0.0f, 0.0f); glVertex3f( coord_x+20.0, coord_y,       b_coord_z+400.0-20.0+100.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( coord_x,      coord_y,       b_coord_z+400.0-20.0+100.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( coord_x,      coord_y+10.0,  b_coord_z+400.0-20.0+100.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( coord_x+20.0, coord_y+10.0,  b_coord_z+400.0-20.0+100.0);
			}	
		}
	}
	coord_y=0.0;
	for (int j=0;j<len_z;++j)
	{
		coord_z=b_coord_z+(j*20);
		for(int k=0;k<4;++k)
		{
			coord_y=10.0*k+10.0;
			//西墙(高)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( b_coord_x, coord_y,      coord_z);	
			glTexCoord2f(1.0f, 0.0f); glVertex3f( b_coord_x, coord_y,	     coord_z-20.0);	
			glTexCoord2f(1.0f, 1.0f); glVertex3f( b_coord_x, coord_y+10.0, coord_z-20.0);	
			glTexCoord2f(0.0f, 1.0f); glVertex3f( b_coord_x, coord_y+10.0, coord_z);

			//东墙(高)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( b_coord_x+400.0, coord_y,      coord_z-20.0);	
			glTexCoord2f(1.0f, 0.0f); glVertex3f( b_coord_x+400.0, coord_y,      coord_z);	
			glTexCoord2f(1.0f, 1.0f); glVertex3f( b_coord_x+400.0, coord_y+10.0, coord_z);	
			glTexCoord2f(0.0f, 1.0f); glVertex3f( b_coord_x+400.0, coord_y+10.0, coord_z-20.0);

			//if(k==2) break;
			if(j>=5 && j<25 && k<1)
			{
				coord_y=10.0*k;
				//西墙(矮)
				glTexCoord2f(0.0f, 0.0f); glVertex3f( b_coord_x+100.0, coord_y,      coord_z);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( b_coord_x+100.0, coord_y,	     coord_z-20.0);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( b_coord_x+100.0, coord_y+10.0, coord_z-20.0);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( b_coord_x+100.0, coord_y+10.0, coord_z);

				//东墙(矮)
				glTexCoord2f(0.0f, 0.0f); glVertex3f( b_coord_x+300.0, coord_y,      coord_z-20.0);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f( b_coord_x+300.0, coord_y,      coord_z);	
				glTexCoord2f(1.0f, 1.0f); glVertex3f( b_coord_x+300.0, coord_y+10.0, coord_z);	
				glTexCoord2f(0.0f, 1.0f); glVertex3f( b_coord_x+300.0, coord_y+10.0, coord_z-20.0);
			}
		}

	}
	
	glEnd();
	glPopMatrix();
	

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);    //去除这句后会显示背景
	//glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0f);
	glBindTexture(GL_TEXTURE_2D, m_texture[2].ID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( -80.0f,  0.01f, 900.0f);	
	glTexCoord2f(1.0f, 0.0f); glVertex3f(  80.0f,  0.01f, 900.0f);	
	glTexCoord2f(1.0f, 1.0f); glVertex3f(  80.0f,  0.01f, 740.0f);	
	glTexCoord2f(0.0f, 1.0f); glVertex3f( -80.0f,  0.01f, 740.0f);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	for (int i=0;i<3;i++)
	{
// 		god[i].md2.SetState(MODEL_IDLE);
// 		god[i].m_weapon.SetState(MODEL_IDLE);
		//Monsters[i]
		glPushMatrix();
		
		glTranslatef(god[i].m_GodCoord.x,god[i].m_GodCoord.y,god[i].m_GodCoord.z);
		glRotatef(-90.0, 1.0f, 0.0, 0.0);	//两个是调整坐标
		glRotatef(90,0.0f,0.0f,1.0f);
		glScalef(0.6f,0.6f,0.6f);
		int *frame=god[i].md2.GetFrame(MODEL_IDLE);
		god[i].md2.Animate(frame[0],frame[1],0.2f);
		god[i].m_weapon.Animate(frame[0],frame[1],0.2f);
		glPopMatrix();
	}
	
	//glDisable(GL_TEXTURE_2D);
}