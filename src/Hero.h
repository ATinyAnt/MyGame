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

//OWN��ʾ���Լ�,OTHER��ʾ��������Ӣ��
#define OWN 0
#define OTHER 1

class CHero
{
public:
	CHero();
	~CHero();
	
	void setHeroStatus(int flag);					//Ӣ�۵�״̬
	Vector3 setCameraPosition(float x,float y,float z);	//�����������λ��
	//float getAngle(float x1,float y1,float x2,float y2);	
	float getAngle(int x,int z);   //���Ӣ��ת��ļн�,x,z�����������
	float getAngle(){return m_angle;};				//Ӣ�۵�ǰ��ת��
	void  setAngle(float angle){m_angle=angle;};
	//Vector3    TwoDtoThreeD(float x, float y); //2ά��Ļ����ת3ά��������
	void HeroRun(float &curX,float &curY,float desX,float desY);		//Ӣ���ܶ�

	void OpenFire();					//Ӣ�ۿ���
	void ShowLife(float x=-0.5f, float z=-1.0f,int id=OWN);       //��ʾӢ�۵�����
	void SetBeHit(bool isBeHit){BeHit=isBeHit;};
	int GetHP(){return HP;};		//���Ӣ�۵�ǰѪ��
	bool CutHP(int hp){HP=HP-hp;return true;};	//Ӣ�۱����У�Ҫ��ȥһ��Ѫ��
	bool IsDead();
	void Init(bool gameType);
	void Animate(int scene=0,int id=OWN);
	bool Run(float &curX,float &curZ,float desX,float desZ,int id=OWN);
	void SetDistance(float dis){movedistance=dis;};
	void Rush();
	void SetSpeed(float s){curspeed=s;};
	void JumpBit();			//����,�����������������
	void JumpBit(float &curX,float &curZ,float desX,float desZ,int id);
	void ShowBombBall();
	void ShowLaser(float x,float z);
	void Transmit(int &scene,int CurrentScene);			//����
	//void Chrono();				//ʱ��
	
	struct CoordOf2D* Lightning(float x,float z,bool isConnectGame,bool isLButtonDown,int id=OWN);

	void setHeroID(int id);
	int  getHeroID(){return heroID;};
	void setHeroCoord(float x,float z);
	//bool isHitHero(float sx,float sz,float dx,float dz,float hitrange,int HP);					//�ж��Ƿ��������Ӣ�ۣ����������ս

	Vector3  m_RoleCoord;										//Ӣ�۵�ǰ����
	Vector3  m_RoleDesCoord;				//Ӣ��Ҫ���������
	Vector3  m_Color;
	bool	   onlaser;
	int		   showlaser_step;				  //�������ʾ�������γɶ���)
	int		   HP;							//Ӣ�۵�ǰѪ��
	int		   FullHP;						//Ӣ�����Ѫ��
	float      ShootRange;						//�����Χ
	int		 defense;						//Ӣ�۵ķ���ֵ
	bool     isRush;							//���ܼ����Ƿ�����ȴ
	bool     isJumpBit;							//�ж��Ƿ�������������
	bool	 isLightning;
	
	//bool     isFire;
	bool showlightning;
	CGameTime jumpbittime;					//������ȴʱ��
	CGameTime rushtime;						//���ܼ�����ȴʱ��
	CGameTime lasertime;					//��������ȴʱ��
	CGameTime lightningtime;
	CGameTime showlifetime;
	CGameTime transmittime;
	int ownid;								//Ӣ���Լ���ID�����������ս
	bool isConnectGame;
	//CoordOf2D P_Coord;						//��������
// 	int heroid[4];							//�洢����Ӣ�۵�ID�����������ս
// 	int heronum;							//��������Ϸ��Ӣ�ۣ������Լ��������������ս
// 	CHero *fightHero[4];					//����Ӣ�۵Ķ������������ս

private:
	float	   m_angle;						  //Ӣ����ת�ĽǶ�
	float	   movedistance;					//�ƶ�����
	//CLaserBall m_laser;						//������
	CMD2Model  md2;
	CMD2Model  m_weapon;
	
	float curspeed;							//��ǰ�ٶ�
	float speed;							//Ӣ�۵�һ���ٶ�
								
	float rushspeed;
	
	
	float bitspeed;							//��ȭ���ٶ�
	CBombBall m_ball;
	CBombBall m_lightning;
	int heroID;								//ÿһ��fighthero��ʶ��ID����ownid���в�ͬ
	bool BeHit;
	//bool IsShowBombBall;
	//modelState_t status;								//Ӣ�۵�ǰ��״̬
	


};

#define LASERSTEPLEN 8     //��������ʾ���

#endif