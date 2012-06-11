#ifndef _PERLINFIRE_H_
#define _PERLINFIRE_H_

#include <D3DX11tex.h>
#include <D3DX11.h>
#include <D3DX11core.h>
#include <D3DX11async.h>
#include <D3DX10math.h>
#include <D3Dx11effect.h>
#include "CameraManager.h"
#include "CameraClass.h"

#define MIN_SAMPLING_RATE   4
#define MAX_SAMPLING_RATE   64

#define DEFAULT_JITTER true
#define DEFAULT_SAMPLING_RATE 20
#define DEFAULT_SPEED 0.6f
#define DEFAULT_NOISE_SCALE 1.35f
#define DEFAULT_ROUGHNESS 3.20f
#define DEFAULT_SHAPE_SIZE 5.0f
#define DEFAULT_FREQUENCY1 1.0f
#define DEFAULT_FREQUENCY2 0.5f
#define DEFAULT_FREQUENCY3 0.25f
#define DEFAULT_FREQUENCY4 0.125f
#define DEFAULT_FREQUENCY5 0.0625f

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif 

class PerlinFire
{
private:
	// Direct3D 11 resources
	ID3DX11Effect * g_pEffect;

	// DX11 device and rendering related variables
	ID3D11InputLayout * g_pGeometryVertexLayout;

	// Shader variables and matrices

	ID3DX11EffectMatrixVariable * g_pmCubeViewMatrixVariable;
	ID3DX11EffectMatrixVariable * g_pmCubeProjMatrixVariable;
	ID3DX11EffectMatrixVariable * g_pmWorldViewProj;
	ID3DX11EffectVectorVariable * g_pvEyePos;
	ID3DX11EffectVectorVariable * g_pvLightPos;
	ID3DX11EffectScalarVariable * g_pfLightIntensity;
	ID3DX11EffectScalarVariable * g_pfStepSize;
	ID3DX11EffectScalarVariable * g_pfTime;
	ID3DX11EffectScalarVariable * g_pfNoiseScale;
	ID3DX11EffectScalarVariable * g_pfRoughness;
	ID3DX11EffectScalarVariable * g_pfFrequencyWeights;
	ID3DX11EffectScalarVariable * g_pbJitter;
	ID3DX11EffectScalarVariable * g_piCubeMapFace;

	// Textures and related shader resource views

	ID3D11Texture2D * g_pDepthBuffer;
	ID3D11ShaderResourceView * g_pDepthBufferSRV;
	ID3D11DepthStencilView * g_pDepthBufferDSV;

	ID3D11Texture2D * g_pFireTexture;
	ID3D11ShaderResourceView * g_pFireTextureSRV;

	ID3D11Texture2D * g_pNoiseTexture;
	ID3D11ShaderResourceView * g_pJitterTextureSRV;
	ID3D11ShaderResourceView * g_pPermTextureSRV;

	ID3DX11EffectShaderResourceVariable * g_ptxScreenDepth;
	ID3DX11EffectShaderResourceVariable * g_ptxSceneTexture;
	ID3DX11EffectShaderResourceVariable * g_ptxFireTex;
	ID3DX11EffectShaderResourceVariable * g_ptxJitterTex;
	ID3DX11EffectShaderResourceVariable * g_ptxPermTex;

	// Textures and views for shadow mapping

	ID3D11Texture2D * g_pCubeMapDepth;
	ID3D11DepthStencilView * g_pCubeMapDepthViewArray[6];
	ID3D11ShaderResourceView * g_pCubeMapTextureRV;
	ID3DX11EffectShaderResourceVariable * g_pCubeMapTextureVariable;

	// Effect techniques
	ID3DX11EffectTechnique * g_pCurrentTechnique;
	ID3DX11EffectTechnique * g_pPerlinFire3D;
	ID3DX11EffectTechnique * g_pPerlinFire4D;
	ID3DX11EffectTechnique * g_pPerlinFire3DFlow;
	ID3DX11EffectTechnique * g_pGeometryTechnique;
	ID3DX11EffectTechnique * g_pGeometryTechniqueAux;

	ID3D11Buffer * g_pBuffer;
	ID3D11Buffer * g_pIndex;
	// Fire parameters
	bool g_bJitter;
	int g_nSamplingRate;
	float g_fSpeed;
	float g_fNoiseScale;
	float g_fRoughness;
	float g_fShapeSize;
	float g_fFrequencyWeights [5];
	int g_CubeMapSize;
	float m_time;
	D3DXVECTOR3 FirePosition;

	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Nor;
		D3DXVECTOR2 Tex;
	};

public:

	PerlinFire();

	~PerlinFire();

	void SetVariables();

	bool LoadTexture2D(ID3D11Device * pd3dDevice, LPCWSTR fileName, ID3D11Texture2D ** tex, ID3D11ShaderResourceView ** texRV);

	HRESULT OnD3D11CreateDevice( ID3D11Device * pd3dDevice);

	void OnD3D11FrameRender( ID3D11DeviceContext* pd3dDevice,D3DXMATRIX mView, D3DXMATRIX mProj);

	void OnD3D11DestroyDevice();

	HRESULT LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect );
};

#endif