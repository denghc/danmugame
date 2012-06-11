#pragma once

class BulletStorm;
#include <vector>
#include "lua_head.h"

class BulletSystem
{
public:
	BulletSystem(void);
	~BulletSystem(void);

	void Initialize();// clear m_bulletStorms

	void ReadBSList();


	void ShutDown();
	void InsertBulletStorm(BulletStorm* bs);
	void UpdateFrame();
	void UpdateInterpolate(float interpoloate);
	//TODO
	// a function to fill all the vertexes in a render object
	bool IsCollided(float cameraAngle, float shipx, float shipy);
	int FillVertexBuffer(void* vb);
private:
	std::vector<BulletStorm*> m_bulletStorms;
	lua_State* m_luaState;
	std::vector<std::string> m_bslist;
};

