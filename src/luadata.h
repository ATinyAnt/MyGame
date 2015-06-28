#ifndef __GETLUADATA_H__
#define __GETLUADATA_H__

#include "stdafx.h"



class LuaData
{
public:
	LuaData();
	~LuaData();

	bool LoadLuaFile(string fileName);
	lua_State * InitLuaEnv();
	lua_CFunction getGlobalProc(const string& procName);
	lua_State * GetLuaEnv()
	{
		return m_luaEnv;
	};

private:
	lua_State * m_luaEnv;      //

};
#endif