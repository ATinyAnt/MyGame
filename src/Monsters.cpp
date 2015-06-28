
#include "Monsters.h"
#include "luadata.h"

CHero *myHero;
CMonsters::CMonsters()
{

	m_MonsterNum=2;
	m_MonsterRange=2;
	showlaser_step=2;


}

CMonsters::~CMonsters()
{

}


void CMonsters::Init(int s)
{
	LuaData lua;
	lua.InitLuaEnv();
	//lua.SetLuaFile("Lua\\monster.lua");
	if (!lua.LoadLuaFile("monster.lua"))
	{
		MessageBox(NULL,"加载数据文件出错","出错",MB_OK);
		exit(0);
	}
	if(s == 1)
	{
		lua.getGlobalProc("BaseMonster1Data");
	}
	else
	{
		lua.getGlobalProc("BaseMonster2Data");
	}
	
	lua_pcall(lua.GetLuaEnv(), 0, 17, 0);
	int Hasweapon=lua_tonumber(lua.GetLuaEnv(),-1);
	m_MonsterNum=lua_tonumber(lua.GetLuaEnv(),-2);
	m_MonsterRange=m_MonsterNum;
	m_LoadMonster=m_MonsterNum;
	int Skillcooltime=lua_tonumber(lua.GetLuaEnv(),-3);
	float Speed=lua_tonumber(lua.GetLuaEnv(),-4);
	float Scale=lua_tonumber(lua.GetLuaEnv(),-5);
	int Shootrange=lua_tonumber(lua.GetLuaEnv(),-6);
	int Chaserange=lua_tonumber(lua.GetLuaEnv(),-7);
	float Y=lua_tonumber(lua.GetLuaEnv(),-8);
	int Defense=lua_tonumber(lua.GetLuaEnv(),-9);
	float Angle=lua_tonumber(lua.GetLuaEnv(),-10);
	int Fullhp=lua_tonumber(lua.GetLuaEnv(),-11);
	int Hp=lua_tonumber(lua.GetLuaEnv(),-12);
	char *Wpmd2file=(char *)lua_tostring(lua.GetLuaEnv(),-13);
	char *Wptextture=(char *)lua_tostring(lua.GetLuaEnv(),-14);
	char *Md2file=(char *)lua_tostring(lua.GetLuaEnv(),-15);
	char *Textture=(char *)lua_tostring(lua.GetLuaEnv(),-16);
	float  movedistance=lua_tonumber(lua.GetLuaEnv(),-17);
	srand(time(0));
	float x,z;
	for (int i=0;i<m_MonsterNum;i++)
	{
		x=(float)(4700-(rand()%9400))/10.0f;
		z=(float)(4700-(rand()%9400))/10.0f;
		Monsters[i].BeHit=false;
		Monsters[i].HP=Hp;
		Monsters[i].m_FullHP=Fullhp;
		Monsters[i].IsDead=false;
		Monsters[i].m_angle=Angle;
		Monsters[i].m_MonsterCoord.x=x;
		Monsters[i].m_MonsterCoord.y=Y;
		Monsters[i].m_MonsterCoord.z=z;
		Monsters[i].m_MonsterDesCoord.x=Monsters[i].m_MonsterCoord.x;
		Monsters[i].m_MonsterDesCoord.y=Y;
		Monsters[i].m_MonsterDesCoord.z=Monsters[i].m_MonsterCoord.z;
		Monsters[i].ChaseRange=Chaserange;
		Monsters[i].ShootRange=Shootrange;
		Monsters[i].scale=Scale;
		Monsters[i].speed=Speed;
		Monsters[i].AttackDone=true;
		Monsters[i].IsShoot=false;
		Monsters[i].showlaser_step=8;
		Monsters[i].IsRun=true;
		Monsters[i].status=MODEL_IDLE;
		Monsters[i].HasWeapon=Hasweapon > 0 ? true : false;
		Monsters[i].moveDistance=movedistance;

		Monsters[i].SkillCoolTime=Skillcooltime;
		Monsters[i].md2.Load(Md2file, Textture);
		//Monsters[i].md2.Load("ogro\\rhino.md2", "ogro\\rhino.pcx");
		if (Monsters[i].HasWeapon)
		{
			Monsters[i].m_weapon.Load(Wpmd2file, Wptextture);
		}

	}

}

void CMonsters::ReInit()
{
	float x,z;
	m_MonsterNum=m_LoadMonster;
	m_MonsterRange=m_LoadMonster;
	srand(time(0));
	for (int i=0;i<m_MonsterNum;i++)
	{
		x=(float)(4700-(rand()%9400))/10.0f;
		z=(float)(4700-(rand()%9400))/10.0f;
		Monsters[i].m_MonsterCoord.x=x;
		
		Monsters[i].m_MonsterCoord.z=z;
		Monsters[i].IsDead=false;
		Monsters[i].HP=Monsters[i].m_FullHP;
	}
}

void CMonsters::CutHP(unsigned int ID,unsigned int hp)
{
	Monsters[ID].HP=Monsters[ID].HP-hp;
}

int CMonsters::GetHP(unsigned int ID)
{
	return Monsters[ID].HP;
}

void CMonsters::ChaseHero(int id,float x,float z)
{
	//tempX、tempZ为英雄与怪物之间相距坐标
	float tempX,tempZ;
	int i=id;
// 	for(int i=0;i<=m_MonsterRange;i++)
// 	{
// 		if (Monsters[i].IsDead)
// 		{
// 			continue;
// 		}
		tempX= x - Monsters[i].m_MonsterCoord.x;
		tempZ= z - Monsters[i].m_MonsterCoord.z;
		if (fabs(tempX) < Monsters[i].ChaseRange && fabs(tempZ) < Monsters[i].ChaseRange)//当英雄在视野之内，去追击
		{
			//MessageBox(NULL,"sdf","sdf",MB_OK);
			/************************************************************************/
			/* 怪物转向,当正在发射激光炮的时候不改变怪物的方向                                                                     */
			/************************************************************************/
			if (!Monsters[i].IsShoot)
			{
				if (tempX < 0)
				{
					if (tempZ < 0)
					{
						Monsters[i].m_angle=90.0-atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
					}
					else
					{
						Monsters[i].m_angle=90.0+atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
					}
				}
				else
				{
					if (tempZ < 0)
					{
						Monsters[i].m_angle=270.0+atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
					}
					else
					{
						Monsters[i].m_angle=270.0-atan(fabs(tempZ)/fabs(tempX))*(180.0/PI);
					}
				}
			}
			
			float tempDis=(tempX * tempX) + (tempZ * tempZ);
			if (tempDis <= (Monsters[i].ShootRange * Monsters[i].ShootRange))//英雄已经在攻击范围之内,攻击，否则，跑到射击范围内
			{
				Monsters[i].status=MODEL_IDLE;//设置成停顿
				if (Monsters[i].ShootTime.ReachTime(3000))//隔3秒
				{
					Monsters[i].status=MODEL_ATTACK;//设置成开火
					Monsters[i].ShootDirection.x=Monsters[i].m_MonsterCoord.x + HitRandom();
					Monsters[i].ShootDirection.z=Monsters[i].m_MonsterCoord.z + HitRandom();
					Shoot(i,Monsters[i].ShootDirection.x,Monsters[i].ShootDirection.z);
					Monsters[i].ShootTime.SetTimenow();
					Monsters[i].IsShoot=true;
					
				}
				else if (Monsters[i].IsShoot)		//如果射击，则播放动画
				{
					//if (Monsters[i].ShootTime.ReachTime(100))//
					//{
						Shoot(i,Monsters[i].ShootDirection.x,Monsters[i].ShootDirection.z);
						Monsters[i].IsRun=false;
					//}
					
				}
				if ( Monsters[i].showlaser_step > Monsters[i].ShootRange)//如果射击完成则停止播放动画
				{
					Monsters[i].IsShoot=false;
				}
				Monsters[i].IsRun=false;

			}
			else		
			{
				Monsters[i].status=MODEL_RUN;
				if(Monsters[i].RunTime.ReachTime(40))
				{
					Monsters[i].IsRun=Run(i,Monsters[i].m_MonsterCoord.x,Monsters[i].m_MonsterCoord.z,x,z);
					Monsters[i].RunTime.SetTimenow();
				}

			}
		}
		else
		{
			Monsters[i].status=MODEL_IDLE;//当英雄不在怪物的视野范围之内，则设置成停顿
		}

//	}
}

void CMonsters::Shoot(unsigned int ID,float x,float z)
{
	float k,des_x,des_z;
	//因为当m_angle为0或者180时tan为无限在，所以分出来做特殊处理
	if (Monsters[ID].m_angle!=0.0 && Monsters[ID].m_angle!=180.0)
	{
		if (Monsters[ID].m_angle<=180.0)
		{
			k=tan((90.0-Monsters[ID].m_angle)*PI/180.0);
			des_x=-sqrt((Monsters[ID].showlaser_step*Monsters[ID].showlaser_step)/(k*k+1))+x;
			des_z=k*(des_x-x)+z;
		}
		else
		{
			k=-tan((90+Monsters[ID].m_angle)*PI/180.0);
			des_x=sqrt((Monsters[ID].showlaser_step*Monsters[ID].showlaser_step)/(k*k+1))+x;
			des_z=k*(des_x-x)+z;
		}

	}
	else if(Monsters[ID].m_angle == 0.0)
	{
		des_x=x;
		des_z=z-Monsters[ID].showlaser_step;
	}
	else
	{
		des_x=x;
		des_z=z+Monsters[ID].showlaser_step;
	}

	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glTranslatef(des_x,12.0,des_z);

	glColor3f(0.0, 1.0, 0.5);
	//auxSolidSphere(1.2);
	glPopMatrix();
	glPopAttrib();
	//showlaser_step应该是每个怪物维持一个
// 	glPushAttrib(GL_CURRENT_BIT);
// 	glPushMatrix();
	if (Monsters[ID].showlaser_step <= Monsters[ID].ShootRange)
	{
		Monsters[ID].showlaser_step += 8;
	}
	else
	{
		Monsters[ID].showlaser_step=8;
		Monsters[ID].IsShoot=false;
	}

	if(IsHitHero(x,z,des_x,des_z))	//当激光炮已经发射完后，检测是否击中怪物
	{
		//Monsters[ID].showlaser_step +=  Monsters[ID].ShootRange;		//击中后不再显示后面的动画
		Monsters[ID].showlaser_step=8;
		Monsters[ID].IsShoot=false;
	}
// 	glPopMatrix();
// 	glPopAttrib();
}


bool CMonsters::IsHitHero(float sx,float sz,float dx,float dz)
{
	float x1=0.0;
	Vector3 temp=myHero->m_RoleCoord;
	float k=(dz-sz)/(dx-sx);
	float x,z;
	float x2;
	if (dx >= sx)
	{
		x=sx;
		z=sz;
		x2=dx;
	}
	else
	{
		x=dx;
		z=dz;
		x2=sx;
	}
	while(x<=x2)
	{
		if ((temp.x-HITRANGE < x && x < temp.x+HITRANGE) && ((temp.z-HITRANGE) < z) && (z < (temp.z+HITRANGE)))
		{
			//m_Monster.SetBeHit(i,true);
			myHero->CutHP(10);
			return true;
		}
		x1=HITRANGE/sqrt(k*k+1);
		x+=x1;
		z=z+(x1*k);
	}
	return false;
}

//怪物击中的机率
int CMonsters::HitRandom()
{
	srand((unsigned) time(NULL));
	return rand()%8 - 4;
}


//由于desX 与 desZ不是怪物的目标坐标，因此可能会不对
bool CMonsters::Run(unsigned int ID,float &curX,float &curZ,float desX,float desZ)
{
	//if(curX != desX && curZ != desZ)
	if(!(curX == desX && curZ == desZ))
	{
		float k,x,z;
		float distance=Monsters[0].moveDistance;
		k=(desZ-curZ)/(desX-curX);
		//Vector3 tempPosition=m_Camera.getPosition();
		//勾股定理
		if (desX < curX)
		{
			x=0.0-sqrt(distance/(k*k+1));
		}
		else
		{
			x=sqrt(distance/(k*k+1));
		}

		z=k*x;

		if (curX < desX && desX < curX+x)
		{
			curX=desX;
			//tempPosition.x+=desX-curX;
		}
		else if (curX > desX && desX > curX+x)
		{
			curX=desX;
			//tempPosition.x+=desX-curX;
		}
		else
		{
			curX+=x;
			//tempPosition.x+=x;
		}

		if (curZ < desZ && desZ < curZ+z)
		{
			curZ=desZ;
			//tempPosition.z+=desZ-curZ;
		}
		else if (curZ > desZ && desZ > curZ+z)
		{
			curZ=desZ;
			//tempPosition.z+=desZ-curZ;
		}
		else
		{
			curZ+=z;
			//tempPosition.z+=z;
		}
		return true;
	}
	else
	{
		return false;
	}

}

void CMonsters::Run()
{

	//	for(int i=0;i<m_MonsterRange;i++)
	//	{
	if (Monsters[0].IsDead || Monsters[0].IsShoot)
	{
		//continue;
		return;
	}
	//		int sign,x,z;
	if (Monsters[0].IsRun==false && Monsters[0].IsShoot==false)
	{
		srand((int)time(0));
		int sign=rand()%4;
		int x=rand()%25;
		int z=rand()%25;

		switch(sign)
		{
			//case 0 :   不变
		case 1:x=-x; z=-z; break;
		case 2:z=-z; break;
		case 3:x=-x; break;

		}
		Monsters[0].m_MonsterDesCoord.x=Monsters[0].m_MonsterCoord.x+x;
		Monsters[0].m_MonsterDesCoord.z=Monsters[0].m_MonsterCoord.z+z;
	}

	Run(0,Monsters[0].m_MonsterCoord.x, Monsters[0].m_MonsterCoord.z, Monsters[0].m_MonsterDesCoord.x, Monsters[0].m_MonsterDesCoord.z);
}

Vector3 CMonsters::getMonsterCoord(unsigned int ID)
{
	if (ID < m_MonsterRange && !Monsters[ID].IsDead)
	{
		return Monsters[ID].m_MonsterCoord;
	}
	//Vector3 temp={0,-100,0};
	return Vector3(0, -100, 0);;
}


void CMonsters::ShowMonsterLife(unsigned int ID)
{
	if (Monsters[ID].BeHit)
	{
		if(Monsters[ID].LifeTime.ReachTime(1500))
		{
			Monsters[ID].BeHit=false;
		}
		unsigned char rasterallf[12] = {
			0xff, 0xff,0xff, 0xff,0xff, 0xff,
			0xff, 0xff,0xff, 0xff,0xff, 0xff
		};
		glPushAttrib(GL_CURRENT_BIT);
		glPushMatrix();
		glLoadIdentity();

		glTranslatef(0.3f, 0.40f, -1.0f);
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

		glTranslatef(0.3f, 0.40, -1.0);
		glColor3f (0.0, 1.0, 0.0);
		glRasterPos3f (0.0, 0.0,-0.1);
		int temp;
		if(Monsters[ID].m_FullHP!=0)
			temp=Monsters[ID].HP*MAXHP/Monsters[ID].m_FullHP;
		for (int i=0;i<temp;i++)
		{
			glBitmap (8, 11, 0.0, 0.0, 8.0, 0.0, rasterallf);
		}

		glPopMatrix();
		glPopAttrib();
	}
	else if(Monsters[ID].LifeTime.ReachTime(1500))//当大于1秒时，从新设定当前时间
	{
		Monsters[ID].LifeTime.SetTimenow();
	}
}



bool CMonsters::MinusMonster(unsigned int ID)
{
	Monsters[ID].IsDead=true;
	if (ID == m_MonsterRange - 1)
	{
		m_MonsterRange--;		//当ID为最大时m_MonsterRange减1
	}
	m_MonsterNum--;
	return true;
}


void CMonsters::Animate()
{
	//glPushMatrix();
	//glRotatef(-90.0, 1.0f, 0.0, 0.0);	//两个是调整坐标
	//glRotatef(90,0.0f,0.0f,1.0f);
	//ChaseHero(m_Hero.m_RoleCoord.x,m_Hero.m_RoleCoord.z);
	
	for (int i=0;i<m_MonsterRange;i++)
	{
		if (Monsters[i].IsDead)
		{
			continue;
		}
		ShowMonsterLife(i);
		ChaseHero(i,myHero->m_RoleCoord.x,myHero->m_RoleCoord.z);
		glPushMatrix();
		glTranslatef(Monsters[i].m_MonsterCoord.x,Monsters[i].m_MonsterCoord.y ,Monsters[i].m_MonsterCoord.z);
		glScalef(0.4,0.4,0.4);
		//Opengl是右手坐标系，不过这里坐标系是，上：Z 右：X 前：Y
		glRotatef(-90.0, 1.0f, 0.0, 0.0);	//两个是调整坐标
		glRotatef(90,0.0f,0.0f,1.0f);
		glRotatef(Monsters[i].m_angle, 0.0f, 0.0f, 1.0f);
		Monsters[i].md2.SetState(Monsters[i].status);
		Monsters[i].m_weapon.SetState(Monsters[i].status);
		//Monsters[i]
		int *frame=Monsters[i].md2.GetFrame(Monsters[i].status);
		Monsters[i].md2.Animate(frame[0],frame[1],Monsters[i].speed);
		Monsters[i].m_weapon.Animate(frame[0],frame[1],Monsters[i].speed);
		glPopMatrix();
		
	}
	//glPopMatrix();
}

void CMonsters::setMonsterCoord(float x, float y, float z,unsigned int ID)
{
	Monsters[ID].m_MonsterCoord.x=x;
	Monsters[ID].m_MonsterCoord.z=z;

}