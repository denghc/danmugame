#pragma once
#include "shaderclass.h"
#include <vector>

class VertexShaderClass :
	public ShaderClass
{
public:
	VertexShaderClass();
	VertexShaderClass(string tag);
	virtual ~VertexShaderClass(void);
	virtual bool Initialize(ID3D11Device*, HWND, WCHAR*, CHAR*) = 0;
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, CHAR* entryFuncName) = 0;
	virtual bool InitializeConstantBuffer(ID3D11Device*) = 0;
	virtual bool SetRenderParameters(ID3D11DeviceContext* device, D3DXMATRIX worldMatrix, 
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) = 0;
	virtual void Shutdown()= 0;
	bool Register();// register the shader in vertex shader manager
	bool Unregister();
protected:
	virtual void ShutdownShader() = 0;
	virtual void ShutdownConstantBuffer() = 0;




};

