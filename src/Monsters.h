#ifndef __MONSTERS_H__
#define __MONSTERS_H__

//#include "stdafx.h"
#include "md2.h"
#include "GameTime.h"
#include "Vector.h"
#include "Hero.h"			//hero.h�Ѿ�������GetLuaData.h

#define MAXMONSTERNUM 64

struct monster
{
	CMD2Model md2;
	CMD2Model m_weapon;
	Vector3		m_MonsterCoord;
	Vector3		m_MonsterDesCoord;	//���ｫҪ��������꣬һ���������һ����
	int			m_FullHP;
	int			HP;
	float		m_angle;
	bool		IsDead;
	bool		BeHit;
	float		ChaseRange;						//׷��Ӣ�۵ķ�Χ
	float		ShootRange;						//�����Χ
	float		scale;							//����Ŵ�ı���
	float		speed;							//���������ٶ�
	bool		AttackDone;						//�������
	bool		IsShoot;						//�Ѿ�����
	CGameTime	LifeTime;						//Ѫ����ʾʱ��
	CGameTime   ShootTime;						//���ʱ����
	CGameTime	RunTime;						//�ܶ�ʱ����
	int			showlaser_step;					//�����ڲ���
	Vector3		ShootDirection;					//�����������
	bool		IsRun;							//�ж��Ƿ������ܶ�
	modelState_t	status;						//�����״̬�����ߣ�����ͣ
	bool		HasWeapon;						//�Ƿ�������
	char		*MonsterMd2;					//md2�ļ���
	char		*MonsterTexture;				//��������
	char		*WeaponMd2;						//md2�ļ���
	char		*WeaponTexture;					//��������
	int			SkillCoolTime;					//������ȴʱ��,��λ����	
	int			m_Defense;						//����ķ���ֵ
	float		moveDistance;					//����һ���ƶ��ľ���
};

class CMonsters
{
public:
	CMonsters();
	~CMonsters();

	void setMonsterCoord(float x, float y, float z,unsigned int MonsterID);
	Vector3 getMonsterCoord(unsigned int MonsterID);
	void CutHP(unsigned int MonsterID,unsigned int hp);        //��ȥѪ��
	int  GetHP(unsigned int MonsterID);
	int  GetFullHP(unsigned int MonsterID){return Monsters[MonsterID].m_FullHP;}
	void ChaseHero(int id,float x,float z);			//׷��Ӣ��,x,zΪӢ�۵�����
	void Shoot(unsigned int MonsterID,float x,float z/*,float angle*/);				//angle�Ǳ������
	void SetBeHit(unsigned int MonsterID,bool hit){Monsters[MonsterID].BeHit=hit;};
	void Run();					
	bool Run(unsigned int ID,float &curX,float &curZ,float desX,float desZ);					//��Ӣ���ڹ���Ŀ��ӷ�Χ֮�ڣ����ｫ�ܵ��������ڽ������
	float getAngle(unsigned int MonsterID){return Monsters[MonsterID].m_angle;};
	unsigned int getMonsterNum(){return m_MonsterNum;};
	unsigned int getMonsterRange(){return m_MonsterRange;};
	bool MinusMonster(unsigned int MonsterID);					//����Ӣ�ۣ���Ӣ������ʱ�����
	void ShowLaser(unsigned int MonsterID,float x,float z);					//��ʾ����Ч��
	bool IsHitHero(float sx,float sz,float dx,float dz);					//�Ƿ����Ӣ��
	void ShowMonsterLife(unsigned int MonsterID);
	/*void ShowMonsterLife();*/
	void Animate();										//���Ŷ���
	void Init(int s);										//��ʼ��
	void ReInit();
	bool IsDead(unsigned int MonsterID){return Monsters[MonsterID].IsDead;};//�жϹ����Ƿ�����
	unsigned int GetMonsterRange(){return m_MonsterRange;};
	int HitRandom();						////�������Ӣ�۵Ļ���
	void setMonsterNum(int num){m_MonsterNum=num;};
	
// 	CMD2Model md2;
// 	CMD2Model m_weapon;

private:

	monster  Monsters[MAXMONSTERNUM];
	unsigned int m_MonsterNum;		//���������,����������Ϊ0��ȫ������
	unsigned int m_LoadMonster;		//�����˵Ĺ�������
	unsigned int m_MonsterRange;	//����ĿǰID����������м������Щ���������ˣ�
	int		   showlaser_step;				  //�������ʾ�������γɶ���)

};


#endif