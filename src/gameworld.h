
#ifndef __GAME_WORLD_H__
#define __GAME_WORLD_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "GLFrame.h"												/**< 包含基本的框架类 */
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

/** 从GL_Application派生出一个子类 */
class GameWorld : public GLApplication								
{
public:
	bool	Init();							/**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						/**< 执行所有的卸载工作 */
	void	Update(/*DWORD milliseconds*/);		/**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							/**< 执行所有的绘制操作 */
	
		
	void	DrawGrid();

	Vector3  From2DTo3D(float x, float y); //2维屏幕坐标转3维世界坐标
	void ShowMenu();

	void setHeroStatus(int flag);					//英雄的状态
	//Vector3 setCameraPosition(float x,float y,float z);	//设置摄像机的位置
	
	
	void LaserBomb(float x,float z);
	bool IsMoveOverMonster();			//鼠标是否滑过了怪物
	void ShowLaser(float x,float z);
	bool NextScene();
	void HeroEvent();
	void MonsterEvent();
	void MenuEvent();
	void FightEvent();
	void F_HeroEvent();
	void BeginOrtho();
	void EndOrtho();
	void ParallelView();				//平行视图
	void ProjectionView();				//投影视图
	void ChangeScreen();				//改变屏幕显示
	void ConnectGame();					//连网游戏
	//bool InitGame();
	void HandleRecvMessage(char msg[]);
	void HandleSendMessage();
	void SetFalse();

// 	static void ShowLaser();
	                     /**< 摄像机类 */
private:
	friend class GLApplication;				/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	GameWorld(const char * class_name);	/**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	
	CTerrain   m_Terrain;                     /**< 地形类 */
	//CTerrain2  m_Terrain2;
	
	//CMonster   m_Monster;					  //怪物类
	CHero	   m_Hero;						  //英雄类
	GLFont	   m_font;						  //屏幕字体类
  	GLFont	   m_infofont;
	float      m_Fps;                         /**< 帧速 */
	bool       m_RenderMode;		          /**< 绘制模式 */
	bool       sp;                            /**< 空格键是否释放 */
	bool       mp;                            /**< 'M'键是否释放 */
	POINT      mousePos;
	//float	   m_Hero.m_angle;						  //英雄旋转的角度
	
	string	   m_str;						  //屏幕输出字符
	CInitMenu  m_initmenu;					  //初始化菜单
	int		   showlaser_step;
	
	//CMD2Model  m_md2;
	CMD2Model  m_weapon;

// 	CMD2Model  m_md1;
// 	CMD2Model  m_weapon1;
	int SceneNum;								//定义场景数
	int CurrentScene;							//当前场景
	//bool IsNextScene;							//下一个场景
	bool m_is_open_menu;						//是否打开了菜单
	CGameTime testtime;							//测试时间表，用于测试
	CBombBall m_ball;
	CGameTime balltime;							//技能冷却时间
	CGameTime keytime;							//按键冷却时间
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


#define FIRINGRANGE 120   //射程

#define LBUTTON 0
#define RBUTTON 1

//#define HITRANGE 3.0		//激光杀伤范围




#endif	// __TEST_H__