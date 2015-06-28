/**  
*
*
*   @file   stdafx.h
*
*
*/

#ifndef __STDAFX_H__
#define __STDAFX_H__

/** 包含常用的头文件 */
#include <windows.h>
#include <stdio.h>
#include <math.h>                            
#include <time.h>
#include "GlobalFunc.h"

/** 包含gl头文件 */
				 
#include "glew.h"
#include "wglew.h"
#include <gl/GLU.h>

#include <iostream>
#include <string>
using namespace std;


extern "C"  
{  
#include "lua.h"  
#include "lualib.h"  
#include "lauxlib.h"  
//#pragma comment(lib, "Lua.lib")  
};  


/**< 禁止编译器出现类型转换的警告 */
#pragma warning(disable: 4311)                                 
#pragma warning(disable: 4312)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 4267)


/** 定义地面网格 */
const unsigned int MAP_WIDTH = 1024;
const unsigned int CELL_WIDTH = 16;



//#include "Monsters.h"
static const UINT WM_FINGHT_MSG = (WM_USER + 22);

#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1

#define GL_COMBINE_ARB 0x8570
#define GL_RGB_SCALE_ARB 0x8573



#define PI 3.141592654
#define MAXHP 20			//血条显示的最大长度
//#define MOVWDISTANCE 0.4	//怪物一步移动距离

#define BEGINGAME   0
#define CONNECTGAME 1
#define EXITGAME    2		//退出游戏，用于按钮
#define RETURNGAME  3		//返回游戏，用于按钮


#define CUBECOORD 476.0		//地图的最大坐标

#define DEG2RAD(x) (x*PI)/180.0f
#endif