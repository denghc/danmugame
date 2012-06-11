#pragma once
#include "lua_head.h"

#include <vector>

struct D3DXVECTOR3;
//#include "Structures.h"
class BulletStrip;
struct BulletType;
class BulletStorm
{
public:
	BulletStorm(void);
	~BulletStorm(void);

public:
	int GetVertexes(void* dest);
	void GetLines(void* dest);
	//load .lua file and initialize the bullet storm
	void LoadBulletStorm(const char* fileName);
	void UpdateFrame();
	void UpdateInterpolation(float delta);
	void FetchBullets();
	void ShutDown();
	bool IsDone();
	// must be called after FetchBullets()
	bool IsCollided(D3DXVECTOR3 norm, float shipx, float shipy);

protected:
	unsigned int m_frameCount;
	unsigned int m_vertexCount;
	unsigned int m_lineCount;

	std::vector<BulletType> m_vertexList;
	//unsigned int m_stripCount;// number of bullet strip
	//std::vector<BulletStrip*> m_stripList;
	std::vector<std::pair<unsigned int, unsigned int>> m_lineList;

private:
	lua_State* m_luaState;


};

