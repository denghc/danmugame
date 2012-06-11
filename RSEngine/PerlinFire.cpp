#include "PerlinFire.h"

PerlinFire::PerlinFire()
{
	SetVariables();
}

PerlinFire::~PerlinFire()
{
	OnD3D11DestroyDevice();
}

void PerlinFire::SetVariables()
{
	g_pEffect = NULL;
	g_pGeometryVertexLayout   = NULL;

	// Shader variables and matrices
	g_pmCubeViewMatrixVariable = NULL;
	g_pmCubeProjMatrixVariable = NULL;
	g_pmWorldViewProj = NULL;
	g_pvEyePos = NULL;
	g_pvLightPos = NULL;
	g_pfLightIntensity = NULL;
	g_pfStepSize = NULL;
	g_pfTime = NULL;
	g_pfNoiseScale = NULL;
	g_pfRoughness = NULL;
	g_pfFrequencyWeights = NULL;
	g_pbJitter = NULL;
	g_piCubeMapFace = NULL;

	// Textures and related shader resource views
	g_pDepthBuffer = NULL;
	g_pDepthBufferSRV = NULL;
	g_pDepthBufferDSV = NULL;

	g_pFireTexture = NULL;
	g_pFireTextureSRV = NULL;

	g_pNoiseTexture = NULL;
	g_pJitterTextureSRV = NULL;
	g_pPermTextureSRV = NULL;

	g_ptxScreenDepth = NULL;
	g_ptxSceneTexture = NULL;
	g_ptxFireTex = NULL;
	g_ptxJitterTex = NULL;
	g_ptxPermTex = NULL;

	g_pCubeMapDepth = NULL;
	//g_pCubeMapDepthViewArray[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
	g_pCubeMapTextureRV = NULL;
	g_pCubeMapTextureVariable = NULL;

	g_pCurrentTechnique = NULL;
	g_pPerlinFire3D = NULL;
	g_pPerlinFire4D = NULL;
	g_pPerlinFire3DFlow = NULL;
	g_pGeometryTechnique = NULL;
	g_pGeometryTechniqueAux = NULL;

	g_pBuffer = NULL;
	g_pIndex = NULL;

	g_bJitter = DEFAULT_JITTER;
	g_nSamplingRate = DEFAULT_SAMPLING_RATE;
	g_fSpeed = DEFAULT_SPEED;
	g_fNoiseScale = DEFAULT_NOISE_SCALE;
	g_fRoughness = DEFAULT_ROUGHNESS;
	g_fShapeSize = DEFAULT_SHAPE_SIZE;
	g_fFrequencyWeights[0] = DEFAULT_FREQUENCY1;
	g_fFrequencyWeights[1] = DEFAULT_FREQUENCY2;
	g_fFrequencyWeights[2] = DEFAULT_FREQUENCY3;
	g_fFrequencyWeights[3] = DEFAULT_FREQUENCY4;
	g_fFrequencyWeights[4] = DEFAULT_FREQUENCY5;
	g_CubeMapSize = 800;
}

bool PerlinFire::LoadTexture2D(ID3D11Device * pd3dDevice, LPCWSTR fileName, ID3D11Texture2D ** tex, ID3D11ShaderResourceView ** texRV)
{
	HRESULT hr;
	ID3D11Resource * pRes = NULL;

	D3DX11CreateTextureFromFile( pd3dDevice, fileName, NULL, NULL, &pRes, &hr );

	pRes->QueryInterface( __uuidof(ID3D11Texture2D), (LPVOID*) tex);

	D3D11_TEXTURE2D_DESC desc;
	(*tex)->GetDesc(& desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory( &descSRV, sizeof(descSRV) );
	descSRV.Format = desc.Format;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = desc.MipLevels;

	pd3dDevice->CreateShaderResourceView( *tex, &descSRV, texRV );

	pRes->Release();

	return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependent on the back buffer
//--------------------------------------------------------------------------------------
HRESULT PerlinFire::OnD3D11CreateDevice( ID3D11Device * pd3dDevice)
{
	HRESULT hr;
	
	m_time = 0.0f;
	FirePosition = D3DXVECTOR3(0,0,0);
	const float lengthOfSide = 0.5f;
	//const float myparameter = 1.0f;
	SimpleVertex vertices[] =
	{
		{ D3DXVECTOR3( -lengthOfSide, lengthOfSide, -lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
		{ D3DXVECTOR3( lengthOfSide, lengthOfSide, -lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
		{ D3DXVECTOR3( lengthOfSide, lengthOfSide, lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
		{ D3DXVECTOR3( -lengthOfSide, lengthOfSide, lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
		{ D3DXVECTOR3( -lengthOfSide, -lengthOfSide, -lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
		{ D3DXVECTOR3( lengthOfSide, -lengthOfSide, -lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
		{ D3DXVECTOR3( lengthOfSide, -lengthOfSide, lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
		{ D3DXVECTOR3( -lengthOfSide, -lengthOfSide, lengthOfSide ) ,D3DXVECTOR3(0,0,0),D3DXVECTOR2(0,0)},
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = vertices;
	V_RETURN(pd3dDevice->CreateBuffer( &bd, &InitData, &g_pBuffer ));

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	V_RETURN(pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndex ));

	V_RETURN(LoadEffectFromFile(pd3dDevice,L"PerlinFire.hlsl",&g_pEffect));

	// Obtain techniques
	g_pPerlinFire3D = g_pEffect->GetTechniqueByName( "PerlinFire3D" );
	g_pPerlinFire3DFlow = g_pEffect->GetTechniqueByName( "PerlinFire3DFlow" );
	g_pPerlinFire4D = g_pEffect->GetTechniqueByName( "PerlinFire4D" );
	g_pGeometryTechnique = g_pEffect->GetTechniqueByName( "RenderGeometry" );
	g_pGeometryTechniqueAux = g_pEffect->GetTechniqueByName( "RenderGeometryAux" );

	g_pCurrentTechnique = g_pPerlinFire4D;

	// Obtain texture variables

	g_ptxScreenDepth = g_pEffect->GetVariableByName( "ScreenDepth" )->AsShaderResource();
	g_ptxSceneTexture = g_pEffect->GetVariableByName( "SceneTexture" )->AsShaderResource();
	g_ptxFireTex = g_pEffect->GetVariableByName( "FireShape" )->AsShaderResource();
	g_ptxJitterTex = g_pEffect->GetVariableByName( "JitterTexture" )->AsShaderResource();
	g_ptxPermTex = g_pEffect->GetVariableByName( "PermTexture" )->AsShaderResource();
	g_pCubeMapTextureVariable = g_pEffect->GetVariableByName( "ShadowMap" )->AsShaderResource();

	// Obtain miscellaneous variables

	g_pmCubeViewMatrixVariable = g_pEffect->GetVariableByName( "CubeViewMatrices" )->AsMatrix();
	g_pmCubeProjMatrixVariable = g_pEffect->GetVariableByName( "CubeProjectionMatrix" )->AsMatrix();
	g_pmWorldViewProj = g_pEffect->GetVariableByName( "WorldViewProj" )->AsMatrix();
	g_pvEyePos = g_pEffect->GetVariableByName( "EyePos" )->AsVector();
	g_pvLightPos = g_pEffect->GetVariableByName( "LightPos" )->AsVector();
	g_pfLightIntensity = g_pEffect->GetVariableByName( "LightIntensity" )->AsScalar();
	g_pfStepSize = g_pEffect->GetVariableByName( "StepSize" )->AsScalar();
	g_pfTime = g_pEffect->GetVariableByName( "Time" )->AsScalar();
	g_pfNoiseScale = g_pEffect->GetVariableByName( "NoiseScale" )->AsScalar();
	g_pfRoughness = g_pEffect->GetVariableByName( "Roughness" )->AsScalar();
	g_pfFrequencyWeights = g_pEffect->GetVariableByName( "FrequencyWeights" )->AsScalar();
	g_pbJitter = g_pEffect->GetVariableByName( "bJitter" )->AsScalar();
	g_piCubeMapFace = g_pEffect->GetVariableByName( "CubeMapFace" )->AsScalar();

	// Set input layouts

	D3DX11_PASS_DESC PassDesc;

	D3D11_INPUT_ELEMENT_DESC geometryLayout [] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int numElements = sizeof (geometryLayout) / sizeof (geometryLayout [0]);

	g_pGeometryTechnique->GetPassByIndex(0)->GetDesc( & PassDesc );
	V_RETURN(pd3dDevice->CreateInputLayout( geometryLayout, numElements, PassDesc.pIAInputSignature,	PassDesc.IAInputSignatureSize, & g_pGeometryVertexLayout ));

	// Load textures
	LoadTexture2D( pd3dDevice, L"Resource\\FireTexture\\Firetex.dds", & g_pFireTexture, & g_pFireTextureSRV );

	// Create noise texture
	// Fill the texture with random numbers from 0 to 256
	srand( GetTickCount() );

	BYTE data[256 * 256];
	for (int i = 0; i < 256 * 256; i++)
	{
		data[i] = (unsigned char) (rand () % 256);
	}

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 256;
	desc.Height = 256;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA dataDesc;
	dataDesc.pSysMem = data;
	dataDesc.SysMemPitch = 256;

	V_RETURN(pd3dDevice->CreateTexture2D(&desc, &dataDesc, &g_pNoiseTexture));

	// Create the shader resource view for jittering
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;

	ZeroMemory( &descSRV, sizeof(descSRV) );
	descSRV.Format = DXGI_FORMAT_R8_UNORM;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;
	descSRV.Texture2D.MostDetailedMip = 0;

	V_RETURN(pd3dDevice->CreateShaderResourceView( g_pNoiseTexture, &descSRV, &g_pJitterTextureSRV ));

	// Create the shader resource view for permutation
	descSRV.Format = DXGI_FORMAT_R8_UINT;

	V_RETURN(pd3dDevice->CreateShaderResourceView( g_pNoiseTexture, &descSRV, &g_pPermTextureSRV ));

	V_RETURN(g_ptxFireTex -> SetResource (g_pFireTextureSRV));
	V_RETURN(g_ptxJitterTex -> SetResource (g_pJitterTextureSRV));
	V_RETURN(g_ptxPermTex -> SetResource (g_pPermTextureSRV));

	//---------------------------------
	//resize swap chain
	//---------------------------------

	//----------------------------------
	// frame move
	//----------------------------------

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void PerlinFire::OnD3D11FrameRender( ID3D11DeviceContext* pd3dDevice,D3DXMATRIX mView, D3DXMATRIX mProj)
{
	const float dt = 1.0f/60.0f;
	m_time += dt;

	// Set matrices
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMATRIX mViewProj = mView * mProj;
	D3DXMATRIX mWorldView = mWorld * mView;
	D3DXMATRIX mWorldViewProj = mWorldView * mProj;

	float rnd = (float)rand() / RAND_MAX * 0.5f + 0.5f;
	float lightPos[] = { 0.25f * (rnd - 0.5f), 5.7f, 1.0f * (rnd - 0.5f), 1.0f };

	g_pvLightPos->SetFloatVector( lightPos );
	g_pfNoiseScale->SetFloat( g_fNoiseScale );
	g_pfRoughness->SetFloat( g_fRoughness );
	g_pfFrequencyWeights->SetFloatArray( g_fFrequencyWeights, 0, 5 );

	g_pfTime->SetFloat( (float)m_time * g_fSpeed );
	g_pfStepSize->SetFloat( (float)1.0f / g_nSamplingRate );
	g_pbJitter->SetBool( g_bJitter );

	pd3dDevice->IASetInputLayout( g_pGeometryVertexLayout );

	// Render fire volume

	D3DXMATRIX mTranslate, mScale, mWorldViewInv;
	D3DXMatrixRotationZ(&mTranslate, -1.5708);
	D3DXMatrixScaling( &mScale, 10.0f * g_fShapeSize, 10.0f * g_fShapeSize, 10.0f * g_fShapeSize);
	
	D3DXMatrixTranslation(&mWorld, FirePosition.x, FirePosition.y, FirePosition.z);
	mWorldView = mTranslate * mScale * mWorld * mView;
	
	mWorldViewProj = mWorldView * mProj;

	D3DXMatrixInverse( &mWorldViewInv, NULL, &mWorldView );

	D3DXVECTOR4 vEye;
	D3DXVECTOR4 v(0, 0, 0, 1);
	D3DXVec4Transform( &vEye, &v, &mWorldViewInv );

	g_pmWorldViewProj->SetMatrix ( (float *) & mWorldViewProj );
	g_pvEyePos->SetFloatVector ( (float *) &vEye );
	g_pfLightIntensity->SetFloat( rnd );

	g_pCurrentTechnique->GetPassByIndex(0)->Apply(0,pd3dDevice);

	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	pd3dDevice->IASetVertexBuffers( 0, 1, &g_pBuffer, &stride, &offset );
	pd3dDevice->IASetIndexBuffer( g_pIndex, DXGI_FORMAT_R16_UINT, 0 );
	pd3dDevice->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	pd3dDevice->DrawIndexedInstanced(36,1,0,0,0);
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void PerlinFire::OnD3D11DestroyDevice()
{
	SAFE_RELEASE( g_pPermTextureSRV );
	SAFE_RELEASE( g_pJitterTextureSRV );
	SAFE_RELEASE( g_pNoiseTexture );
	SAFE_RELEASE( g_pFireTextureSRV );
	SAFE_RELEASE( g_pFireTexture );

	SAFE_RELEASE( g_pCubeMapDepth );
	SAFE_RELEASE( g_pCubeMapTextureRV );

	for( int i = 0; i < 6; i ++ )
	{
		//SAFE_RELEASE( g_pCubeMapDepthViewArray[i] );
	}

	SAFE_RELEASE( g_pGeometryVertexLayout );
	SAFE_RELEASE( g_pEffect );
	
	SAFE_RELEASE( g_pBuffer );
	SAFE_RELEASE( g_pIndex );
}

HRESULT PerlinFire::LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect )
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