#pragma once
#include "shaderclass.h"
#include <vector>

class PixelShaderClass :
	public ShaderClass
{
public:

	PixelShaderClass();
	PixelShaderClass(string tag);
	virtual ~PixelShaderClass(void);
	virtual bool Initialize(ID3D11Device*, HWND, WCHAR* vsFilename, CHAR* entryFuncName) = 0;
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, CHAR* entryFuncName) = 0;
	virtual bool InitializeConstantBuffer(ID3D11Device*) = 0;
	virtual bool SetRenderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*) = 0;
	virtual void Shutdown() = 0;
	bool Register();// register the shader in vertex shader manager
	bool Unregister();
protected:
	virtual void ShutdownShader() = 0; 
};

