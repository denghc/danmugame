#ifndef _GAUSSIANMAIN_H_
#define _GAUSSIANMAIN_H_

#include <D3DX11tex.h>
#include <D3DX11.h>
#include <D3DX11core.h>
#include <D3DX11async.h>
#include <D3DX10math.h>
#include <D3Dx11effect.h>
#include <stdio.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif

class GaussianMain
{
private:

	// Render target texture for scene
	ID3D11Texture2D*            g_pTex_Scene;
	ID3D11RenderTargetView*     g_pRTV_Scene;
	ID3D11ShaderResourceView*   g_pSRV_Scene;

	// RW texture as intermediate and output buffer
	ID3D11Texture2D*            g_pTex_Output;
	ID3D11UnorderedAccessView*  g_pUAV_Output;
	ID3D11ShaderResourceView*   g_pSRV_Output;

	// Default render targets in the swap chain
	ID3D11RenderTargetView*		g_pRTV_Default;
	ID3D11DepthStencilView*		g_pDSV_Default;
	D3D11_VIEWPORT				g_VP_Default[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];

	// D3DX11 Effects
	ID3DX11Effect*				g_pFX_GaussianCol;
	ID3DX11Effect*				g_pFX_GaussianRow;
	ID3DX11Effect*				g_pFX_Render;

	// Background cubemap
	ID3D11ShaderResourceView*	g_pSRV_Background;

	// Stuff used for drawing the "full screen quad"
	struct SCREEN_VERTEX
	{
		D3DXVECTOR4 pos;
		D3DXVECTOR2 tex;
	};
	ID3D11Buffer*               g_pScreenQuadVB;
	ID3D11InputLayout*          g_pQuadLayout;

	//--------------------------------------------------------------------------------------
	// Default parameter values
	//--------------------------------------------------------------------------------------
	UINT	g_NumApproxPasses;
	UINT	g_MaxApproxPasses;

	BOOL	g_bColorBlur;
	BOOL	g_bUseD3DRefMode;

	// The number of per group threads is fixed to 128 for mono mode, 256 for color mode.
	// Do not change this value.
	UINT	g_ThreadsPerGroup;

public:
	float	g_FilterRadius;

	GaussianMain();

	~GaussianMain();

	void SetVariables();

	DXGI_SURFACE_DESC GetDXGIBackBufferSurfaceDesc();

	float CalculateBoxFilterWidth(float radius, int pass);

	HRESULT LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect );

	HRESULT CompileGaussianFilterEffects(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

	HRESULT OnD3D11CreateDevice( ID3D11Device* pd3dDevice);

	HRESULT CreateResources(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

	void DrawFullScreenQuad(ID3D11DeviceContext* pd3dImmediateContext, UINT ScreenWidth, UINT ScreenHeight);

	void RestoreDefaultStates(ID3D11DeviceContext* pd3dImmediateContext);

	void ApplyGaussianFilter(ID3D11DeviceContext* pd3dImmediateContext);

	void ShowImage(ID3D11DeviceContext* pd3dImmediateContext);

	void OnD3D11FrameRender1(ID3D11DeviceContext* pd3dImmediateContext);

	void OnD3D11FrameRender2(ID3D11DeviceContext* pd3dImmediateContext);

	void ReleaseResources();

	void OnD3D11DestroyDevice();

	void SetFR(int fr);
};
#endif