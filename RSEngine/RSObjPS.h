#pragma once
#include "pixelshaderclass.h"
class RSObjPS :
	public PixelShaderClass
{
public:
	RSObjPS(std::string tag);
	RSObjPS(void);
	~RSObjPS(void);
	virtual bool Initialize(ID3D11Device*, HWND, WCHAR* vsFilename, CHAR* entryFuncName);
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, CHAR* entryFuncName);
	virtual bool InitializeConstantBuffer(ID3D11Device*);
	virtual bool SetRenderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*);
	virtual void Shutdown();

private:
	virtual void ShutdownShader();
	ID3D11PixelShader* m_pixelShader;
	ID3D11SamplerState* m_samplerState;
};

