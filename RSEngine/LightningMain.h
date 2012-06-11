#pragma once

#include "Utility.h"
using namespace Utility;
#include "EffectVariable.h"
#include "Geometry.h"
#include "Arena.h"
using namespace LightningDemo;

#define MAX_SPRITES 500

class LightingMain
{
private:
	Arena*	g_arena;
	float m_time;

public:
	LightingMain();
	~LightingMain();

	void SetVariables();

	HRESULT OnD3D11CreateDevice( ID3D11Device* pd3dDevice);

	void OnD3D11FrameRender(D3DXMATRIX viewMa,D3DXMATRIX projMa);

	void OnD3D11DestroyDevice();

	void SetLightningRendererRTV_DSV(ID3D11DepthStencilView* m_scene_depth_stencil_view,ID3D11RenderTargetView*	m_scene_render_target_view);

};