
#ifndef __GAME_WORLD_H__
#define __GAME_WORLD_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "GLFrame.h"												/**< ���������Ŀ���� */
#include "Font.h"
#include "Camera.h"
#include "Terrain.h"
#include "MD2Loader.h"
#include "Vector.h"
#include "Socket.h"
#include "InitMenu.h"
#include "Hero.h"
#include "md2.h"
#include "Monsters.h"
#include "GameTime.h"
#include "BombBall.h"
#include "ShowMenu.h"
#include "resource.h"
#include "GlobalFunc.h"
#include "Camp.h"
#include "Log.h"

//#include <assert.h>

CMonsters  m_Monster;

/** ��GL_Application������һ������ */
class GameWorld : public GLApplication								
{
public:
	bool	Init();							/**< ִ�����еĳ�ʼ������������ɹ���������true */
	void	Uninit();						/**< ִ�����е�ж�ع��� */
	void	Update(/*DWORD milliseconds*/);		/**< ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ */
	void	Draw();							/**< ִ�����еĻ��Ʋ��� */
	
		
	void	DrawGrid();

	Vector3  From2DTo3D(float x, float y); //2ά��Ļ����ת3ά��������
	void ShowMenu();

	void setHeroStatus(int flag);					//Ӣ�۵�״̬
	//Vector3 setCameraPosition(float x,float y,float z);	//�����������λ��
	
	
	void LaserBomb(float x,float z);
	bool IsMoveOverMonster();			//����Ƿ񻬹��˹���
	void ShowLaser(float x,float z);
	bool NextScene();
	void HeroEvent();
	void MonsterEvent();
	void MenuEvent();
	void FightEvent();
	void F_HeroEvent();
	void BeginOrtho();
	void EndOrtho();
	void ParallelView();				//ƽ����ͼ
	void ProjectionView();				//ͶӰ��ͼ
	void ChangeScreen();				//�ı���Ļ��ʾ
	void ConnectGame();					//������Ϸ
	//bool InitGame();
	void HandleRecvMessage(char msg[]);
	void HandleSendMessage();
	void SetFalse();

// 	static void ShowLaser();
	                     /**< ������� */
private:
	friend class GLApplication;				/**< ����Ϊ����һ����Ԫ�࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name) */
	GameWorld(const char * class_name);	/**< ���캯�� */

	/** �û��Զ���ĳ������ */ 
	
	CTerrain   m_Terrain;                     /**< ������ */
	//CTerrain2  m_Terrain2;
	
	//CMonster   m_Monster;					  //������
	CHero	   m_Hero;						  //Ӣ����
	GLFont	   m_font;						  //��Ļ������
  	GLFont	   m_infofont;
	float      m_Fps;                         /**< ֡�� */
	bool       m_RenderMode;		          /**< ����ģʽ */
	bool       sp;                            /**< �ո���Ƿ��ͷ� */
	bool       mp;                            /**< 'M'���Ƿ��ͷ� */
	POINT      mousePos;
	//float	   m_Hero.m_angle;						  //Ӣ����ת�ĽǶ�
	
	string	   m_str;						  //��Ļ����ַ�
	CInitMenu  m_initmenu;					  //��ʼ���˵�
	int		   showlaser_step;
	
	//CMD2Model  m_md2;
	CMD2Model  m_weapon;

// 	CMD2Model  m_md1;
// 	CMD2Model  m_weapon1;
	int SceneNum;								//���峡����
	int CurrentScene;							//��ǰ����
	//bool IsNextScene;							//��һ������
	bool m_is_open_menu;						//�Ƿ���˲˵�
	CGameTime testtime;							//����ʱ������ڲ���
	CBombBall m_ball;
	CGameTime balltime;							//������ȴʱ��
	CGameTime keytime;							//������ȴʱ��
	CGameTime menutime;
	CGameTime startgametime;
	//CGameTime lightningtime;
	CShowMenu m_showmenu;
	CCamp     m_camp;
	CLog	  m_log;	
	
	CFightSocket FS;
	int AllScene;
	bool IsWin;
	bool IsFight;
	bool begin_game;
	//bool isTransmit;
	
	 
	int firstin;
	
	bool goon;
	HCURSOR cur;
};


#define FIRINGRANGE 120   //���

#define LBUTTON 0
#define RBUTTON 1

//#define HITRANGE 3.0		//����ɱ�˷�Χ




#endif	// __TEST_H__