#pragma once
#include "vertexshaderclass.h"
class TestVS :
	public VertexShaderClass
{
public:
	TestVS();
	TestVS(string tag);
	virtual ~TestVS(void);

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, CHAR* entryFuncName);
	virtual bool InitializeConstantBuffer(ID3D11Device* device);
	virtual bool SetRenderParameters(ID3D11DeviceContext* device, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	virtual void Shutdown();

private:
	virtual void ShutdownShader();
	virtual void ShutdownConstantBuffer(){};

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

};

