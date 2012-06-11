////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

#include "D3DXUtils.h"
using namespace std;



////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderClass
////////////////////////////////////////////////////////////////////////////////
class ShaderClass
{
private:


public:
	ShaderClass();
	ShaderClass(string tag);
	ShaderClass(const ShaderClass&);
	virtual ~ShaderClass();
	virtual bool Initialize(ID3D11Device*, HWND);
	virtual void Shutdown();

	string GetTag();



	//bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);//first wchar is the file path, 
																	//second is entry function
	virtual bool InitializeConstantBuffer(ID3D11Device*);// subclass must override this function

	virtual void ShutdownShader();
	

	virtual bool SetRenderParameters();
	//void RenderShader(ID3D11DeviceContext*, int);

protected:
	
	string m_tag;

private:
// 	ID3D11VertexShader* m_vertexShader;
// 	ID3D11PixelShader* m_pixelShader;
// 	ID3D11InputLayout* m_layout;
// 	ID3D11Buffer* m_matrixBuffer;
// 	ID3D11SamplerState* m_sampleState;	//maybe there is a texture
};

#endif