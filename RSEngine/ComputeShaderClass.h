#pragma once
#include "shaderclass.h"
class ComputeShaderClass :
	public ShaderClass
{
public:
	ComputeShaderClass();
	ComputeShaderClass(string tag);
	virtual ~ComputeShaderClass(void);

public:
	virtual void Shutdown(){};
	virtual bool Initialize(ID3D11Device*, HWND){return true;};
	bool Register();// register the shader in shader manager
private:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, CHAR*){return true;};//first wchar is the file path, 
	//second is entry function
	virtual bool InitializeConstantBuffer(ID3D11Device*){return true;};// subclass must override this function

	virtual void ShutdownShader(){};


	virtual bool SetShaderParameters(){return true;};
	//void RenderShader(ID3D11DeviceContext*, int);
};

