#pragma once
#include "geometryshaderclass.h"
class BSTestGS :
	public GeometryShaderClass
{
public:
	BSTestGS();
	BSTestGS(string tag);
	~BSTestGS(void);
	virtual bool Initialize(ID3D11Device*, HWND, WCHAR* psFilename, CHAR* entryFuncName);
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* psFilename, CHAR* entryFuncName);
	virtual bool InitializeConstantBuffer(ID3D11Device*);
	virtual bool SetRenderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*);
	virtual void Shutdown();




private:
	virtual void ShutdownShader();

private:
	ID3D11GeometryShader* m_geometryShader;
	ID3D11Buffer* m_miscBuffer;
};

