

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "stdafx.h"
#include "Vector.h"                 /**< 包含向量类头文件 */
#include "GLFrame.h"
#include "GlobalFunc.h"
#include "Log.h"
/** 摄像机类 */
class Camera
{
public:
	
	/** 构造函数和析构函数 */
	Camera();
	~Camera();
	
	/** 获得摄像机状态方法 */
	Vector3 getPosition()   {	return m_Position;		}
	Vector3 getView()	    {	return m_View;			}
	Vector3 getUpVector()   {	return m_UpVector;		}
	float   getSpeed()      {   return m_Speed;         }

     
	/** 设置摄像机的位置, 观察点和向上向量 */
	void setCamera(float positionX, float positionY, float positionZ,
			 	   float viewX,     float viewY,     float viewZ,
				   float upVectorX, float upVectorY, float upVectorZ,
				   bool unSetViewY=true);

	
    /** 左右摄像机移动 */
	bool moveCamera(float speed,float angle);
	
	/** 放置摄像机 */
	void setLook(int s);

	void setScene(int s){scene=s;};

	//设置摄像机与英雄的相对距离
	Vector3 setCameraPosition(float x,float y,float z);

	//设置水平距离
	void setLen(float len){m_Len=len;}

	float getLen(){return m_Len;};

	float          move_angle;		//摄像机的转向角度
private:
	/** 摄像机属性 */
	Vector3        m_Position;      /**< 位置 */
	Vector3        m_View;          /**< 朝向 */
	Vector3        m_UpVector;      /**< 向上向量 */
	float          m_Speed;         /**< 速度 */
	float          m_Len;			//摄像机与英雄的水平距离
	bool		   m_IsSetCamera;
	int			   scene;
	
};

#endif //__CAMERA_H__