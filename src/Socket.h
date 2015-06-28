#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "stdafx.h"
#include "GameTime.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include <winsock.h>
#pragma comment(lib,"Wsock32.lib")

//�ܵļ�������Ϊ'W'

#define NOSKILL  '!'

#define LOSECONNECT 0

//����ս��Э��
struct FightProtocal
{
	int hero_id;		//����idʶ�����Լ���Ӣ�ۻ��ǶԷ�Ӣ�۵���Ϣ
	float x;
	float y;
	float z;
	float dx;
	float dy;
	float dz;
	float angle;
	int HP;				//��HPΪ0ʱ������Ӣ���Ѿ�����
	char skill;			//ʹ�õ��Ǻ��ּ���
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
	CoordOf2D P_Coord;			//��������
	int heroid[4];							//�洢����Ӣ�۵�ID�����������ս
	int heroNum;							//��������Ϸ��Ӣ�ۣ������Լ��������������ս
	CHero * fightHero[4];					//����Ӣ�۵Ķ������������ս


private:
	SOCKET destSocket;
	bool isInit;
	CGameTime sendTime;
};

#endif