//#include "stdafx.h"

#include "Hero.h"
#include "luadata.h"

extern Camera m_Camera;

CHero::CHero()
{
//	m_angle=0;
	showlaser_step=LASERSTEPLEN;
	onlaser=false;
	speed=0.15f;
	movedistance=1;
	isRush=false;
	rushspeed=0.9f;
	isJumpBit=false;
	bitspeed=0.9f;
	showlaser_step=8;
	HP=1;
	isLightning=false;
	showlightning=false;
	BeHit=false;
	m_Color.x=1.0;
	m_Color.y=1.0;
	m_Color.z=1.0;
	showlifetime.SetTimenow();
}


CHero::~CHero()
{
	//delete
}


void CHero::Init(bool gameType)
{
	isConnectGame=gameType;
	LuaData lua;
	lua.InitLuaEnv();
	//lua.SetLuaFile("Lua\\hero.lua");
	if (!lua.LoadLuaFile("hero.lua"))
	{
		MessageBox(NULL,"加载数据文件出错","CHero 出错",MB_OK);
		exit(0);
	}
// 
	lua.getGlobalProc("AllHeroData");
	lua_pcall(lua.GetLuaEnv(), 0, 13, 0);

	//Vector3 temp;
	float x,y,z;
	z=(float)lua_tonumber(lua.GetLuaEnv(),-1);
	y=(float)lua_tonumber(lua.GetLuaEnv(),-2);
	x=(float)lua_tonumber(lua.GetLuaEnv(),-3);
	ShootRange=lua_tonumber(lua.GetLuaEnv(),-4);
	defense=lua_tonumber(lua.GetLuaEnv(),-5);
	FullHP=lua_tonumber(lua.GetLuaEnv(),-6);
 	HP=lua_tonumber(lua.GetLuaEnv(),-7);
 	m_angle=lua_tonumber(lua.GetLuaEnv(),-8);
	char *md2file=(char *)lua_tostring(lua.GetLuaEnv(),-9);
	char *txtfile=(char *)lua_tostring(lua.GetLuaEnv(),-10);
	char *wpmd2file=(char *)lua_tostring(lua.GetLuaEnv(),-11);
	char *wptxtfile=(char *)lua_tostring(lua.GetLuaEnv(),-12);
	speed=lua_tonumber(lua.GetLuaEnv(),-13);
	curspeed=speed;
	m_RoleCoord=Vector3(x,y,z);
	m_RoleDesCoord=Vector3(x,y,z);

	md2.Load(md2file, txtfile);
	md2.SetState(MODEL_IDLE);
	m_ball.LoadTexture("..\\data\\bomb.bmp");
	m_lightning.LoadTexture("..\\data\\lightning.bmp");

}


void CHero::setHeroID(int id)
{
	heroID=id;
}

void CHero::setHeroCoord(float x , float z)
{
	m_RoleCoord.x=x;
	m_RoleCoord.z=z;

	m_RoleDesCoord.x=x;
	m_RoleDesCoord.z=z;
}

void CHero::Animate(int scene,int id)
{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	
	glTranslatef(m_RoleCoord.x,7.0,m_RoleCoord.z);
	glScalef(0.3f,0.3f,0.3f);
	//glScalef(0.2,0.2,0.2);
	//Opengl是右手坐标系，不过这里坐标系是，上：Z 右：X 前：Y
	glRotatef(-90.0, 1.0f, 0.0, 0.0);	//两个是调整坐标
	glRotatef(90,0.0f,0.0f,1.0f);
	glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
	int *frame=md2.GetFrame(md2.GetState());
	glColor3f(m_Color.x,m_Color.y,m_Color.z);
	//glPushAttrib(GL_CURRENT_BIT);
	md2.Animate(frame[0],frame[1],curspeed);
	//glPopAttrib();
	glPopMatrix();
	glPopAttrib();
	//IsInCube(m_RoleDesCoord.x,m_RoleDesCoord.z,HERO);
	if (onlaser)
	{
		if (showlaser_step < ShootRange)
		{
			ShowLaser(m_RoleCoord.x,m_RoleCoord.z);
		}
		else
		{
			onlaser=false;
			showlaser_step=8;
		}
	}
	CoordOf2D b_coord;
	CoordOf2D e_coord;
	if (scene==0)
	{	
		b_coord.x=-100.0+5.0;
		b_coord.z=580.0+5.0;

		e_coord.x=100.0-5.0;
		e_coord.z=980.0-5.0;
		IsInCube(m_RoleDesCoord.x,m_RoleDesCoord.z,b_coord,e_coord,HERO);
	}
	else
	{
		b_coord.x=-476.0;
		b_coord.z=-476.0;

		e_coord.x=476.0;
		e_coord.z=476.0;
		IsInCube(m_RoleDesCoord.x,m_RoleDesCoord.z,b_coord,e_coord,HERO);
		if (scene==2)		//当第二个场景才做处理
		{
			IsInCube(m_RoleDesCoord.x,m_RoleDesCoord.z,HERO);
		}
	}
	
	if(isJumpBit)
	{
		
		JumpBit(m_RoleCoord.x,m_RoleCoord.z,m_RoleDesCoord.x,m_RoleDesCoord.z,id);
		//MessageBox(NULL,"fsdf","fds",MB_OK);
	}
	else
	{
		Run(m_RoleCoord.x,m_RoleCoord.z,m_RoleDesCoord.x,m_RoleDesCoord.z,id);
	}
	
}


bool CHero::IsDead()
{
	if (HP<=0)
	{
		return true;
	}
	return false;
}


void CHero::OpenFire()
{
	//设置开火的动作和激光射击过程为真
	if(lasertime.ReachTime(500))
	{
		lasertime.SetTimenow();
		m_RoleDesCoord.x=m_RoleCoord.x;
		m_RoleDesCoord.z=m_RoleCoord.z;
		//onfire=true;
		onlaser=true;
		showlaser_step=LASERSTEPLEN;
		md2.SetState(MODEL_ATTACK);
	}
	
	
	return;
}


void CHero::ShowLaser(float x,float z)
{
	float k,des_x,des_z;
	float angle=m_angle;

	if (angle==0.0 || angle==180.0)
	{
		angle+=0.001f;
	}
	//因为当m_angle为0或者180时tan为无限在，所以分出来做特殊处理
	if (angle!=0.0 && angle!=180.0)
	{
		if (angle<=180.0)
		{
			k=tan((90.0-angle)*PI/180.0);
			des_x=-sqrt((showlaser_step*showlaser_step)/(k*k+1))+x;
			des_z=k*(des_x-x)+z;
		}
		else
		{
			k=-tan((90+angle)*PI/180.0);
			des_x=sqrt((showlaser_step*showlaser_step)/(k*k+1))+x;
			des_z=k*(des_x-x)+z;
		}

	}
	else if(angle == 0.0)
	{
		des_x=x;
		des_z=z-showlaser_step;
	}
	else
	{
		des_x=x;
		des_z=z+showlaser_step;
	}

	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(des_x,10.0,des_z);

	//auxSolidSphere(1.5);
	//glEnable(GL_LIGHTING);
	glPopMatrix();
	glPopAttrib();

	showlaser_step += LASERSTEPLEN;
	if (isConnectGame)
	{
		if(IsHitHero(x,z,des_x,des_z,HITRANGE,25,getHeroID()))	//当激光炮已经发射完后，检测是否击中怪物
		{
			showlaser_step += ShootRange;		//击中后不再显示后面的动画
		}	
	}
	else if(IsHitMonster(x,z,des_x,des_z))	//当激光炮已经发射完后，检测是否击中怪物
	{
		showlaser_step += ShootRange;		//击中后不再显示后面的动画
	}	
}



float CHero::getAngle(int x,int z)
{
	//Vector3 point=TwoDtoThreeD(x,y);
	float tempX,tempZ;
	tempX=x-m_RoleCoord.x;
	tempZ=z-m_RoleCoord.z;
	float angle;
	if (tempX < 0)
	{
		if (tempZ < 0)
		{
			angle=90.0-atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
		}
		else
		{
			angle=90.0+atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
		}
	}
	else
	{
		if (tempZ < 0)
		{
			angle=270.0+atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
		}
		else
		{
			angle=270.0-atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
		}
	}
	return angle;
}




void CHero::ShowLife(float x, float z,int id)
{
 	if(id == OTHER && !BeHit)
 	{
 		return;
 	}
// 	if(id == OTHER && BeHit)
// 	{
// 		showlifetime.SetTimenow();
// 		
// 	}
	else if(id==OTHER  && BeHit && showlifetime.TimeInterval(1200))
	{
		BeHit=false;
		return;
	}
	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
	};
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(x, 0.40f, z);
	glColor3f (0.0f, 0.0f, 0.0f);
	glRasterPos3f (0.0f, 0.0f,-0.1f);
	for (int i=0;i<MAXHP;i++)
	{

		glBitmap (8, 12, 0.0, 0.0, 8.0, 0.0, rasterallf);
	}
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(x, 0.40f, z);
	glColor3f (0.0, 1.0, 0.0);
	glRasterPos3f (0.0f, 0.0f,-0.1f);
	int temp=HP*MAXHP/FullHP;
	for (int i=0;i<temp;i++)
	{
		glBitmap (8, 11, 0.0, 0.0, 8.0, 0.0, rasterallf);
	}

	glPopMatrix();
	glPopAttrib();
}


void CHero::Rush()
{
	if(!rushtime.ReachTime(1000))
	{
		return;
	}
	rushtime.SetTimenow();
	//一步的距离
	SetDistance(5.0);
	//一个Rush的距离
	float rushdistance=70.0;
	float angle=getAngle();
	float k=0.0;
	isRush=true;
	SetSpeed(rushspeed);
	CoordOf2D temp=GetCoordByAngle(m_RoleCoord.x,m_RoleCoord.z,angle,rushdistance);
	m_RoleDesCoord.x=temp.x;
	m_RoleDesCoord.z=temp.z;

}



bool CHero::Run(float &curX,float &curZ,float desX,float desZ,int id)
{
	//if(curX != desX && curZ != desZ)
	if(!(curX == desX && curZ == desZ))
	{
		if (isRush==false)
		{
			SetDistance(1.0);			

		}
		md2.SetState(MODEL_RUN);
		float x=0.0,z=0.0;
		float distance=movedistance;
		CoordOf2D coord;
		Vector3 tempPosition=m_Camera.getPosition();
		coord=GetDistCoord(curX,curZ,desX,desZ,distance);
		x=coord.x;
		z=coord.z;

		if (curX < desX && desX < curX+x)
		{
			curX=desX;
			tempPosition.x+=desX-curX;
		}
		else if (curX > desX && desX > curX+x)
		{
			curX=desX;
			tempPosition.x+=desX-curX;
		}
		else
		{
			curX+=x;
			tempPosition.x+=x;
		}

		if (curZ < desZ && desZ < curZ+z)
		{
			curZ=desZ;
			tempPosition.z+=desZ-curZ;
		}
		else if (curZ > desZ && desZ > curZ+z)
		{
			curZ=desZ;
			tempPosition.z+=desZ-curZ;
		}
		else
		{
			curZ+=z;
			tempPosition.z+=z;
		}
		if (id==OWN)
		{
			m_Camera.setCamera(tempPosition.x,tempPosition.y,tempPosition.z,
			m_RoleCoord.x,m_RoleCoord.y ,m_RoleCoord.z,
			0,1,0);
		}
		
		return true;
	}
	else
	{
		md2.SetState(MODEL_IDLE);
		isRush=false;
		SetSpeed(speed);
		return false;
	}

}


CoordOf2D* CHero::Lightning(float x,float z,bool isConnectGame,bool isLButtonDown,int id)
{
	static float lx=0.0;
	static float lz=0.0;
	static CoordOf2D *temp=new CoordOf2D;
	temp->x=-999;
	temp->z=-999;
	if(showlightning)
	{
		
		if (lightningtime.TimeInterval(300))
		{
			showlightning=false;
			if (!isConnectGame)
			{
				IsHitMonster(lx,lz,lx,lz,15,50);
			}
			else
			{
				IsHitHero(x,z,x,z,15,50);
			}
			//sendpro
			
		}
		if (id==OWN)
		{
			//显示闪电
			m_lightning.Render(30,lx,-1.0,lz,m_Camera.move_angle);
		}
		else
		{
			m_lightning.Render(30,x,-1.0,z,m_Camera.move_angle);
		}

	}
	else if (isLightning && isLButtonDown==false && showlightning==false)
	{
		if ((m_RoleCoord.x-x)*(m_RoleCoord.x-x)+(m_RoleCoord.z-z)*(m_RoleCoord.z-z)<=80.0*80.0)
		{
			md2.SetState(MODEL_SAULTE);
			float xx,yy;
			glPushAttrib(GL_CURRENT_BIT);
			glPushMatrix();
			//glTranslatef(x,5.0,z);
			glColor4f(0.0f,1.0f,0.0f,0.5f);
			glPointSize(8.0f);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBegin(GL_POINTS);
			for(float angle=0.0f;angle<2*PI;angle+=0.01f)
			{
				xx=15.0f*cos(angle);
				yy=15.0f*sin(angle);
				glVertex2f(x+xx,z+yy);
			}

			glEnd();
			glPopMatrix();
			glPopAttrib();
		}
		m_RoleDesCoord.x=m_RoleCoord.x;
		m_RoleDesCoord.z=m_RoleCoord.z;
	}
	else if (isLightning  && isLButtonDown && ((m_RoleCoord.x-x)*(m_RoleCoord.x-x)+(m_RoleCoord.z-z)*(m_RoleCoord.z-z)<=80.0*80.0))
	{
		isLightning=false;
		showlightning=true;
		::ShowCursor(true);
		lx=x;
		lz=z;
		temp->x=lx;
		temp->z=lz;
		lightningtime.SetTimenow();
	}
	return temp;
}


void CHero::JumpBit()
{
	if(!jumpbittime.ReachTime(3000))
	{
		return;
	}
	jumpbittime.SetTimenow();
	isJumpBit=true;
	int dist=25;		//攻击距离

	md2.SetState(MODEL_JUMP);
	float angle=getAngle();
	float k=0.0;

	CoordOf2D temp=GetCoordByAngle(m_RoleCoord.x,m_RoleCoord.z,angle,dist);
	m_RoleDesCoord.x=temp.x;
	m_RoleDesCoord.z=temp.z;

}


//跳击
void CHero::JumpBit(float &curX,float &curZ,float desX,float desZ,int id)
{
	if ((curX==desX) && (curZ==desZ))
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		isJumpBit=false;
		//SetSpeed(speed);
	}
	else
	{
		float dist=(curX-desX)*(curX-desX);
		dist+=(curZ-desZ)*(curZ-desZ);
		//当离目标距离的平方还差小于9时，显示跳的这个动作
		if (dist<=9)
		{
			SetSpeed(bitspeed);
			md2.SetState(MODEL_POINT);
			float k,x=0.0,z=0.0;
			float distance=0.1f;
			//小于2显示爆炸球
			if (dist<=2)
			{
				
				float bx;
				float bz;
				//只有第一进来的时候计算距离

				dist=12.0;
				float angle=getAngle();

				if (angle==0.0 || angle==180.0)
				{
					angle+=0.001f;
				}
				if (angle!=0.0 && angle!=180.0)
				{
					if (angle<=180.0)
					{
						k=tan((90.0-angle)*PI/180.0);
						bx=-sqrt((dist*dist)/(k*k+1))+m_RoleCoord.x;
						bz=k*(bx-m_RoleCoord.x)+m_RoleCoord.z;
					}
					else
					{
						k=-tan((90+angle)*PI/180.0);
						bx=sqrt((dist*dist)/(k*k+1))+m_RoleCoord.x;
						bz=k*(bx-m_RoleCoord.x)+m_RoleCoord.z;
					}

				}
				m_ball.Render(12,bx,8.0,bz,m_Camera.move_angle);
				
			}
			Vector3 tempPosition=m_Camera.getPosition();
			CoordOf2D coord=GetDistCoord(curX,curZ,desX,desZ,distance);
			x=coord.x;
			z=coord.z;

			if (curX < desX && desX < curX+x)
			{
				curX=desX;
				tempPosition.x+=desX-curX;
			}
			else if (curX > desX && desX > curX+x)
			{
				curX=desX;
				tempPosition.x+=desX-curX;
			}
			else
			{
				curX+=x;
				tempPosition.x+=x;
			}

			if (curZ < desZ && desZ < curZ+z)
			{
				curZ=desZ;
				tempPosition.z+=desZ-curZ;
			}
			else if (curZ > desZ && desZ > curZ+z)
			{
				curZ=desZ;
				tempPosition.z+=desZ-curZ;
			}
			else
			{
				curZ+=z;
				tempPosition.z+=z;
			}
			if (id==OWN)
			{
				m_Camera.setCamera(tempPosition.x,tempPosition.y,tempPosition.z,
				m_RoleCoord.x,m_RoleCoord.y ,m_RoleCoord.z,
				0,1,0);
			}
			if (isConnectGame)
			{
				IsHitHero(curX,curZ,desX,desZ,5.0,7,heroID);
			}
			else
			{
				IsHitMonster(curX,curZ,desX,desZ,5.0);
				
			}
			IsInJumpBitRange(isConnectGame,curX,curZ,m_angle,5.0,id);
			
		}
		else
		{
			float x,z;
			float distance=6;
			CoordOf2D coord;
			Vector3 tempPosition=m_Camera.getPosition();
			coord=GetDistCoord(curX,curZ,desX,desZ,distance);
			x=coord.x;
			z=coord.z;

			if (curX < desX && desX < curX+x)
			{
				curX=desX;
				tempPosition.x+=desX-curX;
			}
			else if (curX > desX && desX > curX+x)
			{
				curX=desX;
				tempPosition.x+=desX-curX;
			}
			else
			{
				curX+=x;
				tempPosition.x+=x;
			}

			if (curZ < desZ && desZ < curZ+z)
			{
				curZ=desZ;
				tempPosition.z+=desZ-curZ;
			}
			else if (curZ > desZ && desZ > curZ+z)
			{
				curZ=desZ;
				tempPosition.z+=desZ-curZ;
			}
			else
			{
				curZ+=z;
				tempPosition.z+=z;
			}
			if (id==OWN)
			{
				m_Camera.setCamera(tempPosition.x,tempPosition.y,tempPosition.z,
					m_RoleCoord.x,m_RoleCoord.y ,m_RoleCoord.z,
					0,1,0);
			}
			IsInJumpBitRange(isConnectGame,curX,curZ,m_angle,5.0,id);
			
		}
	}
}


void CHero::Transmit(int &scene,int CurrentScene)
{
	if(!transmittime.ReachTime(5000))
	{
		return ;
	}
	transmittime.SetTimenow();
	if (scene>0)
	{
		scene=0;
		m_Camera.setScene(scene);
		m_RoleCoord.x=0.0;
		m_RoleCoord.z=830.0;
		m_RoleDesCoord.x=0.0;
		m_RoleDesCoord.z=830.0;
		Vector3 m_CameraPosition=m_Camera.setCameraPosition(m_RoleCoord.x, m_RoleCoord.y, m_RoleCoord.z);
		/** 设置摄像机 */
		m_Camera.setCamera( m_CameraPosition.x,m_CameraPosition.y, m_CameraPosition.z,  m_RoleCoord.x, m_RoleCoord.y, m_RoleCoord.z,  0, 1, 0,false);
	}
	else
	{
		scene=CurrentScene;
		m_Camera.setScene(scene);
		m_RoleCoord.x=0.0;
		m_RoleCoord.z=300.0;
		m_RoleDesCoord.x=0.0;
		m_RoleDesCoord.z=300.0;
		Vector3 m_CameraPosition=m_Camera.setCameraPosition(m_RoleCoord.x, m_RoleCoord.y, m_RoleCoord.z);
		/** 设置摄像机 */
		m_Camera.setCamera( m_CameraPosition.x,m_CameraPosition.y, m_CameraPosition.z,m_RoleCoord.x, m_RoleCoord.y, m_RoleCoord.z,  0, 1, 0,false);
	}
	//return scene;
}