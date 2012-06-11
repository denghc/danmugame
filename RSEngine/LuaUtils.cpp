#include "LuaUtils.h"

float LuaUtils::lua_getNumber(lua_State* L, const lua_Number num) 
{
	float result = 0.0f;
	lua_pushnumber(L, num);
	lua_gettable(L, -2);
	result = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

std::string LuaUtils::lua_getString(lua_State* L, const lua_Number num) 
{
	std::string result;
	lua_pushnumber(L, num);
	lua_gettable(L, -2);
	result = (std::string)lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}
