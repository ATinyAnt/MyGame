#include "ShowMenu.h"

CShowMenu::CShowMenu()
{

}

CShowMenu::~CShowMenu()
{

}

bool CShowMenu::LoadTexture()
{
	char* fileName[] = {
		"..\\data\\laserball.bmp",
		"..\\data\\firehand.bmp",
		"..\\data\\rush.bmp",
		"..\\data\\bar.bmp",
		"..\\data\\lightningicon.bmp",
		"..\\data\\run.bmp",
		"..\\data\\site.bmp",
		"..\\data\\transmit.bmp"
	};

	for(int i=0; i < SMNUM; i++)
	{
		if(!m_texture[i].LoadBitmap(fileName[i]) )                    /**< 载入位图文件 */
		{
			MessageBox(NULL,"装载位图文件失败！","CShowMenu 错误",MB_OK);       /**< 如果载入失败则弹出对话框 */
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



int CShowMenu::render(int x,int y,int w,int h,int tpye,float r,float g,float b)
{

	glLoadIdentity();

	glBindTexture  (GL_TEXTURE_2D,  m_texture[tpye].ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//画背景矩形
	glTranslatef(x,y,0.5f);
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0f);

	glTexCoord2f(1.0f, 0.0f);		
	glVertex3f(w, 0.0,  0.0f);		

	//up
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(w, h,  0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0, h,  0.0f);
	glEnd();	
	glPopMatrix();
	//glPopAttrib();
	
	return -1;
}

int CShowMenu::renderbar(float w,float h)
{
	glLoadIdentity();

	glBindTexture  (GL_TEXTURE_2D,  m_texture[3].ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//画背景矩形
	glTranslatef(0,0,0.5f);
	//glColor3f(r,g,b);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0f);

	glTexCoord2f(1.0f, 0.0f);		
	glVertex3f(w, 0.0,  0.0f);		

	//up
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(w, h,  0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0, h,  0.0f);
	glEnd();	
	glPopMatrix();
	return 1;
}