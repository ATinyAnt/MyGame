

#include "Camera.h"                    /**< 包含摄像机头文件 */





//Camera* Camera::m_pCamera = NULL;

/** 构造函数 */
Camera::Camera()
{
	/** 初始化向量值 */
	Vector3 zero = Vector3(0.0, 0.0, 2.0);		
	Vector3 view = Vector3(0.0, 1.0, 0.5);		
	Vector3 up   = Vector3(0.0, 0.0, 1.0);		
    
	/** 初始化摄像机 */
	m_Position	= zero;					
	m_View		= view;				
	m_UpVector	= up;	
	m_Speed     = 0.6f;
	
	//m_pCamera = this;
	m_IsSetCamera=false;
}


Camera::~Camera()
{
}
#define CUBECOORD 476.0
/** 设置摄像机的位置,朝向和向上向量 */
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ,
						bool unSetViewY)
{
	/** 构造向量 */
	Vector3 Position	= Vector3(positionX, positionY, positionZ);
	Vector3 View= Vector3(viewX, viewY, viewZ);

	if (unSetViewY)
	{
		Vector3 tempView = getView();
		View.y=tempView.y;
	}
	
	Vector3 UpVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	/** 设置摄像机 */
	if (scene>0)
	{
		CoordOf2D b_coord;
		CoordOf2D e_coord;
		b_coord.x=-476.0;
		b_coord.z=-476.0;
		e_coord.x= 476.0;
		e_coord.z= 476.0;
		if(!IsInCube(Position.x,Position.z,b_coord,e_coord,CAMERA))
		{
			//为0时处理
			if(Position.z==View.z)
			{
				Position.z=Position.z+0.001;
			}
			int tempangle=(int)move_angle;

			if(fabs(Position.x) > CUBECOORD)
			{
				if(Position.x<0) Position.x=- CUBECOORD;
				else Position.x=CUBECOORD;		


				if(abs(tempangle%360)>=179)
				{

					Position.z=-sqrt((m_Len*m_Len) - ((Position.x-View.x)*(Position.x-View.x)))+View.z;
					move_angle=tan((Position.x-View.x)/(Position.z-View.z))-180.0;
				}
				else
				{
					Position.z=sqrt((m_Len*m_Len) - ((Position.x-View.x)*(Position.x-View.x)))+View.z;
					move_angle=tan((Position.x-View.x)/(Position.z-View.z));
				}
			}
			else if(fabs(Position.z) > CUBECOORD)
			{
				if(Position.z<0) Position.z=- CUBECOORD;
				else Position.z=CUBECOORD;

				//这个成功了
				int temp=(tempangle%360 + 360)%360;



				if(temp >=90 && temp < 180)
				{
					Position.x=sqrt((m_Len*m_Len) - ((Position.z-View.z)*(Position.z-View.z)))+View.x;
					move_angle=tan((Position.x-View.x)/(Position.z-View.z))+90.0;
				}
				else if(temp >= 180 && temp < 270)
				{
					Position.x=-sqrt((m_Len*m_Len) - ((Position.z-View.z)*(Position.z-View.z)))+View.x;
					move_angle=tan((Position.x-View.x)/(Position.z-View.z))-90.0;
				}
				else if(temp >= 270 && temp < 360)
				{
					Position.x=-sqrt((m_Len*m_Len) - ((Position.z-View.z)*(Position.z-View.z)))+View.x;
					move_angle=tan((Position.x-View.x)/(Position.z-View.z))-90.0;
				}
				else if(temp >= 0 && temp < 90)
				{
					Position.x=sqrt((m_Len*m_Len) - ((Position.z-View.z)*(Position.z-View.z)))+View.x;
					move_angle=tan((Position.x-View.x)/(Position.z-View.z))+90.0;
				}
			}
		}
	}
	
	m_Position = Position;	
	m_View     = View;			
	m_UpVector = UpVector;
	m_IsSetCamera=true;
}

/**  旋转摄像机方向  */
// void Camera::rotateView(float angle, float x, float y, float z)
// {
// 	Vector3 newView;
// 
// 	/** 计算方向向量 */
// 	Vector3 view = m_View - m_Position;		
// 
// 	/** 计算 sin 和cos值 */
// 	float cosTheta = (float)cos(angle);
// 	float sinTheta = (float)sin(angle);
// 
// 	/** 计算旋转向量的x值 */
// 	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
// 	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
// 	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;
// 
// 	/** 计算旋转向量的y值 */
// 	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
// 	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
// 	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;
// 
// 	/** 计算旋转向量的z值 */
// 	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
// 	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
// 	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;
// 
// 	/** 更新摄像机的方向 */
// 	m_View = m_Position + newView;
// }

/** 用鼠标旋转摄像机 */
// void Camera::setViewByMouse()
// {
// 	POINT mousePos;									  /**< 保存当前鼠标位置 */
// 	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< 得到屏幕宽度的一半 */
// 	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< 得到屏幕高度的一半 */
// 	float angleY = 0.0f;							  /**< 摄像机左右旋转角度 */
// 	float angleZ = 0.0f;		                      /**< 摄像机上下旋转角度 */					
// 	static float currentRotX = 0.0f;
// 	
// 	/** 得到当前鼠标位置 */
// 	//Sleep(10);
// 	GetCursorPos(&mousePos);						
// 	ShowCursor(TRUE);
// 	
// 	/** 如果鼠标没有移动,则不用更新 */
// 	if( (mousePos.x == middleX) && (mousePos.y == middleY) )
// 		return;
// 
// 	/** 设置鼠标位置在屏幕中心 */
// 	SetCursorPos(middleX, middleY);	
// 	
// 	/**< 得到鼠标移动方向 */
// 	angleY = (float)( (middleX - mousePos.x) ) / 1000.0f;		
// 	angleZ = (float)( (middleY - mousePos.y) ) / 1000.0f;		
// 
//     static float lastRotX = 0.0f;      /**< 用于保存旋转角度 */
//  	lastRotX = currentRotX; 
// 	
// 	/** 跟踪摄像机上下旋转角度 */
// 	currentRotX += angleZ;
//  
// 	/** 如果上下旋转弧度大于1.0,我们截取到1.0并旋转 */
// 	if(currentRotX > 1.0f)     
// 	{
// 		currentRotX = 1.0f;
// 		
// 		/** 根据保存的角度旋转方向 */
// 		if(lastRotX != 1.0f) 
// 		{
// 			/** 通过叉积找到与旋转方向垂直的向量 */
// 			Vector3 vAxis = m_View - m_Position;
// 			vAxis = vAxis.crossProduct(m_UpVector);
// 			vAxis = vAxis.normalize();
// 			
// 			///旋转
// 			rotateView( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
// 		}
// 	}
// 	/** 如果旋转弧度小于-1.0,则也截取到-1.0并旋转 */
// 	else if(currentRotX < -1.0f)
// 	{
// 		currentRotX = -1.0f;
// 				
// 		if(lastRotX != -1.0f)
// 		{
// 			
// 			/** 通过叉积找到与旋转方向垂直的向量 */
// 			Vector3 vAxis = m_View - m_Position;
// 			vAxis = vAxis.crossProduct(m_UpVector);
// 			vAxis = vAxis.normalize();
// 			
// 			///旋转
// 			rotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
// 		}
// 	}
// 	/** 否则就旋转angleZ度 */
// 	else 
// 	{	
// 		/** 找到与旋转方向垂直向量 */
// 		Vector3 vAxis = m_View - m_Position;
// 		vAxis = vAxis.crossProduct(m_UpVector);
// 		vAxis = vAxis.normalize();
// 	
// 		///旋转
// 		rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
// 	}
// 
// 	/** 总是左右旋转摄像机 */
// 	rotateView(angleY, 0, 1, 0);
// 
// }



// void Camera::yawCamera(float speed)
// {
// 	Vector3 yaw;
// 	Vector3 oldPos,oldView;
// 	Vector3 cross = m_View - m_Position;
// 	oldPos = m_Position;
// 	oldView = m_View;
// 	cross = cross.crossProduct(m_UpVector);
// 
// 	///归一化向量
// 	yaw = cross.normalize();
//  
// 	m_Position.x += yaw.x * speed;
// 	m_Position.z += yaw.z * speed;
// 
// 	m_View.x += yaw.x * speed;
// 	m_View.z += yaw.z * speed;
// 	
// 	/** 进行边界检查和限定 */
// 
// 	if (m_View.x > MAP_WIDTH ||  m_View.x+MAP_WIDTH < 0)
// 	{
// 		//MessageBox(NULL,"dfd","sdfsdf",MB_OK);
// 		m_Position.x = oldPos.x ;
// 		m_View.x = oldView.x;
// 	}
// 
// 	if (m_View.z > MAP_WIDTH ||  m_View.z+MAP_WIDTH < 0)
// 	{
// 
// 		m_Position.z = oldPos.z ;
// 	}
// 
// }

//#define PI 3.14159f



/** 左右移动摄像机 */
bool Camera::moveCamera(float speed,float angle)
{
	/** 计算方向向量 */
	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         /**< 单位化 */
	Vector3 oldPos;
	oldPos = m_Position;
	
	
	/** 更新摄像机 */
	//m_Position.x += vector.x * speed;    /**< 根据速度更新位置 */
	//m_Position.z += vector.z * speed;	
	
	m_Position.y += vector.y * speed;
	
	m_Position.x=sin(DEG2RAD(angle))*m_Len+m_View.x;
	m_Position.z=cos(DEG2RAD(angle))*m_Len+m_View.z;
	if (scene==2)
	{
		if(IsInCube(m_Position.x,m_Position.z,CAMERA))
		{
			m_Position=oldPos;
			return false;
		}
	}
	if (scene>0)
	{
		CoordOf2D b_coord;
		CoordOf2D e_coord;
		b_coord.x=-476.0;
		b_coord.z=-476.0;
		e_coord.x= 476.0;
		e_coord.z= 476.0;
		if(!IsInCube(m_Position.x,m_Position.z,b_coord,e_coord,CAMERA))
		{
			m_Position=oldPos;
			return false;
		}
		else
		{
			m_View.x += vector.x * speed;		 /**< 根据速度更新方向 */	
			m_View.y += vector.y * speed;
			m_View.z += vector.z * speed;
		}
	}
// 	if(IsInCube(m_Position.x,m_Position.z,CAMERA)==false )
// 	{
// 		m_Position=oldPos;
// 		return false;
// 	}
// 	else
// 	{
// 		m_View.x += vector.x * speed;		 /**< 根据速度更新方向 */	
// 		m_View.y += vector.y * speed;
// 		m_View.z += vector.z * speed;
// 	}
	return true;	

}

/** 设置视点 */
void Camera::setLook(int s)
{
	
	// 当设置了摄像机，才设置视口
	if (m_IsSetCamera)
	{
		scene=s;
		gluLookAt(m_Position.x, m_Position.y, m_Position.z,	
			m_View.x,	 m_View.y,     m_View.z,	
			m_UpVector.x, m_UpVector.y, m_UpVector.z);
	}
	
}


Vector3 Camera::setCameraPosition(float x,float y,float z)
{
	float len=70.0;
	move_angle=0.0f;
	setLen(len);
	return Vector3(x,y+30.0,z+len);
}