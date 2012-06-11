////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "RenderObject.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass:public RenderObject
{
private:
	// for model with no texture




public:
	ModelClass();
	ModelClass(const ModelClass&);
	virtual ~ModelClass();

	virtual bool Initialize(ID3D11Device*) = 0;
	//virtual bool Initialize(ID3D11Device* device, WCHAR* textureFilename);
	virtual void Shutdown() = 0;
	virtual void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX) = 0;
	
	virtual int GetIndexCount() = 0;
	bool IsTextured();
	virtual bool Update() = 0;
	//ID3D11ShaderResourceView* GetTexture();


protected:
	virtual bool InitializeBuffers(ID3D11Device*) = 0;
	virtual bool InitializeShaders(ID3D11Device*) = 0;
	virtual void ShutdownBuffers() = 0;
	virtual void RenderBuffers(ID3D11DeviceContext*) = 0;
	virtual bool LoadTexture(ID3D11Device*) = 0;
	virtual void ReleaseTexture() = 0;//call texture manager to release

protected:
	bool hasTexture;
};

#endif
