#ifndef __MONSTERS_H__
#define __MONSTERS_H__

//#include "stdafx.h"
#include "md2.h"
#include "GameTime.h"
#include "Vector.h"
#include "Hero.h"			//hero.h已经包含了GetLuaData.h

#define MAXMONSTERNUM 64

struct monster
{
	CMD2Model md2;
	CMD2Model m_weapon;
	Vector3		m_MonsterCoord;
	Vector3		m_MonsterDesCoord;	//怪物将要到达的坐标，一般情况下是一样的
	int			m_FullHP;
	int			HP;
	float		m_angle;
	bool		IsDead;
	bool		BeHit;
	float		ChaseRange;						//追击英雄的范围
	float		ShootRange;						//射击范围
	float		scale;							//怪物放大的倍数
	float		speed;							//怪物行走速度
	bool		AttackDone;						//攻击完毕
	bool		IsShoot;						//已经攻击
	CGameTime	LifeTime;						//血条显示时间
	CGameTime   ShootTime;						//射击时间间隔
	CGameTime	RunTime;						//跑动时间间隔
	int			showlaser_step;					//激光炮步长
	Vector3		ShootDirection;					//怪物射击方向
	bool		IsRun;							//判断是否正在跑动
	modelState_t	status;						//怪物的状态，是走，还是停
	bool		HasWeapon;						//是否有武器
	char		*MonsterMd2;					//md2文件名
	char		*MonsterTexture;				//纹理名称
	char		*WeaponMd2;						//md2文件名
	char		*WeaponTexture;					//纹理名称
	int			SkillCoolTime;					//技能冷却时间,单位毫秒	
	int			m_Defense;						//怪物的防御值
	float		moveDistance;					//怪物一步移动的距离
};

class CMonsters
{
public:
	CMonsters();
	~CMonsters();

	void setMonsterCoord(float x, float y, float z,unsigned int MonsterID);
	Vector3 getMonsterCoord(unsigned int MonsterID);
	void CutHP(unsigned int MonsterID,unsigned int hp);        //减去血量
	int  GetHP(unsigned int MonsterID);
	int  GetFullHP(unsigned int MonsterID){return Monsters[MonsterID].m_FullHP;}
	void ChaseHero(int id,float x,float z);			//追击英雄,x,z为英雄的坐标
	void Shoot(unsigned int MonsterID,float x,float z/*,float angle*/);				//angle是保存射击
	void SetBeHit(unsigned int MonsterID,bool hit){Monsters[MonsterID].BeHit=hit;};
	void Run();					
	bool Run(unsigned int ID,float &curX,float &curZ,float desX,float desZ);					//当英雄在怪物的可视范围之内，怪物将跑到最近射程内进行射击
	float getAngle(unsigned int MonsterID){return Monsters[MonsterID].m_angle;};
	unsigned int getMonsterNum(){return m_MonsterNum;};
	unsigned int getMonsterRange(){return m_MonsterRange;};
	bool MinusMonster(unsigned int MonsterID);					//减少英雄，当英雄死亡时候调用
	void ShowLaser(unsigned int MonsterID,float x,float z);					//显示激光效果
	bool IsHitHero(float sx,float sz,float dx,float dz);					//是否击中英雄
	void ShowMonsterLife(unsigned int MonsterID);
	/*void ShowMonsterLife();*/
	void Animate();										//播放动画
	void Init(int s);										//初始化
	void ReInit();
	bool IsDead(unsigned int MonsterID){return Monsters[MonsterID].IsDead;};//判断怪物是否死亡
	unsigned int GetMonsterRange(){return m_MonsterRange;};
	int HitRandom();						////怪物击中英雄的机率
	void setMonsterNum(int num){m_MonsterNum=num;};
	
// 	CMD2Model md2;
// 	CMD2Model m_weapon;

private:

	monster  Monsters[MAXMONSTERNUM];
	unsigned int m_MonsterNum;		//怪物的数量,当怪物数量为0则全部死亡
	unsigned int m_LoadMonster;		//加载了的怪物数量
	unsigned int m_MonsterRange;	//怪物目前ID的最大数（中间可能有些怪物死亡了）
	int		   showlaser_step;				  //激光的显示次数（形成动画)

};


#endif