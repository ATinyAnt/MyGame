

#include "Camera.h"                    /**< ���������ͷ�ļ� */





//Camera* Camera::m_pCamera = NULL;

/** ���캯�� */
Camera::Camera()
{
	/** ��ʼ������ֵ */
	Vector3 zero = Vector3(0.0, 0.0, 2.0);		
	Vector3 view = Vector3(0.0, 1.0, 0.5);		
	Vector3 up   = Vector3(0.0, 0.0, 1.0);		
    
	/** ��ʼ������� */
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
/** �����������λ��,������������� */
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ,
						bool unSetViewY)
{
	/** �������� */
	Vector3 Position	= Vector3(positionX, positionY, positionZ);
	Vector3 View= Vector3(viewX, viewY, viewZ);

	if (unSetViewY)
	{
		Vector3 tempView = getView();
		View.y=tempView.y;
	}
	
	Vector3 UpVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	/** ��������� */
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
			//Ϊ0ʱ����
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

				//����ɹ���
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

/**  ��ת���������  */
// void Camera::rotateView(float angle, float x, float y, float z)
// {
// 	Vector3 newView;
// 
// 	/** ���㷽������ */
// 	Vector3 view = m_View - m_Position;		
// 
// 	/** ���� sin ��cosֵ */
// 	float cosTheta = (float)cos(angle);
// 	float sinTheta = (float)sin(angle);
// 
// 	/** ������ת������xֵ */
// 	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
// 	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
// 	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;
// 
// 	/** ������ת������yֵ */
// 	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
// 	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
// 	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;
// 
// 	/** ������ת������zֵ */
// 	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
// 	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
// 	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;
// 
// 	/** ����������ķ��� */
// 	m_View = m_Position + newView;
// }

/** �������ת����� */
// void Camera::setViewByMouse()
// {
// 	POINT mousePos;									  /**< ���浱ǰ���λ�� */
// 	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< �õ���Ļ��ȵ�һ�� */
// 	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< �õ���Ļ�߶ȵ�һ�� */
// 	float angleY = 0.0f;							  /**< �����������ת�Ƕ� */
// 	float angleZ = 0.0f;		                      /**< �����������ת�Ƕ� */					
// 	static float currentRotX = 0.0f;
// 	
// 	/** �õ���ǰ���λ�� */
// 	//Sleep(10);
// 	GetCursorPos(&mousePos);						
// 	ShowCursor(TRUE);
// 	
// 	/** ������û���ƶ�,���ø��� */
// 	if( (mousePos.x == middleX) && (mousePos.y == middleY) )
// 		return;
// 
// 	/** �������λ������Ļ���� */
// 	SetCursorPos(middleX, middleY);	
// 	
// 	/**< �õ�����ƶ����� */
// 	angleY = (float)( (middleX - mousePos.x) ) / 1000.0f;		
// 	angleZ = (float)( (middleY - mousePos.y) ) / 1000.0f;		
// 
//     static float lastRotX = 0.0f;      /**< ���ڱ�����ת�Ƕ� */
//  	lastRotX = currentRotX; 
// 	
// 	/** ���������������ת�Ƕ� */
// 	currentRotX += angleZ;
//  
// 	/** ���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת */
// 	if(currentRotX > 1.0f)     
// 	{
// 		currentRotX = 1.0f;
// 		
// 		/** ���ݱ���ĽǶ���ת���� */
// 		if(lastRotX != 1.0f) 
// 		{
// 			/** ͨ������ҵ�����ת����ֱ������ */
// 			Vector3 vAxis = m_View - m_Position;
// 			vAxis = vAxis.crossProduct(m_UpVector);
// 			vAxis = vAxis.normalize();
// 			
// 			///��ת
// 			rotateView( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
// 		}
// 	}
// 	/** �����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת */
// 	else if(currentRotX < -1.0f)
// 	{
// 		currentRotX = -1.0f;
// 				
// 		if(lastRotX != -1.0f)
// 		{
// 			
// 			/** ͨ������ҵ�����ת����ֱ������ */
// 			Vector3 vAxis = m_View - m_Position;
// 			vAxis = vAxis.crossProduct(m_UpVector);
// 			vAxis = vAxis.normalize();
// 			
// 			///��ת
// 			rotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
// 		}
// 	}
// 	/** �������תangleZ�� */
// 	else 
// 	{	
// 		/** �ҵ�����ת����ֱ���� */
// 		Vector3 vAxis = m_View - m_Position;
// 		vAxis = vAxis.crossProduct(m_UpVector);
// 		vAxis = vAxis.normalize();
// 	
// 		///��ת
// 		rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
// 	}
// 
// 	/** ����������ת����� */
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
// 	///��һ������
// 	yaw = cross.normalize();
//  
// 	m_Position.x += yaw.x * speed;
// 	m_Position.z += yaw.z * speed;
// 
// 	m_View.x += yaw.x * speed;
// 	m_View.z += yaw.z * speed;
// 	
// 	/** ���б߽�����޶� */
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



/** �����ƶ������ */
bool Camera::moveCamera(float speed,float angle)
{
	/** ���㷽������ */
	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         /**< ��λ�� */
	Vector3 oldPos;
	oldPos = m_Position;
	
	
	/** ��������� */
	//m_Position.x += vector.x * speed;    /**< �����ٶȸ���λ�� */
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
			m_View.x += vector.x * speed;		 /**< �����ٶȸ��·��� */	
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
// 		m_View.x += vector.x * speed;		 /**< �����ٶȸ��·��� */	
// 		m_View.y += vector.y * speed;
// 		m_View.z += vector.z * speed;
// 	}
	return true;	

}

/** �����ӵ� */
void Camera::setLook(int s)
{
	
	// ����������������������ӿ�
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