#ifndef _SOFTPARTICLES_H_
#define _SOFTPARTICLES_H_

#include <D3DX11tex.h>
#include <D3DX11.h>
#include <D3DX11core.h>
#include <D3DX11async.h>
#include <D3DX10math.h>
#include <D3Dx11effect.h>
#include "CameraManager.h"
#include "CameraClass.h"

#define MAX_PARTICLES 500

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif 

class SoftParticles
{
private:
	struct CHAR4
	{
		char x,y,z,w;
	};

	struct PARTICLE_VERTEX
	{
		D3DXVECTOR3	 Pos;
		D3DXVECTOR3  Vel;
		float		 Life;
		float		 Size;
	};

	PARTICLE_VERTEX*		g_pCPUParticles;
	DWORD*					g_pCPUParticleIndices;
	float*					g_pParticleDepthArray;

	ID3DX11Effect*           g_pEffect11;
	ID3D11InputLayout*      g_pParticleVertexLayout;

	ID3D11Texture2D*		  g_pDepthStencilTexture;
	ID3D11DepthStencilView*	  g_pDepthStencilDSV;
	ID3D11ShaderResourceView* g_pDepthStencilSRV;

	ID3D11Buffer*           g_pParticleVB;
	ID3D11Buffer*           g_pParticleIB;
	ID3D11ShaderResourceView* g_pParticleTexRV;
	ID3D11Texture3D*        g_pNoiseVolume;
	ID3D11ShaderResourceView* g_pNoiseVolumeRV;
	ID3D11Texture2D*        g_pColorGradTexture;
	ID3D11ShaderResourceView* g_pColorGradTexRV;

	ID3DX11EffectTechnique*  g_pRenderBillboardParticlesHard;
	ID3DX11EffectTechnique*  g_pRenderBillboardParticlesODepth;
	ID3DX11EffectTechnique*  g_pRenderBillboardParticlesSoft;
	ID3DX11EffectTechnique*  g_pRenderVolumeParticlesHard;
	ID3DX11EffectTechnique*  g_pRenderVolumeParticlesSoft;
	ID3DX11EffectTechnique*  g_pRenderBillboardParticlesODepthSoft;
	ID3DX11EffectTechnique*  g_pRenderVolumeParticlesSoftMSAA;
	ID3DX11EffectTechnique*  g_pRenderVolumeParticlesHardMSAA;
	ID3DX11EffectTechnique*  g_pRenderBillboardParticlesSoftMSAA;
	ID3DX11EffectTechnique*  g_pRenderBillboardParticlesODepthSoftMSAA;
	ID3DX11EffectMatrixVariable* g_pmWorldViewProj;
	ID3DX11EffectMatrixVariable* g_pmWorldView;
	ID3DX11EffectMatrixVariable* g_pmWorld;
	ID3DX11EffectMatrixVariable* g_pmInvView;
	ID3DX11EffectMatrixVariable* g_pmInvProj;
	ID3DX11EffectScalarVariable* g_pfFadeDistance;
	ID3DX11EffectScalarVariable* g_pfSizeZScale;
	ID3DX11EffectVectorVariable* g_pvViewLightDir1;
	ID3DX11EffectVectorVariable* g_pvViewLightDir2;
	ID3DX11EffectVectorVariable* g_pvWorldLightDir1;
	ID3DX11EffectVectorVariable* g_pvWorldLightDir2;
	ID3DX11EffectVectorVariable* g_pvEyePt;
	ID3DX11EffectVectorVariable* g_pvViewDir;
	ID3DX11EffectVectorVariable* g_pvOctaveOffsets;
	ID3DX11EffectVectorVariable* g_pvScreenSize;
	ID3DX11EffectShaderResourceVariable* g_pDiffuseTex;
	ID3DX11EffectShaderResourceVariable* g_pNormalTex;
	ID3DX11EffectShaderResourceVariable* g_pColorGradient;
	ID3DX11EffectShaderResourceVariable* g_pVolumeDiffTex;
	ID3DX11EffectShaderResourceVariable* g_pVolumeNormTex;
	ID3DX11EffectShaderResourceVariable* g_pDepthTex;
	ID3DX11EffectShaderResourceVariable* g_pDepthMSAATex;

	INT g_iWidth;
	INT g_iHeight;
	INT g_iSampleCount;

	enum PARTICLE_TECHNIQUE
	{
		PT_VOLUME_SOFT = 0x0,
		PT_VOLUME_HARD,
		PT_BILLBOARD_ODEPTHSOFT,
		PT_BILLBOARD_ODEPTH,
		PT_BILLBOARD_SOFT,
		PT_BILLBOARD_HARD
	};

	float g_fFadeDistance;
	float g_fParticleLifeSpan;
	float g_fEmitRate;

	float g_ParticleVel;
	float g_fParticleMaxSize;
	float g_fParticleMinSize;
	bool  g_bAnimateParticles;

	PARTICLE_TECHNIQUE g_ParticleTechnique;
	D3DXVECTOR3 g_vLightDir1;
	D3DXVECTOR3 g_vLightDir2;

	static int iFrame;
	float m_time;

public:
	SoftParticles();

	~SoftParticles();

	void SetVariables();

	HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice);

	void OnD3D11FrameRender(ID3D11DeviceContext* context, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,D3DXVECTOR3 spaceshipPos);

	void OnD3D11DestroyDevice();

	float RPercent();

	void EmitParticle( PARTICLE_VERTEX* pParticle );

	HRESULT CreateParticleBuffers( ID3D11Device* pd3dDevice );

	float GetDensity( int x, int y, int z, CHAR4* pTexels, UINT VolumeSize );

	void SetNormal( D3DXVECTOR3 Normal, int x, int y, int z, CHAR4* pTexels, UINT VolumeSize );

	HRESULT CreateNoiseVolume( ID3D11Device* pd3dDevice, UINT VolumeSize );

	void QuickDepthSort(DWORD* indices, float* depths, int lo, int hi);

	void SortParticleBuffer( D3DXVECTOR3 vEye, D3DXVECTOR3 vDir );

	void AdvanceParticles( double fTime, float fTimeDelta );

	void UpdateParticleBuffers( ID3D11DeviceContext* pd3dDevice );

	HRESULT LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect );
};

#endif


