
#include "luadata.h"

LuaData::LuaData()
{

}

LuaData::~LuaData()
{

}

bool LuaData::LoadLuaFile(string fileName) 
{  
	static string root_path = "..\\Lua\\";
	fileName = root_path + fileName;
	int result = luaL_loadfile(m_luaEnv, fileName.c_str());  
	if (result)  
	{  
		return false;  
	}  

	result = lua_pcall(m_luaEnv, 0, 0, 0);  
	return result == 0;  
}  


lua_State* LuaData::InitLuaEnv()  
{  
//	LuaEnv = lua_open(); //�ɰ汾�÷�
	m_luaEnv = luaL_newstate();
	luaopen_base(m_luaEnv);  
	luaL_openlibs(m_luaEnv);  

	return m_luaEnv;  
}  

// ��ȡȫ�ֺ���.  
//  
lua_CFunction LuaData::getGlobalProc(const string& procName)  
{  
	lua_getglobal(m_luaEnv, procName.c_str());  
	if (!lua_isfunction(m_luaEnv, 1))  
	{  
		return 0;  
	}  

	return lua_tocfunction(m_luaEnv, 1);  
} 

// ��ȡȫ���������



