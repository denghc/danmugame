#include "LightningMain.h"

LightingMain::LightingMain()
{
	SetVariables();
}

LightingMain::~LightingMain()
{
	OnD3D11DestroyDevice();
}

void LightingMain::SetVariables()
{
	g_arena = NULL;
	m_time = 0.0f;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependent on the back buffer
//--------------------------------------------------------------------------------------
HRESULT LightingMain::OnD3D11CreateDevice( ID3D11Device* pd3dDevice)
{
	DXGI_SAMPLE_DESC desc;
	desc.Count = 1;
	desc.Quality = 0;
	
	g_arena = new Arena(pd3dDevice,desc);

	//------------------------------------
	// Resized Swap Chain
	//------------------------------------
	//g_arena->RenderTargetResize(800, 600,  , DXUTGetD3D10DepthStencilView());

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void LightingMain::OnD3D11FrameRender(D3DXMATRIX viewMa, D3DXMATRIX projMa)
{
	const float dt = 1.0f/60.0f;
	m_time += dt;
	
	g_arena->Matrices(viewMa, projMa);
	g_arena->Time(m_time,dt);
	g_arena->Render();
}

//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void LightingMain::OnD3D11DestroyDevice()
{
	delete g_arena;

	//D3DX10UnsetAllDeviceObjects(DXUTGetD3D10Device());
}

void LightingMain::SetLightningRendererRTV_DSV(ID3D11DepthStencilView* m_scene_depth_stencil_view,ID3D11RenderTargetView*	m_scene_render_target_view)
{
	g_arena->m_lightning_renderer.m_scene_depth_stencil_view = m_scene_depth_stencil_view;
	g_arena->m_lightning_renderer.m_scene_render_target_view = m_scene_render_target_view;
}