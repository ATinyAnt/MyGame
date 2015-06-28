
#include "stdafx.h"
#include <vector>
#include "GlobalFunc.h"
#include "Monsters.h"
#include "Hero.h"
#include "Vector.h"
#include "Socket.h"
#include "luadata.h"

using namespace std;

extern CMonsters  m_Monster;
extern CHero *myHero;
extern Camera m_Camera;
extern CFightSocket *F_S;
bool IsLoadCubeCoord = false;


//保存立方体的容器
vector<Cube> cube;
vector<Cube>::iterator itr;

//Vector3 setCameraPosition(float x,float y,float z);

bool IsHitMonster(float sx,float sz,float dx,float dz,float hitrange,int HP)
{
	float x1=0.0;
	for(int i=0;i<m_Monster.getMonsterRange();i++)
	{
		//MessageBox(NULL,"击中sss了怪物","牛B",MB_OK);
		if (m_Monster.IsDead(i))		//当怪物死亡，就检测下一只怪物
		{
			continue;
		}
		Vector3 temp=m_Monster.getMonsterCoord(i);
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
			if ((temp.x-hitrange < x && x < temp.x+hitrange) && ((temp.z-hitrange) < z) && (z < (temp.z+hitrange)))
			{
				m_Monster.SetBeHit(i,true);
				m_Monster.CutHP(i,HP);
				if (m_Monster.GetHP(i)<=0)		//	当怪物的血量为0时，减去该怪物
				{
					m_Monster.MinusMonster(i);
				}
				//LaserBomb(x,z);
				//MessageBox(NULL,"击中sss了怪物","牛B",MB_OK);
				m_Monster.ShowMonsterLife(i);
				return true;
			}
			x1=HITRANGE/sqrt(k*k+1);
			x+=x1;
			z=z+(x1*k);
		}
	}


	return false;
}

bool IsInJumpBitRange(bool gametype,float x, float z,float angle,float range,int id)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gametype && (id != OWN))
	{
		for(int i=0;i<F_S->heroNum;++i)
		{
			if (F_S->fightHero[i]->getHeroID()!=myHero->ownid)
			{
				//if ((F_S->fightHero[i]->m_RoleCoord.x-x)*(F_S->fightHero[i]->m_RoleCoord.x-x) + ((F_S->fightHero[i]->m_RoleCoord.z-z)*(F_S->fightHero[i]->m_RoleCoord.z-z))<=(range * range))
				if (((myHero->m_RoleCoord.x-x)*(myHero->m_RoleCoord.x-x))+((myHero->m_RoleCoord.z-z)*(myHero->m_RoleCoord.z-z))<(range*range))
				{
					CoordOf2D coord=GetCoordByAngle(x,z,angle,4.0);
					//F_S->fightHero[i]->setHeroCoord(coord.x,coord.z);
					myHero->setHeroCoord(coord.x,coord.z);
					//F_S->fightHero[i]->setHeroCoord(x,z);
				}
			}
		}
	}
	else if(!gametype)
	{
		for(int i=0;i<m_Monster.getMonsterRange();i++)
		{
			Vector3 monster_coord=m_Monster.getMonsterCoord(i);
			if (((monster_coord.x-x)*(monster_coord.x-x)) + ((monster_coord.z-z)*(monster_coord.z-z)) <= (range * range))
			{
				//MessageBox(NULL,"fsdf","fsdf",MB_OK);
				CoordOf2D coord=GetCoordByAngle(x,z,angle,4.0);
				m_Monster.setMonsterCoord(coord.x,0,coord.z,i);
			}
		}
	}
	
	return true;
}


//检测是否在方形外
bool IsInCube(float &x ,float &z,int type)
{
	LoadCubeCoord();
	if (type==HERO)
	{			
		float k=0.001;
		if (myHero->m_RoleDesCoord.x != myHero->m_RoleCoord.x)
		{
			k=(myHero->m_RoleDesCoord.z-myHero->m_RoleCoord.z)/(myHero->m_RoleDesCoord.x-myHero->m_RoleCoord.x);
		}
		for(itr=cube.begin();itr!=cube.end();itr++)
		{
			if (itr->bx < x && x < itr->ex && itr->bz < z && z < itr->ez)
			{
				if (myHero->m_RoleCoord.x < itr->bx && itr->bz < myHero->m_RoleCoord.z && myHero->m_RoleCoord.z < itr->ez)
				{
					x=itr->bx-1.0;
					z=k*(x-myHero->m_RoleCoord.x)+myHero->m_RoleCoord.z;
				}
				else if (myHero->m_RoleCoord.x > itr->ex && itr->bz < myHero->m_RoleCoord.z && myHero->m_RoleCoord.z < itr->ez)
				{
					x=itr->ex+1.0;
					z=k*(x-myHero->m_RoleCoord.x)+myHero->m_RoleCoord.z;
				}
				else if (myHero->m_RoleCoord.z < itr->bz && itr->bx < myHero->m_RoleCoord.x && myHero->m_RoleCoord.x < itr->ex)
				{
					x=(itr->bz-myHero->m_RoleCoord.z)/k + myHero->m_RoleCoord.x;
					z=itr->bz-1.0;
				}
				else if (myHero->m_RoleCoord.z > itr->ez && itr->bx < myHero->m_RoleCoord.x && myHero->m_RoleCoord.x < itr->ex)
				{
					x=(itr->ez-myHero->m_RoleCoord.z)/k + myHero->m_RoleCoord.x;
					z=itr->ez+1.0;
				}
				return true;
			}
		}
	}
	else if (type == CAMERA)
	{
		float k=0.001;
		Vector3 temp_P=m_Camera.getPosition();
		Vector3 temp_V=m_Camera.getView();
		if (temp_P.x != temp_V.x)
		{
			k=(temp_V.z-temp_P.z)/(temp_V.x-temp_P.x);
		}
		for(itr=cube.begin();itr!=cube.end();itr++)
		{
			if (itr->bx-1.0 <= x && x <= itr->ex+1.0 && itr->bz-1.0 <= z && z <= itr->ez+1.0)
			{
				return true;
			}
		}
	}

	return false;
}

//检测是否在方形内
bool IsInCube(float &x ,float &z,CoordOf2D beginCoord,CoordOf2D endCoord,int type)
{
	if (type==HERO)
	{
		if (x > endCoord.x)
		{
			x = endCoord.x;
		}
		else if (x < beginCoord.x)
		{
			x = beginCoord.x;
		}

		if (z > endCoord.z)
		{
			z = endCoord.z;
		}
		else if (z < beginCoord.z)
		{
			z = beginCoord.z;
		}
	}
	else if(type==CAMERA)
	{
		if (x > endCoord.x || x < beginCoord.x || z > endCoord.z || z < beginCoord.z)
		{
			return false;
		}
	}

	return true;
}


CoordOf2D GetDistCoord(float sx,float sz,float dx,float dz,float dist)
{
	struct CoordOf2D recoord={0.0,0.0};
	if (sx!=dx)
	{
		float k=0.0;
		k=(dz-sz)/(dx-sx);
		//勾股定理
		if (dx < sx)
		{
			recoord.x=0.0-sqrt(dist/(k*k+1));
		}
		else
		{
			recoord.x=sqrt(dist/(k*k+1));
		}

		recoord.z=k*recoord.x;
	}
	else
	{
		if (dz>=sz)
		{
			recoord.z=dist;
		}
		else
		{
			recoord.z=-dist;
		}
		
	}
	return recoord;
}


CoordOf2D GetCoordByAngle(float x,float z,float angle,float dist)
{
	struct CoordOf2D recoord={0.0,0.0};
	float k=0.0;
	if (angle==0.0 || angle==180.0)
	{
		angle+=0.001;
	}
	//因为当m_angle为0或者180时tan为无限在，所以分出来做特殊处理
	if (angle!=0.0 && angle!=180.0)
	{
		if (angle<=180.0)
		{
			k=tan((90.0-angle)*PI/180.0);
			recoord.x=-sqrt((dist*dist)/(k*k+1))+x;
			recoord.z=k*(recoord.x-x)+z;
		}
		else
		{
			k=-tan((90+angle)*PI/180.0);
			recoord.x=sqrt((dist*dist)/(k*k+1))+x;
			recoord.z=k*(recoord.x-x)+z;
		}

	}
	return recoord;
}

void LoadCubeCoord()
{
	if (IsLoadCubeCoord)
	{
		return;
	}
	LuaData lua;
	int coordnum=0;
	Cube cubetemp;
	lua.InitLuaEnv();
	//lua.SetLuaFile("Lua\\cube.lua");
	if (!lua.LoadLuaFile("cube.lua"))
	//if (!lua.LoadLuaFile())
	{
		MessageBox(NULL,"加载数据文件出错","LoadCubeCoord 出错",MB_OK);
		exit(0);
	}
	
	lua.getGlobalProc("GetCubeNum");
	lua_pcall(lua.GetLuaEnv(), 0, 1, 0);
	int cubenum=lua_tonumber(lua.GetLuaEnv(),-1);
	coordnum=cubenum * 4;
	lua.getGlobalProc("AllCubeData");
	lua_pcall(lua.GetLuaEnv(), 0, coordnum, 0);
	for(int i=-1;i>=-coordnum;i=i-4)
	{
		cubetemp.bx=(float)lua_tonumber(lua.GetLuaEnv(),i-3);
		cubetemp.bz=(float)lua_tonumber(lua.GetLuaEnv(),i-2);
		cubetemp.ex=(float)lua_tonumber(lua.GetLuaEnv(),i-1);
		cubetemp.ez=(float)lua_tonumber(lua.GetLuaEnv(),i);
		cube.push_back(cubetemp);
	}
	IsLoadCubeCoord=true;
}

//主要是判断是否击中“自己”，每个连上对战的自己
bool IsHitHero(float sx,float sz,float dx,float dz,float hitrange,int HP,int id)
{
	if(myHero->getHeroID()==id)
	{
		for(int i=0;i<F_S->heroNum;++i)
		{
			if (F_S->fightHero[i]->getHeroID()!=id)
			{
				float x1=0.0;
				Vector3 temp=F_S->fightHero[i]->m_RoleCoord;
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
					if ((temp.x-hitrange < x && x < temp.x+hitrange) && ((temp.z-hitrange) < z) && (z < (temp.z+hitrange)))
					{
						//myHero->CutHP(HP);
						return true;
					}
					x1=HITRANGE/sqrt(k*k+1);
					x+=x1;
					z=z+(x1*k);
				}
			}
		}
		return false;
	}
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
		if ((temp.x-hitrange < x && x < temp.x+hitrange) && ((temp.z-hitrange) < z) && (z < (temp.z+hitrange)))
		{
			myHero->CutHP(HP);
			return true;
		}
		x1=HITRANGE/sqrt(k*k+1);
		x+=x1;
		z=z+(x1*k);
	}

	return false;
}



