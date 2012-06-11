#include "BulletSystem.h"
#include "BulletStorm.h"
#include "d3dx10math.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "LuaUtils.h"
BulletSystem::BulletSystem(void)
{
}


BulletSystem::~BulletSystem(void)
{
}


void BulletSystem::ShutDown()
{
	for (std::vector<BulletStorm*>::iterator iter = m_bulletStorms.begin(); iter < m_bulletStorms.end(); ++iter)
	{
		(*iter)->ShutDown();
	}
	m_bulletStorms.clear();
}

void BulletSystem::InsertBulletStorm(BulletStorm* bs)
{
	m_bulletStorms.push_back(bs);
}


void BulletSystem::Initialize()
{
	m_bslist.clear();
	m_bulletStorms.clear();
	srand(unsigned(time(0)));
	// Read from bulletslist.dat to get all the bullet file list
	ReadBSList();

	BulletStorm* bs = new BulletStorm();
	bs->LoadBulletStorm("BulletScript\\moxing1.lua");
	InsertBulletStorm(bs);

}

void BulletSystem::ReadBSList()
{
	int rett;
	lua_State *L = luaL_newstate();
	m_luaState = L;
	luaL_openlibs(L);
	rett = luaL_loadfile(L, "BulletScript\\random_fetch.lua");
	const char* error = lua_tostring(L, -1);//打印错误结果
	rett = lua_pcall(L, 0, 0, 0);
	error = lua_tostring(L, -1);//打印错误结果
	lua_getglobal(m_luaState, "get_bs_list");
	rett = lua_pcall(m_luaState, 0, 0, 0);

	error = lua_tostring(L, -1);//打印错误结果

	lua_getglobal(m_luaState, "fileCount");
	int fileCount = (int)lua_tonumber(m_luaState, -1);
	for (int i = 1; i <= fileCount; ++i)
	{
		lua_getglobal(m_luaState, "lines");
		m_bslist.push_back(LuaUtils::lua_getString(m_luaState, i));
	}
	lua_close(m_luaState);
}

void BulletSystem::UpdateFrame()
{
	std::vector<std::vector<BulletStorm*>::iterator> iterList;
	iterList.clear();
	for (std::vector<BulletStorm*>::iterator iter = m_bulletStorms.begin(); iter < m_bulletStorms.end(); ++iter)
	{
		(*iter)->UpdateFrame();
		if ((*iter)->IsDone())
		{
			iterList.push_back(iter);
		}
	}
	
	// now remove the outdated bulletstorm
	bool haveRemoved = false;
	for (std::vector<std::vector<BulletStorm*>::iterator>::iterator iteriter = iterList.begin(); 
		iteriter != iterList.end(); ++iteriter)
	{
		haveRemoved = true;
		m_bulletStorms.erase((*iteriter));


	}

	
	
	

	for (std::vector<BulletStorm*>::iterator iter = m_bulletStorms.begin(); iter < m_bulletStorms.end(); ++iter)
	{
		(*iter)->FetchBullets();
	}

	if (haveRemoved)
	{
		BulletStorm* bs = new BulletStorm();
		const char* fn = m_bslist[int(std::rand() * m_bslist.size() / (double)RAND_MAX)].c_str();
		bs->LoadBulletStorm(fn);
		InsertBulletStorm(bs);
	}
}

void BulletSystem::UpdateInterpolate(float interpoloate)
{
	for (std::vector<BulletStorm*>::iterator iter = m_bulletStorms.begin(); iter < m_bulletStorms.end(); ++iter)
	{
		(*iter)->UpdateInterpolation(interpoloate);
	}
	for (std::vector<BulletStorm*>::iterator iter = m_bulletStorms.begin(); iter < m_bulletStorms.end(); ++iter)
	{
		(*iter)->FetchBullets();
	}
}

bool BulletSystem::IsCollided(float cameraAngle, float shipx, float shipy)
{
	for (std::vector<BulletStorm*>::iterator iter = m_bulletStorms.begin(); iter < m_bulletStorms.end(); ++iter)
	{
		D3DXVECTOR3 norm;
		norm.x = 0;
		norm.y = sin(cameraAngle);
		norm.z = cos(cameraAngle);
		if ((*iter)->IsCollided(norm, shipx, shipy))
			return true;
	}
	return false;
}

int BulletSystem::FillVertexBuffer(void* vb)
{
	int bytesCopy = 0;
	for (std::vector<BulletStorm*>::iterator iter = m_bulletStorms.begin();
			iter != m_bulletStorms.end(); ++iter)
	{
		(*iter)->FetchBullets();
		bytesCopy += (*iter)->GetVertexes((void*)((char*)vb + bytesCopy));
		vb = (void*)((char*)vb + bytesCopy);
	}
	return bytesCopy;
}
