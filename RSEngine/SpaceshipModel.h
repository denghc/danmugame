#pragma once
#include "objmodelclass.h"
#include "GameLogic.h"
#include "ObjModelClass.h"
// class for rendering .obj format file
#include "ObjLoader.h"
#include "Structures.h"
#include "PixelShaderClass.h"
#include "VertexShaderClass.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "TextureClass.h"
#include <string>
#include <d3dx11effect.h>
#include <D3Dcompiler.h>
#include "FurMain.h"
#include "SoftParticles.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

class SpaceshipModel :
	public ObjModelClass
{
public:
	SpaceshipModel(char* objFileName);
	SpaceshipModel(void);
	~SpaceshipModel(void);
	virtual bool Initialize(ID3D11Device*);
	virtual void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	virtual bool Update();
	void UpdatePosition(float x, float y, float z);// use x, y, z to update world matrix

	void RenderMesh(ID3D11DeviceContext*);
private:
	virtual bool InitializeShaders(ID3D11Device* device);
	D3DXMATRIX m_backWorldMatrix;// already done scaling and rotation, only need to be transformed
	//
	ID3D11BlendState* m_pAlphaBlendState;
	FurMain* m_furMain;
	SoftParticles* m_softParticles;
};

