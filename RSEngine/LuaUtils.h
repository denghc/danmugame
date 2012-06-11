#include "lua_head.h"
#include <string>
namespace LuaUtils
{
	float lua_getNumber(lua_State* L, const lua_Number num);
	std::string lua_getString(lua_State* L, const lua_Number num);
}