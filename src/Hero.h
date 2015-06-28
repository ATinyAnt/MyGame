#ifndef __HERO_H__
#define __HERO_H__

#include "stdafx.h"
#include "MD2Loader.h"
//#include "LaserBall.h"
#include "md2.h"
#include "Camera.h"
#include "GameTime.h"
#include "BombBall.h"
#include "Vector.h"
#include "GlobalFunc.h"
//#include "Socket.h"
//#include <stdarg.h>
//#include "H_md2.h"

#define INTDATA  0
#define CHARDATA 1

//OWN表示是自己,OTHER表示是其它的英雄
#define OWN 0
#define OTHER 1

class CHero
{
public:
	CHero();
	~CHero();
	
	void setHeroStatus(int flag);					//英雄的状态
	Vector3 setCameraPosition(float x,float y,float z);	//设置摄像机的位置
	//float getAngle(float x1,float y1,float x2,float y2);	
	float getAngle(int x,int z);   //获得英雄转向的夹角,x,z是世界的坐标
	float getAngle(){return m_angle;};				//英雄当前的转角
	void  setAngle(float angle){m_angle=angle;};
	//Vector3    TwoDtoThreeD(float x, float y); //2维屏幕坐标转3维世界坐标
	void HeroRun(float &curX,float &curY,float desX,float desY);		//英雄跑动

	void OpenFire();					//英雄开火
	void ShowLife(float x=-0.5f, float z=-1.0f,int id=OWN);       //显示英雄的生命
	void SetBeHit(bool isBeHit){BeHit=isBeHit;};
	int GetHP(){return HP;};		//获得英雄当前血量
	bool CutHP(int hp){HP=HP-hp;return true;};	//英雄被击中，要减去一定血量
	bool IsDead();
	void Init(bool gameType);
	void Animate(int scene=0,int id=OWN);
	bool Run(float &curX,float &curZ,float desX,float desZ,int id=OWN);
	void SetDistance(float dis){movedistance=dis;};
	void Rush();
	void SetSpeed(float s){curspeed=s;};
	void JumpBit();			//跳击,由下面两个动作完成
	void JumpBit(float &curX,float &curZ,float desX,float desZ,int id);
	void ShowBombBall();
	void ShowLaser(float x,float z);
	void Transmit(int &scene,int CurrentScene);			//传送
	//void Chrono();				//时空
	
	struct CoordOf2D* Lightning(float x,float z,bool isConnectGame,bool isLButtonDown,int id=OWN);

	void setHeroID(int id);
	int  getHeroID(){return heroID;};
	void setHeroCoord(float x,float z);
	//bool isHitHero(float sx,float sz,float dx,float dz,float hitrange,int HP);					//判断是否击中其它英雄，用于网络对战

	Vector3  m_RoleCoord;										//英雄当前坐标
	Vector3  m_RoleDesCoord;				//英雄要到达的坐标
	Vector3  m_Color;
	bool	   onlaser;
	int		   showlaser_step;				  //激光的显示次数（形成动画)
	int		   HP;							//英雄当前血量
	int		   FullHP;						//英雄最大血量
	float      ShootRange;						//射击范围
	int		 defense;						//英雄的防御值
	bool     isRush;							//快跑技能是否已冷却
	bool     isJumpBit;							//判断是否开启了跳击技能
	bool	 isLightning;
	
	//bool     isFire;
	bool showlightning;
	CGameTime jumpbittime;					//跳击冷却时间
	CGameTime rushtime;						//快跑技能冷却时间
	CGameTime lasertime;					//激光炮冷却时间
	CGameTime lightningtime;
	CGameTime showlifetime;
	CGameTime transmittime;
	int ownid;								//英雄自己的ID，用于网络对战
	bool isConnectGame;
	//CoordOf2D P_Coord;						//闪电坐标
// 	int heroid[4];							//存储其它英雄的ID，用于网络对战
// 	int heronum;							//连上了游戏的英雄（包括自己），用于网络对战
// 	CHero *fightHero[4];					//其它英雄的对象，用于网络对战

private:
	float	   m_angle;						  //英雄旋转的角度
	float	   movedistance;					//移动距离
	//CLaserBall m_laser;						//技能类
	CMD2Model  md2;
	CMD2Model  m_weapon;
	
	float curspeed;							//当前速度
	float speed;							//英雄的一般速度
								
	float rushspeed;
	
	
	float bitspeed;							//出拳的速度
	CBombBall m_ball;
	CBombBall m_lightning;
	int heroID;								//每一个fighthero的识别ID，与ownid略有不同
	bool BeHit;
	//bool IsShowBombBall;
	//modelState_t status;								//英雄当前的状态
	


};

#define LASERSTEPLEN 8     //激光炮显示间隔

#endif