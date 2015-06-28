#include "Bombball.h"


bool CBombBall::LoadTexture(char* fileName)
{
	//char* fileName="data\\bomb.bmp";
	//	MessageBox(NULL,fileName[1],fileName[1],MB_OK);
	if(!m_texture.LoadBitmap(fileName) )                    /**< ����λͼ�ļ� */
	{
		MessageBox(NULL,"װ��λͼ�ļ�ʧ�ܣ�","CBombBall ����",MB_OK);       /**< �������ʧ���򵯳��Ի��� */
		exit(0);
	}
	glGenTextures(1, &m_texture.ID);                            /**< ����һ������������� */


	glBindTexture(GL_TEXTURE_2D, m_texture.ID);                 /**< ����������� */

	/** �����˲� */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	/** �������� */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_texture.imageWidth,
		m_texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		m_texture.image);

	return true;
}


void CBombBall::Render(float size,float x,float y,float z ,float angle)
{
// 	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
// 	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
// 	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
// 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//float w=300,h=200;
	//glPushAttrib(GL_CURRENT_BIT);
	//glPushMatrix();
	glTranslatef(x,y,z);
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);    //ȥ���������ʾ����
	//glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
 	glEnable(GL_ALPHA_TEST);
 	glAlphaFunc(GL_GREATER,0.0f);
	glBindTexture(GL_TEXTURE_2D,m_texture.ID);
	glScalef(size,size,size);
	//auxSolidSphere(5.0);
	
	glBegin(GL_QUADS);

	float m_x1,m_x2;
	float m_z1,m_z2;

	angle=angle+90.0;
	m_x1=sin(DEG2RAD(angle))*0.5;
	m_z1=cos(DEG2RAD(angle))*0.5;
	angle=angle+180.0;
	m_x2=sin(DEG2RAD(angle))*0.5;
	m_z2=cos(DEG2RAD(angle))*0.5;

	glTexCoord2f(1.0f, 1.0f); glVertex3f( m_x2, 0.0f,  m_z2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( m_x1, 0.0f,  m_z1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( m_x1, 1.0f,  m_z1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( m_x2, 1.0f,  m_z2);



	glEnd();
	
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	//glDisable(GL_TEXTURE_2D);
//	
	
	
	//glPopMatrix();
	//glPopAttrib();
}


void CBombBall::ShowBitBomb()
{

}


