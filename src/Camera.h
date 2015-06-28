

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "stdafx.h"
#include "Vector.h"                 /**< ����������ͷ�ļ� */
#include "GLFrame.h"
#include "GlobalFunc.h"
#include "Log.h"
/** ������� */
class Camera
{
public:
	
	/** ���캯������������ */
	Camera();
	~Camera();
	
	/** ��������״̬���� */
	Vector3 getPosition()   {	return m_Position;		}
	Vector3 getView()	    {	return m_View;			}
	Vector3 getUpVector()   {	return m_UpVector;		}
	float   getSpeed()      {   return m_Speed;         }

     
	/** �����������λ��, �۲����������� */
	void setCamera(float positionX, float positionY, float positionZ,
			 	   float viewX,     float viewY,     float viewZ,
				   float upVectorX, float upVectorY, float upVectorZ,
				   bool unSetViewY=true);

	
    /** ����������ƶ� */
	bool moveCamera(float speed,float angle);
	
	/** ��������� */
	void setLook(int s);

	void setScene(int s){scene=s;};

	//�����������Ӣ�۵���Ծ���
	Vector3 setCameraPosition(float x,float y,float z);

	//����ˮƽ����
	void setLen(float len){m_Len=len;}

	float getLen(){return m_Len;};

	float          move_angle;		//�������ת��Ƕ�
private:
	/** ��������� */
	Vector3        m_Position;      /**< λ�� */
	Vector3        m_View;          /**< ���� */
	Vector3        m_UpVector;      /**< �������� */
	float          m_Speed;         /**< �ٶ� */
	float          m_Len;			//�������Ӣ�۵�ˮƽ����
	bool		   m_IsSetCamera;
	int			   scene;
	
};

#endif //__CAMERA_H__