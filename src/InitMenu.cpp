#include "InitMenu.h"


CInitMenu::CInitMenu()
{
	
}

CInitMenu::~CInitMenu()
{
	for(int i=0; i<TXNUM; i++)
	{
		m_texture[i].FreeImage();
		glDeleteTextures(1,&m_texture[i].ID);
	}
}



bool CInitMenu::loadTexture()
{
	if(!font.InitFont())
		return false;
	
	char* fileName[] = {
		"../data/initbackground.BMP",
		"../data/bt_be.bmp",
		"../data/bt_co.bmp",
		"../data/bt_ex.bmp"
	};

	for(int i=0; i < TXNUM; i++)
	{
		if(!m_texture[i].LoadBitmap(fileName[i]) )                    /**< ����λͼ�ļ� */
		{
			MessageBox(NULL,"װ��λͼ�ļ�ʧ�ܣ�","CInitMenu ����",MB_OK);       /**< �������ʧ���򵯳��Ի��� */
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

#define BUTTONNUM 3

int CInitMenu::render(int x,int y,int w,int h,int mx,int my,bool LButtonDown,int type)
{

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture  (GL_TEXTURE_2D,  m_texture[0].ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glPushMatrix();

	//����������
	glTranslatef(x,y,-0.5f);
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

	
	
	
	//windows����Ļ������opengl�Ĳ�ͬ���������glTranslatef(500,500,-0.5f);),windowsԭ��Ϊ���Ͻǣ�openglΪ���½�
	float z=-0.5;
	int tx_x,tx_y;
	
	//int tx_rt=500,ty_rt=500;
	int tx_rt,ty_rt;
	int bt_w=160,bt_h=60;
	tx_rt=(w-bt_w)>>1;
	ty_rt=(h>>1)+(BUTTONNUM/2 * 100);
	for(int i=0;i<BUTTONNUM;i++)
	{
		tx_x=tx_rt; 
		tx_y=ty_rt-(100*i);				//ÿ100���ػ�һ����ť
		//if ((tx_x<mx) && (mx<(tx_x+bt_w)) && ((240+(i*100))<my) && (my<(300+(i*100))))
		if ((tx_x<mx) && (mx<(tx_x+bt_w)) && ((h-tx_y-bt_h)<my) && (my<(h-tx_y)))
		{
			SetCursor(LoadCursor(NULL,IDC_HAND));
			tx_x+=10;
			tx_y+=10;
			if (LButtonDown)
			{
				return i;
			}
		}
		glPushMatrix();
		glLoadIdentity();
		glBindTexture  (GL_TEXTURE_2D,  m_texture[i+1].ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTranslatef(tx_x,tx_y,z);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0f);

		glTexCoord2f(1.0f, 0.0f);		
		glVertex3f(bt_w, 0.0,  0.0f);		

		//up
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(bt_w, bt_h,  0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, bt_h,  0.0f);
		glEnd();	
		glPopMatrix();
	}
	
	//��ʼ��Ϸ

	return -1;
}