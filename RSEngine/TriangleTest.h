#pragma once
#include "modelclass.h"

class TestVS;
class TestPS;

class TriangleTest :
	public ModelClass
{
public:
	TriangleTest(void);
	virtual ~TriangleTest(void);

	virtual bool Initialize(ID3D11Device*);
	//virtual bool Initialize(ID3D11Device* device, WCHAR* textureFilename);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	virtual int GetIndexCount();
	//ID3D11ShaderResourceView* GetTexture();

private:
	virtual bool InitializeShaders(ID3D11Device*);
	virtual bool InitializeBuffers(ID3D11Device*);
	virtual void ShutdownBuffers();
	virtual void RenderBuffers(ID3D11DeviceContext*);
	virtual bool LoadTexture(ID3D11Device*);
	virtual void ReleaseTexture();//call texture manager to release
	virtual bool Update();
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_indexCount;
	int m_vertexCount;

private:
	//shaders
	TestVS* m_vs;
	TestPS* m_ps;
};

