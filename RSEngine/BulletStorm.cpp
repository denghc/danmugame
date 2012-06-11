#include "BulletStorm.h"
#include "Structures.h"
#include "LuaUtils.h"

//#pragma comment(lib,"lua.lib")

/*
	load lua script
	call lua function
	fetch vertex
	put vertex in vertex buffer (some middle procedure)
*/
BulletStorm::BulletStorm(void)
{
}


BulletStorm::~BulletStorm(void)
{
}

int BulletStorm::GetVertexes(void* dest)
{
	int bytesCopy = 0;
	memcpy(dest, &(*m_vertexList.begin()), sizeof(BulletType) * m_vertexList.size());
	bytesCopy += sizeof(BulletType) * m_vertexList.size();
	return bytesCopy;
}

void BulletStorm::GetLines(void* dest)
{
	//memcpy(dest, m_lineList, sizeof(std::pair<unsigned int, unsigned int>) * m_lineCount);
}

// release all the resources
void BulletStorm::ShutDown()
{
	lua_close(m_luaState);
	//free(m_vertexList);

}

void BulletStorm::UpdateInterpolation(float delta)
{
	lua_getglobal(m_luaState, "updateInterpolation");
	lua_pushnumber(m_luaState, delta);
	lua_pcall(m_luaState, 1, 0, 0);
}

void BulletStorm::UpdateFrame()
{
	lua_getglobal(m_luaState, "updateFrame");
	int rett = lua_pcall(m_luaState, 0, 0, 0);
	const char* error = lua_tostring(m_luaState, -1);//打印错误结果

}

float GetField (lua_State* L, const char *key) 
{
	float result = 0.0f;
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	result = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

void BulletStorm::LoadBulletStorm(const char* fileName)
{
	// now just test lua
	int rett;
	// TODO to make this into a log system
	WCHAR   wsz[64]; 
	swprintf(wsz, L"%S ",   fileName); 
	LPCWSTR   p   =   wsz;
	OutputDebugString(p);
	lua_State *L = luaL_newstate();
	m_luaState = L;
	luaL_openlibs(L);
	rett = luaL_loadfile(L, fileName);
	rett = lua_pcall(L, 0, 0, 0);
	lua_getglobal(m_luaState, "initialize");
	rett = lua_pcall(m_luaState, 0, 0, 0);
	const char* error = lua_tostring(L, -1);//打印错误结果
// 	rett = lua_pcall(L, 0, 0, 0);
// 	lua_getglobal(L, "background");
// 	int outputd = (int)GetField(L, "b");
}



void BulletStorm::FetchBullets()
{
	// fetch lines
	lua_getglobal(m_luaState, "lineUpdated");
	int lineUpdated = (int)lua_tonumber(m_luaState, -1);
	lua_pop(m_luaState,1);
	if (lineUpdated)
	{
		lua_getglobal(m_luaState, "lineCount");
		m_lineCount = (int)lua_tonumber(m_luaState, -1);
		lua_pop(m_luaState, 1);
		m_lineList.clear();
		lua_getglobal(m_luaState, "line");
		for (int i = 0 ; i < m_lineCount; ++i)
		{
			std::pair<unsigned int, unsigned int> tpair;
			tpair.first = (int)LuaUtils::lua_getNumber(m_luaState, i * 2 + 1) - 1;
			tpair.second = (int)LuaUtils::lua_getNumber(m_luaState, i * 2 + 2) - 1;
			m_lineList.push_back(tpair);
		}
		lua_pop(m_luaState,1);// pop the table
	}

	// fetch vertexs
	lua_getglobal(m_luaState, "stripCount");
	int stripCount = (int)lua_tonumber(m_luaState, -1);
	lua_pop(m_luaState, 1);

	m_vertexList.clear();
	int totId = 1;
	for (int i = 0; i < stripCount; ++i)
	{
		lua_getglobal(m_luaState, "vetxCount");	
		int stripSize = (int)LuaUtils::lua_getNumber(m_luaState, i + 1);
		lua_pop(m_luaState, 1);

		lua_getglobal(m_luaState, "vetx");
		int tcount = 0;
		while (tcount < stripSize)
		{
			BulletType tmp;
			tmp.position.x = LuaUtils::lua_getNumber(m_luaState, totId);
			++totId;
			tmp.position.y = LuaUtils::lua_getNumber(m_luaState, totId);
			++totId;
			tmp.position.z = LuaUtils::lua_getNumber(m_luaState, totId);
			++totId;

			m_vertexList.push_back(tmp);

			++tcount;
		}
		lua_pop(m_luaState, 1);// pop the vetx table
	}
}

bool BulletStorm::IsDone()
{
	lua_getglobal(m_luaState, "isDone");
	lua_pcall(m_luaState, 0, 1, 0);
	int ret = (int)lua_tonumber(m_luaState, -1);
	lua_pop(m_luaState, 1);
	if (ret == 0)
		return false;
	return true;
}


bool BulletStorm::IsCollided(D3DXVECTOR3 norm, float shipx, float shipy)
{
	// prerequisite: bullets are already fetched from lua script
	// scan all points
	for (std::vector<BulletType>::iterator iter = m_vertexList.begin(); iter != m_vertexList.end(); ++iter)
	{
		float bx, by;
		bx = (*iter).position.x;
		float tmp = D3DXVec3Dot(&norm, &((*iter).position));
		D3DXVECTOR3 tnorm = -tmp * norm;
		D3DXVECTOR3 persP;
		D3DXVec3Add(&persP, &(*iter).position, &tnorm);
		D3DXVECTOR3 yvec;
		D3DXVec3Cross(&yvec, &D3DXVECTOR3(1, 0, 0), &norm);
		if (abs(yvec.y) > abs(yvec.z))
			by = persP.y / yvec.y;
		else by = persP.z / yvec.z;

		float dis = (sqrt((by - shipy) * (by - shipy) + (bx - shipx) * (bx - shipx)));
		if (dis < 7.0f)
		{
			return true;
		}
	}
	return false;
}