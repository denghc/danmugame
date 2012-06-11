#pragma once
#include "vertexshaderclass.h"
#include "Structures.h"
class RSObjVS :
	public VertexShaderClass
{
public:
	RSObjVS(string tag);
	RSObjVS(void);
	~RSObjVS(void);
	virtual bool Initialize(ID3D11Device*, HWND, WCHAR*, CHAR*);
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, CHAR* entryFuncName);
	virtual bool InitializeConstantBuffer(ID3D11Device*);
	virtual bool SetRenderParameters(ID3D11DeviceContext* device, D3DXMATRIX worldMatrix, 
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	virtual void Shutdown();

private:
	virtual void ShutdownConstantBuffer();
	virtual void ShutdownShader();

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_rsObjBuffer;
};

