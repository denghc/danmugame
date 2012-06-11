#pragma once
#include "shaderclass.h"
class GeometryShaderClass :
	public ShaderClass
{
public:
	GeometryShaderClass();
	GeometryShaderClass(string tag);
	virtual ~GeometryShaderClass(void);
	virtual bool Initialize(ID3D11Device*, HWND){return true;};
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, CHAR* entryFuncName){return true;};
	virtual bool InitializeConstantBuffer(ID3D11Device*) = 0;
	virtual bool SetRenderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*) = 0;
	virtual void Shutdown() = 0;
	bool Register();// register the shader in vertex shader manager
	bool Unregister();
private:
	virtual void ShutdownShader() = 0;
};

