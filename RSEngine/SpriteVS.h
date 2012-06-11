#pragma once
#include "vertexshaderclass.h"
class SpriteVS :
	public VertexShaderClass
{
public:
	SpriteVS(string tag);
	SpriteVS(void);
	~SpriteVS(void);
public:
	virtual bool Initialize(ID3D11Device*, HWND, WCHAR*, CHAR*);
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, CHAR* entryFuncName);
	virtual bool InitializeConstantBuffer(ID3D11Device*);
	virtual bool SetRenderParameters(ID3D11DeviceContext* device, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	virtual void Shutdown();
protected:

	virtual void ShutdownShader();
	virtual void ShutdownConstantBuffer();
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* viewCB_;
	ID3D11Buffer* projCB_;
	ID3D11Buffer* worldCB_;
};

