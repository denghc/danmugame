#pragma once
#include "objmodelclass.h"
class SphereClass :
	public ObjModelClass
{
public:
	SphereClass(char* objFileName);
	~SphereClass(void);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix);
protected:
	virtual bool InitializeShaders(ID3D11Device*);
	virtual bool LoadTexture(ID3D11Device*);
	virtual void ShutdownBuffers();
private:
	TextureClass* m_texture;
};

