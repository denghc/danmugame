#pragma once
#include "pixelshaderclass.h"
class SpritePS :
	public PixelShaderClass
{
public:
	SpritePS(string tag);
	SpritePS(void);
	~SpritePS(void);
public:

	virtual bool Initialize(ID3D11Device*, HWND, WCHAR* vsFilename, CHAR* entryFuncName);
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, CHAR* entryFuncName);
	virtual bool InitializeConstantBuffer(ID3D11Device*);
	virtual bool SetRenderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*);
	virtual void Shutdown();
protected:
	virtual void ShutdownShader();
	ID3D11PixelShader* m_pixelShader;
	ID3D11SamplerState* m_colorState;
};

