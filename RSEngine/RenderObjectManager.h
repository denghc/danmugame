#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include <vector>
class RenderObject;

class RenderObjectManager
{
public:

	~RenderObjectManager(void);
	
	void InsertRenderObject(RenderObject* rob);
	void Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXMATRIX orthoMatrix);
	void Initialize();
	void Shutdown();
	void RemoveRenderObject(RenderObject* rob);

	static RenderObjectManager* GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new RenderObjectManager();
		}
		return m_instance;
	}

private:
	RenderObjectManager(void);

private:
	std::vector<RenderObject*> m_renderObjList;
	static RenderObjectManager* m_instance;

};

