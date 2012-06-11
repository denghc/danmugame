#include "GaussianMain.h"

GaussianMain::GaussianMain()
{
	SetVariables();
}

GaussianMain::~GaussianMain()
{
	OnD3D11DestroyDevice();
}

void GaussianMain::SetVariables()
{
	g_NumApproxPasses = 3;
	g_MaxApproxPasses = 8;
	g_FilterRadius = 30;

	g_bColorBlur = TRUE;
	g_bUseD3DRefMode = FALSE;

	g_ThreadsPerGroup = 256;

	g_pTex_Scene = NULL;
	g_pRTV_Scene = NULL;
	g_pSRV_Scene = NULL;
	g_pTex_Output = NULL;
	g_pUAV_Output = NULL;
	g_pSRV_Output = NULL;
	g_pRTV_Default = NULL;
	g_pDSV_Default = NULL;
	g_pFX_GaussianCol = NULL;
	g_pFX_GaussianRow = NULL;
	g_pFX_Render = NULL;
	g_pSRV_Background = NULL;
	g_pScreenQuadVB = NULL;
	g_pQuadLayout = NULL;
}

DXGI_SURFACE_DESC GaussianMain::GetDXGIBackBufferSurfaceDesc()
{
	DXGI_SURFACE_DESC desc;
	desc.Width = 800;
	desc.Height = 600;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	return desc;
}

float GaussianMain::CalculateBoxFilterWidth(float radius, int pass)
{
	// Calculate standard deviation according to cutoff width

	// We use sigma*3 as the width of filtering window
	float sigma = radius / 3.0f;

	// The width of the repeating box filter
	float box_width = sqrt(sigma * sigma * 12.0f / pass + 1);

	return box_width;
}

HRESULT GaussianMain::LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect )
{
	HRESULT hr = S_OK;

	// Compile the effect file
	ID3DBlob* pBlobFX = NULL;
	ID3DBlob* pErrorBlob = NULL;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, NULL, "fx_5_0", NULL, NULL, NULL, &pBlobFX, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		char* err = (char*)pErrorBlob->GetBufferPointer();
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}

	// Create the effect
	hr = D3DX11CreateEffectFromMemory(pBlobFX->GetBufferPointer(), pBlobFX->GetBufferSize(), 0, pd3dDevice, ppEffect);
	if( FAILED(hr) )
	{
		OutputDebugString( TEXT("Failed to load effect file.") );
		return hr;
	}

	SAFE_RELEASE(pBlobFX);
	SAFE_RELEASE(pErrorBlob);

	return S_OK;
}

HRESULT GaussianMain::CompileGaussianFilterEffects(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr = S_OK;

	// The compute shaders for column and row filtering are created from the same
	// effect file, separated by different macro definitions.

	// Find the file
	WCHAR str[] = L"gaussian_color_cs.hlsl";
	D3D10_SHADER_MACRO defines[8];

	// Software emulation mode requires different shader code
	char str_ref_mode[8];
	sprintf_s(str_ref_mode, "%u", g_bUseD3DRefMode ? 1 : 0);
	defines[0].Name = "USE_D3D_REF_MODE";
	defines[0].Definition = str_ref_mode;

	// Input image height
	UINT num_rows = pBackBufferSurfaceDesc->Height;
	char str_num_rows[8];
	sprintf_s(str_num_rows, "%u", num_rows);
	defines[1].Name = "NUM_IMAGE_ROWS";
	defines[1].Definition = str_num_rows;

	// Input image width
	UINT num_cols = pBackBufferSurfaceDesc->Width;
	char str_num_cols[8];
	sprintf_s(str_num_cols, "%u", num_cols);
	defines[2].Name = "NUM_IMAGE_COLS";
	defines[2].Definition = str_num_cols;


	// ------------------------ Column filtering shader -----------------------

	// Row scan or column scan
	defines[3].Name = "SCAN_COL_PASS";
	defines[3].Definition = "1";

	// Allocate shared memory according to the size of input image
	char str_data_length[8];
	sprintf_s(str_data_length, "%u", max(pBackBufferSurfaceDesc->Height, g_ThreadsPerGroup * 2));
	defines[4].Name = "SCAN_SMEM_SIZE";
	defines[4].Definition = str_data_length;

	// Number of texels per thread handling
	UINT texels_per_thread = (pBackBufferSurfaceDesc->Height + g_ThreadsPerGroup - 1) / g_ThreadsPerGroup;
	char str_texels_per_thread[8];
	sprintf_s(str_texels_per_thread, "%u", texels_per_thread);
	defines[5].Name = "TEXELS_PER_THREAD";
	defines[5].Definition = str_texels_per_thread;

	char str_threads_per_group[8];
	sprintf_s(str_threads_per_group, "%u", g_ThreadsPerGroup);
	defines[6].Name = "THREADS_PER_GROUP";
	defines[6].Definition = str_threads_per_group;

	defines[7].Name = NULL;
	defines[7].Definition = NULL;

	// Compile the effect file
	ID3DBlob* pBlobFX = NULL;
	ID3DBlob* pErrorBlob = NULL;
	hr = D3DX11CompileFromFile(str, defines, NULL, NULL, "fx_5_0", NULL, NULL, NULL, &pBlobFX, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if(pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}

	// Create the effect for column Gaussian filtering
	D3DX11CreateEffectFromMemory(pBlobFX->GetBufferPointer(), pBlobFX->GetBufferSize(), 0, pd3dDevice, &g_pFX_GaussianCol);

	SAFE_RELEASE(pBlobFX);
	SAFE_RELEASE(pErrorBlob);

	// ------------------------- Row filtering shader -------------------------

	defines[3].Name = "SCAN_COL_PASS";
	defines[3].Definition = "0";

	// Allocate shared memory according to the size of input image
	sprintf_s(str_data_length, "%u", max(pBackBufferSurfaceDesc->Width, g_ThreadsPerGroup * 2));
	defines[4].Name = "SCAN_SMEM_SIZE";
	defines[4].Definition = str_data_length;

	// Number of texels per thread handling
	texels_per_thread = (pBackBufferSurfaceDesc->Width + g_ThreadsPerGroup - 1) / g_ThreadsPerGroup;
	sprintf_s(str_texels_per_thread, "%u", texels_per_thread);
	defines[5].Name = "TEXELS_PER_THREAD";
	defines[5].Definition = str_texels_per_thread;

	hr = D3DX11CompileFromFile(str, defines, NULL, NULL, "fx_5_0", NULL, NULL, NULL, &pBlobFX, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if(pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}

	// Create the effect for row Gaussian filtering
	D3DX11CreateEffectFromMemory(pBlobFX->GetBufferPointer(), pBlobFX->GetBufferSize(), 0, pd3dDevice, &g_pFX_GaussianRow);

	SAFE_RELEASE(pBlobFX);
	SAFE_RELEASE(pErrorBlob);


	return hr;
}

HRESULT GaussianMain::OnD3D11CreateDevice( ID3D11Device* pd3dDevice)
{
	//HRESULT hr;

	ID3D11DeviceContext* pd3dImmediateContext;
	pd3dDevice->GetImmediateContext(&pd3dImmediateContext);

	// Load effect
	LoadEffectFromFile(pd3dDevice, L"gaussian_vs_ps.hlsl", &g_pFX_Render);

	// Layout for full screen quad rendering
	const D3D11_INPUT_ELEMENT_DESC quad_layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	D3DX11_PASS_DESC pass_desc;
	g_pFX_Render->GetTechniqueByName("Tech_ShowImage")->GetPassByName("Pass_ShowMonoImage")->GetDesc(&pass_desc);
	pd3dDevice->CreateInputLayout(quad_layout, 2, pass_desc.pIAInputSignature, pass_desc.IAInputSignatureSize, &g_pQuadLayout);

	// Vertex buffer for full screen quad rendering
	SCREEN_VERTEX svQuad[4];
	svQuad[0].pos = D3DXVECTOR4( -1.0f, 1.0f, 0.5f, 1.0f );
	svQuad[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	svQuad[1].pos = D3DXVECTOR4( 1.0f, 1.0f, 0.5f, 1.0f );
	svQuad[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
	svQuad[2].pos = D3DXVECTOR4( -1.0f, -1.0f, 0.5f, 1.0f );
	svQuad[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
	svQuad[3].pos = D3DXVECTOR4( 1.0f, -1.0f, 0.5f, 1.0f );
	svQuad[3].tex = D3DXVECTOR2( 1.0f, 1.0f );

	D3D11_BUFFER_DESC vbdesc =
	{
		4 * sizeof( SCREEN_VERTEX ),
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_VERTEX_BUFFER,
		0,
		0
	};
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = svQuad;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	pd3dDevice->CreateBuffer( &vbdesc, &InitData, &g_pScreenQuadVB );

	//---------------------------------
	//resize swap chain
	//---------------------------------

	CreateResources(pd3dDevice, &GetDXGIBackBufferSurfaceDesc());

	// Save render target and viewport
	pd3dImmediateContext->OMGetRenderTargets(1, &g_pRTV_Default, &g_pDSV_Default);
	UINT nViewPorts = 1;
	pd3dImmediateContext->RSGetViewports(&nViewPorts, g_VP_Default);
	SAFE_RELEASE(pd3dImmediateContext);

	//----------------------------------
	// frame move
	//----------------------------------
	// Compile the shaders if haven't.
	SAFE_RELEASE(g_pFX_GaussianCol);
	SAFE_RELEASE(g_pFX_GaussianRow);
	CompileGaussianFilterEffects(pd3dDevice, &GetDXGIBackBufferSurfaceDesc());

	return S_OK;
}

HRESULT GaussianMain::CreateResources(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	// Render target texture for scene rendering
	D3D11_TEXTURE2D_DESC tex_desc;
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;

	ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));
	ZeroMemory(&rtv_desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	ZeroMemory(&uav_desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	tex_desc.ArraySize = 1;
	tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.Format = g_bColorBlur ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_R32_FLOAT;
	tex_desc.Width = pBackBufferSurfaceDesc->Width;
	tex_desc.Height = pBackBufferSurfaceDesc->Height;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	V_RETURN(pd3dDevice->CreateTexture2D(&tex_desc, NULL, &g_pTex_Scene));

	rtv_desc.Format = tex_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	V_RETURN(pd3dDevice->CreateRenderTargetView(g_pTex_Scene, &rtv_desc, &g_pRTV_Scene));

	srv_desc.Format = tex_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;
	V_RETURN(pd3dDevice->CreateShaderResourceView(g_pTex_Scene, &srv_desc, &g_pSRV_Scene));

	// RW texture for output
	tex_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.Format = g_bColorBlur ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	V_RETURN(pd3dDevice->CreateTexture2D(&tex_desc, NULL, &g_pTex_Output));

	uav_desc.Format = tex_desc.Format;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uav_desc.Texture2D.MipSlice = 0;
	V_RETURN(pd3dDevice->CreateUnorderedAccessView(g_pTex_Output, &uav_desc, &g_pUAV_Output));

	srv_desc.Format = tex_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;
	V_RETURN(pd3dDevice->CreateShaderResourceView(g_pTex_Output, &srv_desc, &g_pSRV_Output));

	return S_OK;
}

void GaussianMain::DrawFullScreenQuad(ID3D11DeviceContext* pd3dImmediateContext, UINT ScreenWidth, UINT ScreenHeight)
{
	// Setup the viewport to match the backbuffer
	D3D11_VIEWPORT vp;
	vp.Width = (float)ScreenWidth;
	vp.Height = (float)ScreenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pd3dImmediateContext->RSSetViewports(1, &vp);

	UINT strides = sizeof(SCREEN_VERTEX);
	UINT offsets = 0;
	ID3D11Buffer* pBuffers[1] = {g_pScreenQuadVB};

	pd3dImmediateContext->IASetInputLayout(g_pQuadLayout);
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pBuffers, &strides, &offsets);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pd3dImmediateContext->Draw( 4, 0 );
}

void GaussianMain::RestoreDefaultStates(ID3D11DeviceContext* pd3dImmediateContext)
{
	pd3dImmediateContext->ClearState();

	// Render target
	ID3D11RenderTargetView* rtv_array[1] = {g_pRTV_Default};
	pd3dImmediateContext->OMSetRenderTargets(1, rtv_array, g_pDSV_Default);

	// Viewport
	pd3dImmediateContext->RSSetViewports(1, g_VP_Default);
}

void GaussianMain::ApplyGaussianFilter(ID3D11DeviceContext* pd3dImmediateContext)
{
	D3D11_TEXTURE2D_DESC tex_desc;
	g_pTex_Scene->GetDesc(&tex_desc);

	float box_width = CalculateBoxFilterWidth(g_FilterRadius, g_NumApproxPasses);
	float half_box_width = box_width * 0.5f;
	float frac_half_box_width = (half_box_width + 0.5f) - (int)(half_box_width + 0.5f);
	float inv_frac_half_box_width = 1.0f - frac_half_box_width;
	float rcp_box_width = 1.0f / box_width;


	// Step 1. Vertical passes: Each thread group handles a colomn in the image

	ID3DX11EffectTechnique* pTech = g_pFX_GaussianCol->GetTechniqueByName("Tech_GaussianFilter");

	// Input texture
	g_pFX_GaussianCol->GetVariableByName("g_texInput")->AsShaderResource()->SetResource(g_pSRV_Scene);
	// Output texture
	g_pFX_GaussianCol->GetVariableByName("g_rwtOutput")->AsUnorderedAccessView()->SetUnorderedAccessView(g_pUAV_Output);

	g_pFX_GaussianCol->GetVariableByName("g_NumApproxPasses")->AsScalar()->SetInt(g_NumApproxPasses - 1);
	g_pFX_GaussianCol->GetVariableByName("g_HalfBoxFilterWidth")->AsScalar()->SetFloat(half_box_width);
	g_pFX_GaussianCol->GetVariableByName("g_FracHalfBoxFilterWidth")->AsScalar()->SetFloat(frac_half_box_width);
	g_pFX_GaussianCol->GetVariableByName("g_InvFracHalfBoxFilterWidth")->AsScalar()->SetFloat(inv_frac_half_box_width);
	g_pFX_GaussianCol->GetVariableByName("g_RcpBoxFilterWidth")->AsScalar()->SetFloat(rcp_box_width);

	// Select pass
	ID3DX11EffectPass* pPass = g_bColorBlur ? pTech->GetPassByName("Pass_GaussianColor") : pTech->GetPassByName("Pass_GaussianMono");

	pPass->Apply(0, pd3dImmediateContext);
	pd3dImmediateContext->Dispatch(tex_desc.Width, 1, 1);

	// Unbound CS resource and output
	ID3D11ShaderResourceView* srv_array[] = {NULL, NULL, NULL, NULL};
	pd3dImmediateContext->CSSetShaderResources(0, 4, srv_array);
	ID3D11UnorderedAccessView* uav_array[] = {NULL, NULL, NULL, NULL};
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 4, uav_array, NULL);


	// Step 2. Horizontal passes: Each thread group handles a row in the image

	pTech = g_pFX_GaussianRow->GetTechniqueByName("Tech_GaussianFilter");

	// Input texture
	g_pFX_GaussianRow->GetVariableByName("g_texInput")->AsShaderResource()->SetResource(g_pSRV_Scene);
	// Output texture
	g_pFX_GaussianRow->GetVariableByName("g_rwtOutput")->AsUnorderedAccessView()->SetUnorderedAccessView(g_pUAV_Output);

	g_pFX_GaussianRow->GetVariableByName("g_NumApproxPasses")->AsScalar()->SetInt(g_NumApproxPasses - 1);
	g_pFX_GaussianRow->GetVariableByName("g_HalfBoxFilterWidth")->AsScalar()->SetFloat(half_box_width);
	g_pFX_GaussianRow->GetVariableByName("g_FracHalfBoxFilterWidth")->AsScalar()->SetFloat(frac_half_box_width);
	g_pFX_GaussianRow->GetVariableByName("g_InvFracHalfBoxFilterWidth")->AsScalar()->SetFloat(inv_frac_half_box_width);
	g_pFX_GaussianRow->GetVariableByName("g_RcpBoxFilterWidth")->AsScalar()->SetFloat(rcp_box_width);

	// Select pass
	pPass = g_bColorBlur ? pTech->GetPassByName("Pass_GaussianColor") : pTech->GetPassByName("Pass_GaussianMono");

	pPass->Apply(0, pd3dImmediateContext);
	pd3dImmediateContext->Dispatch(tex_desc.Height, 1, 1);

	// Unbound CS resource and output
	pd3dImmediateContext->CSSetShaderResources(0, 4, srv_array);
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 4, uav_array, NULL);
}

void GaussianMain::ShowImage(ID3D11DeviceContext* pd3dImmediateContext)
{
	// Output to default render target
	ID3D11RenderTargetView* rtv_array[1] = {g_pRTV_Default};
	pd3dImmediateContext->OMSetRenderTargets(1, rtv_array, g_pDSV_Default);
	pd3dImmediateContext->RSSetViewports(1, g_VP_Default);

	ID3DX11EffectTechnique* pTech = g_pFX_Render->GetTechniqueByName("Tech_ShowImage");
	ID3DX11EffectPass* pPass = g_bColorBlur ? pTech->GetPassByName("Pass_ShowColorImage") : pTech->GetPassByName("Pass_ShowMonoImage");

	if (g_bColorBlur)
		g_pFX_Render->GetVariableByName("g_texColorInput")->AsShaderResource()->SetResource(g_pSRV_Output);
	else
		g_pFX_Render->GetVariableByName("g_texMonoInput")->AsShaderResource()->SetResource(g_pSRV_Output);

	UINT strides = sizeof(SCREEN_VERTEX);
	UINT offsets = 0;
	ID3D11Buffer* pBuffers[1] = {g_pScreenQuadVB};

	pd3dImmediateContext->IASetInputLayout(g_pQuadLayout);
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pBuffers, &strides, &offsets);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pPass->Apply(0, pd3dImmediateContext);
	pd3dImmediateContext->Draw( 4, 0 );
}

void GaussianMain::SetFR(int fr)
{
	g_FilterRadius = (float)(fr);	
}

void GaussianMain::OnD3D11FrameRender1(ID3D11DeviceContext* pd3dImmediateContext)
{	
	// Clear render targets
	float ClearColor[4] = {0, 0, 0, 0};
	//pd3dImmediateContext->ClearRenderTargetView(g_pRTV_Default, ClearColor);
	//pd3dImmediateContext->ClearDepthStencilView(g_pDSV_Default, D3D11_CLEAR_DEPTH, 1.0, 0);

	// Set FP16x4 texture as the output RT
	ID3D11RenderTargetView* rtv_array[1];
	rtv_array[0] = g_pRTV_Scene;
	pd3dImmediateContext->OMSetRenderTargets(1, rtv_array, g_pDSV_Default);
	pd3dImmediateContext->ClearRenderTargetView(g_pRTV_Scene, ClearColor);

// 	g_FilterRadius += 1.0f;
// 	if(g_FilterRadius >= 180.0f) g_FilterRadius = 0.0f;

}

void GaussianMain::OnD3D11FrameRender2(ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11RenderTargetView* rtv_array[1];
	// Resore default render target so that g_pRTV_Downscale can be unbound.
	rtv_array[0] = g_pRTV_Default;
	pd3dImmediateContext->OMSetRenderTargets(1, rtv_array, g_pDSV_Default);

	// Perform Gaussian filtering with repeated box filters
	ApplyGaussianFilter(pd3dImmediateContext);

	// Display the filtering result
	ShowImage(pd3dImmediateContext);

	// The D3D states must be restored at the end of frame. Otherwise the runtime
	// will complain unreleased resource due to D3DX11Effect.
	//RestoreDefaultStates(pd3dImmediateContext);
}

void GaussianMain::ReleaseResources()
{
	SAFE_RELEASE(g_pTex_Scene);
	SAFE_RELEASE(g_pRTV_Scene);
	SAFE_RELEASE(g_pSRV_Scene);
	SAFE_RELEASE(g_pTex_Output);
	SAFE_RELEASE(g_pUAV_Output);
	SAFE_RELEASE(g_pSRV_Output);

	// The compute shader for Gaussian filtering is re-compiled every time the
	// size of input image changed.
	SAFE_RELEASE(g_pFX_GaussianCol);
	SAFE_RELEASE(g_pFX_GaussianRow);
}

void GaussianMain::OnD3D11DestroyDevice()
{
	ReleaseResources();

	SAFE_RELEASE(g_pFX_Render);
	SAFE_RELEASE(g_pSRV_Background);
	SAFE_RELEASE(g_pScreenQuadVB);
	SAFE_RELEASE(g_pQuadLayout);
	SAFE_RELEASE(g_pRTV_Default);
	SAFE_RELEASE(g_pDSV_Default);
}