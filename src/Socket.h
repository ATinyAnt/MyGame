#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "stdafx.h"
#include "GameTime.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include <winsock.h>
#pragma comment(lib,"Wsock32.lib")

//跑的技能设置为'W'

#define NOSKILL  '!'

#define LOSECONNECT 0

//发送战斗协议
struct FightProtocal
{
	int hero_id;		//根据id识别是自己的英雄还是对方英雄的消息
	float x;
	float y;
	float z;
	float dx;
	float dy;
	float dz;
	float angle;
	int HP;				//当HP为0时，即该英雄已经死了
	char skill;			//使用的是何种技能
};

struct OwnID
{
	int id;
};

struct HeroBaseInfo
{
	int hero_id;
	float x;
	float z;
};

struct HeroInfo
{
	int hero_num;
	HeroBaseInfo base_info;
};


struct Ptotocal
{
	char flag[16];
	char msg[512];
};

class CFightSocket
{
public:
	CFightSocket();
	~CFightSocket(){};
	bool InitSocket(char *ipaddr,int port);
	void SendMsg(char *msg);
	int RecvMsg(char *msg);
	void SendFinghtInfo();
	void InitProtocal();

public:
	FightProtocal sendPro;
	FightProtocal recvPro;
	char recvMsg[1024];
	char sendMsg[1024];
	CoordOf2D P_Coord;			//闪电坐标
	int heroid[4];							//存储其它英雄的ID，用于网络对战
	int heroNum;							//连上了游戏的英雄（包括自己），用于网络对战
	CHero * fightHero[4];					//其它英雄的对象，用于网络对战


private:
	SOCKET destSocket;
	bool isInit;
	CGameTime sendTime;
};

#endif