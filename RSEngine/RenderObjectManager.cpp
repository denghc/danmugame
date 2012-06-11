#include "RenderObjectManager.h"
#include "RenderObject.h"

RenderObjectManager* RenderObjectManager::m_instance = 0;

RenderObjectManager::RenderObjectManager(void)
{
}


RenderObjectManager::~RenderObjectManager(void)
{
}

void RenderObjectManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix, 
	D3DXMATRIX projMatrix, D3DXMATRIX orthoMatrix)
{
	for (std::vector<RenderObject*>::iterator iter = this->m_renderObjList.begin(); iter != this->m_renderObjList.end(); ++iter)
	{
		RenderObject* rob = *iter;
		rob->Update();
		if (rob->IsVisible())
		{
			if (!rob->Is2D())
				rob->Render(deviceContext, viewMatrix, projMatrix);
			else
				rob->Render(deviceContext, viewMatrix, orthoMatrix);
		}
	}
}

void RenderObjectManager::Initialize()
{
	this->m_renderObjList.clear();
}

void RenderObjectManager::Shutdown()
{
	for (std::vector<RenderObject*>::iterator iter = this->m_renderObjList.begin(); iter != this->m_renderObjList.end(); ++iter)
	{
		RenderObject* rob = *iter;
		rob->Shutdown();
	}
	this->m_renderObjList.clear();
}

void RenderObjectManager::InsertRenderObject(RenderObject* rob)
{
	this->m_renderObjList.push_back(rob);
}


void RenderObjectManager::RemoveRenderObject(RenderObject* rob)
{
	for (std::vector<RenderObject*>::iterator iter = this->m_renderObjList.begin(); iter != this->m_renderObjList.end(); ++iter)
	{
		if (rob == *iter)
		{
			this->m_renderObjList.erase(iter);
			return;
		}
	}
}

