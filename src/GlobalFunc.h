#ifndef __GLOBALFUNC_H__
#define __GLOBALFUNC_H__

//#include "Vector.h"
//bool IsLoadCube=false;		//�Ƿ��Ѿ�������
#define CAMERA 0
#define HERO 1
#define MONSTER 2
#define HITRANGE 2.0		//����ɱ�˷�Χ


struct Cube
{
	float bx;
	float bz;
	float ex;
	float ez;
};


struct CoordOf2D
{
	float x;
	float z;
};

// struct PointCoord
// {
// 	float x;
// 	float z;
// };

bool IsHitMonster(float sx,float sz,float dx,float dz,float hitrange=HITRANGE,int HP=15);

bool IsInJumpBitRange(bool gametype,float x, float z,float angle,float range=5.0,int id=-1);

bool IsInCube(float &x ,float &z,int type);

bool IsInCube(float &x ,float &z,CoordOf2D beginCoord,CoordOf2D endCoord,int type);

void LoadCubeCoord();

bool IsHitHero(float sx,float sz,float dx,float dz,float hitrange,int HP,int id=-1);
//vector GetCubeData();



//������������һ�������
CoordOf2D GetDistCoord(float sx,float sz,float dx,float dz,float dist);

CoordOf2D GetCoordByAngle(float x,float z,float angle,float dist);

#endif