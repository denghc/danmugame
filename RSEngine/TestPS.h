#pragma once
#include "pixelshaderclass.h"
class TestPS :
	public PixelShaderClass
{
public:
	TestPS(void);
	virtual ~TestPS(void);
	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* psFilename, CHAR* entryFuncName);
	virtual bool InitializeConstantBuffer(ID3D11Device*);
	virtual bool SetRenderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*);
	virtual void Shutdown();
private:
	virtual void ShutdownShader();

private:
	ID3D11PixelShader* m_pixelShader;
	
};

