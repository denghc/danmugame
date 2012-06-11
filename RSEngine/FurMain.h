#ifndef _FURMAIN_H_
#define _FURMAIN_H_

#include <D3DX11tex.h>
#include <D3DX11.h>
#include <D3DX11core.h>
#include <D3DX11async.h>
#include <D3DX10math.h>
#include <d3dx11effect.h>
#include <stdio.h>
#include <vector>

#pragma warning(disable:4995) // avoird warning for "...was marked as #pragma deprecated"

using namespace std;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef MEDIAPATH
#define MEDIAPATH(x) L"Resource/FurTexture/" L#x	
#endif

#include "FurMain.h"

class FurMain
{
private:
	//--------------------------------------------------------------------------------------
	// Global Variables
	//--------------------------------------------------------------------------------------
	ID3DX11Effect*              g_pEffect;       // D3DX effect interface
	ID3D11InputLayout*          g_pVertexLayout; // Vertex Layout for mesh
	ID3D11InputLayout*          g_pVertexLayoutArrow; // Vertex Layout for arrow
	bool                        g_bShowHelp;    // show help menu

	ID3D10Buffer*               g_MeshLineAdjIB;
	ID3D10Buffer*               g_MeshVB;
	int                         g_MeshLineAdjIndices;

	//effect variables
	ID3DX11EffectMatrixVariable* g_pWorldVariable;
	ID3DX11EffectMatrixVariable* g_pWorldViewProj;
	ID3DX11EffectVectorVariable* g_pEyeVec;
	ID3DX11EffectVectorVariable* g_pLightVec; 
	ID3DX11EffectScalarVariable* g_pNumShellsNumberShaderVariable;
	ID3DX11EffectScalarVariable* g_pShellIncrementShaderVariable;
	ID3DX11EffectScalarVariable* g_pMaxOpacityShaderVariable;
	ID3DX11EffectScalarVariable* g_pCombStrengthShaderVariable;
	ID3DX11EffectVectorVariable* g_pCombVectorShaderVariable;

	ID3D11RasterizerState * g_pRasterStateWireFrame;
	ID3D11RasterizerState * g_pRasterStateSolid;
	ID3D11RasterizerState * g_pRasterStateSolidNoCull;

	//techniques
	ID3DX11EffectTechnique*  g_pTechniqueRenderMesh;

	D3DXVECTOR3 vecEye;
	D3DXVECTOR3 vecLight;
	D3DXVECTOR3 eyeInObjectSpace;
	D3DXVECTOR3 lightInObjectSpace;
	D3DXVECTOR3 g_modelCentroid;
	D3DXMATRIX  g_World;

	bool                        g_bToggleWireFrame;
	float                       g_shellIncrement;
	bool                        g_bShowShells;
	bool                        g_bShowFins;
	float                       g_maxOpacity;
	D3DXVECTOR3                 g_combVector;
	float                       g_combStrength;

	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
	};
public:
	int                         g_numShells;
	ID3DX11EffectScalarVariable* g_pShellNumberShaderVariable;
	ID3DX11EffectTechnique*  g_pTechniqueRenderShells;
	ID3DX11EffectTechnique*  g_pTechniqueRenderFinsLineAdj;

	FurMain();

	~FurMain();

	void SetVariables();

	HRESULT OnD3D11CreateDevice( ID3D11Device* pd3dDevice);

	void OnD3D11FrameRender1(ID3D11DeviceContext* pd3dImmediateContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void OnD3D11DestroyDevice();

	HRESULT loadTextureFromFile(LPCWSTR file,LPCSTR shaderTextureName, ID3D11Device* pd3dDevice);

	HRESULT LoadTextureArray( ID3D11Device* pd3dDevice, LPCTSTR* szTextureNames, int iNumTextures, ID3D11Texture2D** ppTex2D, ID3D11ShaderResourceView** ppSRV);

	void vectorMatrixMultiply(D3DXVECTOR3* vecOut,const D3DXMATRIX matrix,const D3DXVECTOR3 vecIn);

	HRESULT LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect );

};


#endif