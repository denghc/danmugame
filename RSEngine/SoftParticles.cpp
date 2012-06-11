#include "SoftParticles.h"

int SoftParticles::iFrame = 0;

SoftParticles::SoftParticles()
{
	SetVariables();
}
SoftParticles::~SoftParticles()
{
	OnD3D11DestroyDevice();
}

void SoftParticles::SetVariables()
{
	g_pCPUParticles = NULL;
	g_pCPUParticleIndices = NULL;
	g_pParticleDepthArray = NULL;

	g_pEffect11 = NULL;
	g_pParticleVertexLayout = NULL;

	g_pDepthStencilTexture = NULL;
	g_pDepthStencilDSV = NULL;
	g_pDepthStencilSRV = NULL;

	g_pParticleVB = NULL;
	g_pParticleIB = NULL;
	g_pParticleTexRV = NULL;
	g_pNoiseVolume = NULL;
	g_pNoiseVolumeRV = NULL;
	g_pColorGradTexture = NULL;
	g_pColorGradTexRV = NULL;

	g_pRenderBillboardParticlesHard = NULL;
	g_pRenderBillboardParticlesODepth = NULL;
	g_pRenderBillboardParticlesSoft = NULL;
	g_pRenderVolumeParticlesHard = NULL;
	g_pRenderVolumeParticlesSoft = NULL;
	g_pRenderBillboardParticlesODepthSoft = NULL;
	g_pRenderVolumeParticlesSoftMSAA = NULL;
	g_pRenderVolumeParticlesHardMSAA = NULL;
	g_pRenderBillboardParticlesSoftMSAA = NULL;
	g_pRenderBillboardParticlesODepthSoftMSAA = NULL;
	g_pmWorldViewProj = NULL;
	g_pmWorldView = NULL;
	g_pmWorld = NULL;
	g_pmInvView = NULL;
	g_pmInvProj = NULL;
	g_pfFadeDistance = NULL;
	g_pfSizeZScale = NULL;
	g_pvViewLightDir1 = NULL;
	g_pvViewLightDir2 = NULL;
	g_pvWorldLightDir1 = NULL;
	g_pvWorldLightDir2 = NULL;
	g_pvEyePt = NULL;
	g_pvViewDir = NULL;
	g_pvOctaveOffsets = NULL;
	g_pvScreenSize = NULL;
	g_pDiffuseTex = NULL;
	g_pNormalTex = NULL;
	g_pColorGradient = NULL;
	g_pVolumeDiffTex = NULL;
	g_pVolumeNormTex = NULL;
	g_pDepthTex = NULL;
	g_pDepthMSAATex = NULL;

	g_iWidth = 800;
	g_iHeight = 600;
	g_iSampleCount = 1;

	float scaleValue = 2.5f;

	g_fFadeDistance = 4 * 16.0f * scaleValue;
	g_fParticleLifeSpan = 5.0f;
	g_fEmitRate = 0.015f;

	g_ParticleVel = -3.0f * scaleValue;
	g_fParticleMaxSize = 1.25f * scaleValue;
	g_fParticleMinSize = 1.0f * scaleValue;
	g_bAnimateParticles = true;

	g_ParticleTechnique = PT_BILLBOARD_ODEPTH;//PT_BILLBOARD_HARD
	g_vLightDir1 = D3DXVECTOR3(1.705f,5.557f,-9.380f);
	g_vLightDir2 = D3DXVECTOR3(-5.947f,-5.342f,-5.733f);

	iFrame = 0;
	m_time = 0.0f;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT SoftParticles::OnD3D11CreateDevice(ID3D11Device* pd3dDevice)
{
	HRESULT hr;

	V_RETURN( LoadEffectFromFile(pd3dDevice,  L"SoftParticles.hlsl", &g_pEffect11) );

	// Obtain the technique handles
	g_pRenderBillboardParticlesHard = g_pEffect11->GetTechniqueByName( "RenderBillboardParticles_Hard" );
	g_pRenderBillboardParticlesODepth = g_pEffect11->GetTechniqueByName( "RenderBillboardParticles_ODepth" );
	g_pRenderBillboardParticlesSoft = g_pEffect11->GetTechniqueByName( "RenderBillboardParticles_Soft" );
	g_pRenderBillboardParticlesODepthSoft = g_pEffect11->GetTechniqueByName( "RenderBillboardParticles_ODepthSoft" );
	g_pRenderVolumeParticlesHard = g_pEffect11->GetTechniqueByName( "RenderVolumeParticles_Hard" );
	g_pRenderVolumeParticlesSoft = g_pEffect11->GetTechniqueByName( "RenderVolumeParticles_Soft" );
	g_pRenderVolumeParticlesSoftMSAA = g_pEffect11->GetTechniqueByName( "RenderVolumeParticles_Soft_MSAA" );
	g_pRenderVolumeParticlesHardMSAA = g_pEffect11->GetTechniqueByName( "RenderVolumeParticles_Hard_MSAA" );
	g_pRenderBillboardParticlesSoftMSAA = g_pEffect11->GetTechniqueByName( "RenderBillboardParticles_Soft_MSAA" );
	g_pRenderBillboardParticlesODepthSoftMSAA = g_pEffect11->GetTechniqueByName( "RenderBillboardParticles_ODepthSoft_MSAA" );
  
	// Obtain the parameter handles
	g_pmWorldViewProj = g_pEffect11->GetVariableByName( "g_mWorldViewProj" )->AsMatrix();
	g_pmWorldView = g_pEffect11->GetVariableByName( "g_mWorldView" )->AsMatrix();
	g_pmWorld = g_pEffect11->GetVariableByName( "g_mWorld" )->AsMatrix();
	g_pmInvView = g_pEffect11->GetVariableByName( "g_mInvView" )->AsMatrix();
	g_pmInvProj = g_pEffect11->GetVariableByName( "g_mInvProj" )->AsMatrix();
	g_pfFadeDistance = g_pEffect11->GetVariableByName( "g_fFadeDistance" )->AsScalar();
	g_pfSizeZScale = g_pEffect11->GetVariableByName( "g_fSizeZScale" )->AsScalar();
	g_pvViewLightDir1 = g_pEffect11->GetVariableByName( "g_vViewLightDir1" )->AsVector();
	g_pvViewLightDir2 = g_pEffect11->GetVariableByName( "g_vViewLightDir2" )->AsVector();
	g_pvWorldLightDir1 = g_pEffect11->GetVariableByName( "g_vWorldLightDir1" )->AsVector();
	g_pvWorldLightDir2 = g_pEffect11->GetVariableByName( "g_vWorldLightDir2" )->AsVector();
	g_pvEyePt = g_pEffect11->GetVariableByName( "g_vEyePt" )->AsVector();
	g_pvViewDir = g_pEffect11->GetVariableByName( "g_vViewDir" )->AsVector();
	g_pvOctaveOffsets = g_pEffect11->GetVariableByName( "g_OctaveOffsets" )->AsVector();
	g_pvScreenSize = g_pEffect11->GetVariableByName( "g_vScreenSize" )->AsVector();
	g_pDiffuseTex = g_pEffect11->GetVariableByName( "g_txDiffuse" )->AsShaderResource();
	g_pNormalTex = g_pEffect11->GetVariableByName( "g_txNormal" )->AsShaderResource();
	g_pColorGradient = g_pEffect11->GetVariableByName( "g_txColorGradient" )->AsShaderResource();
	g_pVolumeDiffTex = g_pEffect11->GetVariableByName( "g_txVolumeDiff" )->AsShaderResource();
	g_pVolumeNormTex = g_pEffect11->GetVariableByName( "g_txVolumeNorm" )->AsShaderResource();
	g_pDepthTex = g_pEffect11->GetVariableByName( "g_txDepth" )->AsShaderResource();
	g_pDepthMSAATex = g_pEffect11->GetVariableByName( "g_txDepthMSAA" )->AsShaderResource();

	// Create our vertex input layouts
	D3DX11_PASS_DESC PassDesc;
	const D3D11_INPUT_ELEMENT_DESC particlelayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "LIFE",     0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE",     0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	g_pRenderBillboardParticlesHard->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	V_RETURN( pd3dDevice->CreateInputLayout( particlelayout, sizeof(particlelayout)/sizeof(particlelayout[0]), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &g_pParticleVertexLayout ) );

	// Create the particles
	V_RETURN( CreateParticleBuffers( pd3dDevice ) );

	// Create the noise volume
	V_RETURN( CreateNoiseVolume( pd3dDevice, 32 ) );

	// Load the Particle Texture
	V_RETURN( D3DX11CreateShaderResourceViewFromFile( pd3dDevice, L"Resource\\SmokeTexture\\smokevol1.dds", NULL, NULL, &g_pParticleTexRV, NULL ) );
	V_RETURN( D3DX11CreateShaderResourceViewFromFile( pd3dDevice, L"Resource\\SmokeTexture\\colorgradient.dds", NULL, NULL, &g_pColorGradTexRV, NULL ) );

	g_pfFadeDistance->SetFloat( g_fFadeDistance );

	//---------------------------------
	//resize swap chain
	//---------------------------------

	// Create a new Depth-Stencil texture to replace the DXUT created one
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = 800;
	descDepth.Height = 600;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS; // Use a typeless type here so that it can be both depth-stencil and shader resource.
	// This will fail if we try a format like D32_FLOAT
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	V_RETURN( pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencilTexture ) );

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	if( 1 == descDepth.SampleDesc.Count ) {
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	} else {
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	descDSV.Flags = 0;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;	// Make the view see this as D32_FLOAT instead of typeless
	descDSV.Texture2D.MipSlice = 0;
	V_RETURN( pd3dDevice->CreateDepthStencilView( g_pDepthStencilTexture, &descDSV, &g_pDepthStencilDSV ) );

	// Create the shader resource view
	if( 1 == descDepth.SampleDesc.Count ) {

		D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;	// Make the shaders see this as R32_FLOAT instead of typeless
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MipLevels = 1;
		descSRV.Texture2D.MostDetailedMip = 0;
		V_RETURN( pd3dDevice->CreateShaderResourceView( g_pDepthStencilTexture, &descSRV, &g_pDepthStencilSRV ) );
	}
			
	g_iWidth = 800;
	g_iHeight = 600;
	g_iSampleCount = 1;

	//----------------------------------
	// frame move
	//----------------------------------

	return S_OK;
}

//--------------------------------------------------------------------------------------
void SoftParticles::OnD3D11FrameRender( ID3D11DeviceContext* context, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,D3DXVECTOR3 spaceshipPos)
{
	//////////////////////////////////////////////////////////////////////////
	//On Frame Move
	//////////////////////////////////////////////////////////////////////////

	CameraClass* cc = CameraManager::getInstance()->getCamera("mainCamera");
	const float dt = 1.0f/60.0f;
	m_time += dt;

	D3DXVECTOR3 vEye = cc->GetPosition();   
	D3DXVECTOR3 vDir = - vEye;
	D3DXVec3Normalize( &vDir, &vDir );

	AdvanceParticles( m_time, dt );
	SortParticleBuffer( vEye, vDir );
	UpdateParticleBuffers( context );

	// Update the movement of the noise octaves
	D3DXVECTOR4 OctaveOffsets[4];
	for( int i=0; i<4; i++ )
	{
		OctaveOffsets[i].x = -(float)(m_time*0.05);
		OctaveOffsets[i].y = 0;
		OctaveOffsets[i].z = 0;
		OctaveOffsets[i].w = 0;
	}
	g_pvOctaveOffsets->SetFloatVectorArray( (float*)OctaveOffsets, 0, 4 );
	
	//////////////////////////////////////////////////////////////////////////
	//On Frame Move End
	//////////////////////////////////////////////////////////////////////////

	ID3D11RenderTargetView* pRTV;
	ID3D11DepthStencilView* pDSV;
	context->OMGetRenderTargets(1,&pRTV,&pDSV);

	// Get the projection & view matrix from the camera class
	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mInvView;
	D3DXMATRIX mInvProj;
	D3DXMatrixIdentity( &mWorld );

	//D3DXMATRIX mScale;
	//float mScaleValue = 1.0f;
	//D3DXMatrixScaling(&mScale,mScaleValue,mScaleValue,mScaleValue);
	D3DXMatrixTranslation(&mWorld,spaceshipPos.x - 15.0f,spaceshipPos.y + 3.5f,spaceshipPos.z);
	//mWorld = mScale * mWorld;

	mProj = projectionMatrix;
	mView = viewMatrix;
	D3DXMATRIX mWorldViewProj = mWorld*mView*mProj;
	D3DXMATRIX mWorldView = mWorld*mView;
	D3DXMatrixInverse( &mInvView, NULL, &mView );
	D3DXMatrixInverse( &mInvProj, NULL, &mProj);
	D3DXVECTOR4 vViewLightDir1;
	D3DXVECTOR4 vWorldLightDir1;
	D3DXVECTOR4 vViewLightDir2;
	D3DXVECTOR4 vWorldLightDir2;
	D3DXVec3Normalize( (D3DXVECTOR3*)&vWorldLightDir1, &g_vLightDir1 );
	D3DXVec3TransformNormal( (D3DXVECTOR3*)&vViewLightDir1, &g_vLightDir1, &mView );
	D3DXVec3Normalize( (D3DXVECTOR3*)&vViewLightDir1, (D3DXVECTOR3*)&vViewLightDir1 );
	D3DXVec3Normalize( (D3DXVECTOR3*)&vWorldLightDir2, &g_vLightDir2 );
	D3DXVec3TransformNormal( (D3DXVECTOR3*)&vViewLightDir2, &g_vLightDir2, &mView );
	D3DXVec3Normalize( (D3DXVECTOR3*)&vViewLightDir2, (D3DXVECTOR3*)&vViewLightDir2 );
	D3DXVECTOR3 viewDir = - cc->GetPosition();
	D3DXVec3Normalize( &viewDir, &viewDir );
	D3DXVECTOR3 vec3 = cc->GetPosition();
	D3DXVECTOR4 vEyePt;
	vEyePt.x = vec3.x;
	vEyePt.y = vec3.y;
	vEyePt.z = vec3.z;
	FLOAT fScreenSize[ 2 ] = { (FLOAT)g_iWidth, (FLOAT)g_iHeight };

	g_pmWorldViewProj->SetMatrix( (float*)&mWorldViewProj );
	g_pmWorldView->SetMatrix( (float*)&mWorldView );
	g_pmWorld->SetMatrix( (float*)&mWorld );
	g_pmInvView->SetMatrix( (float*)&mInvView );
	g_pmInvProj->SetMatrix( (float*)&mInvProj );
	g_pvViewLightDir1->SetFloatVector( (float*)&vViewLightDir1 );
	g_pvWorldLightDir1->SetFloatVector( (float*)&vWorldLightDir1);
	g_pvViewLightDir2->SetFloatVector( (float*)&vViewLightDir2 );
	g_pvWorldLightDir2->SetFloatVector( (float*)&vWorldLightDir2);
	g_pvViewDir->SetFloatVector( (float*)&viewDir );
	g_pvEyePt->SetFloatVector( (float*)&vEyePt );
	g_pvScreenSize->SetFloatVector( fScreenSize );

	ID3DX11EffectTechnique* pParticleTech = NULL;

	if( 1 == g_iSampleCount ) {
		switch( g_ParticleTechnique )
		{
		case PT_BILLBOARD_HARD:
			pParticleTech = g_pRenderBillboardParticlesHard;
			break;
		case PT_BILLBOARD_ODEPTH:
			pParticleTech = g_pRenderBillboardParticlesODepth;
			break;
		case PT_BILLBOARD_SOFT:
			pParticleTech = g_pRenderBillboardParticlesSoft;
			break;
		case PT_BILLBOARD_ODEPTHSOFT:
			pParticleTech = g_pRenderBillboardParticlesODepthSoft;
			break;
		case PT_VOLUME_HARD:
			pParticleTech = g_pRenderVolumeParticlesHard;
			break;
		case PT_VOLUME_SOFT:
			pParticleTech = g_pRenderVolumeParticlesSoft;
			break;
		};
	} else {
		switch( g_ParticleTechnique )
		{
		case PT_BILLBOARD_HARD:
			pParticleTech = g_pRenderBillboardParticlesHard;
			break;
		case PT_BILLBOARD_ODEPTH:
			pParticleTech = g_pRenderBillboardParticlesODepth;
			break;
		case PT_BILLBOARD_SOFT:
			pParticleTech = g_pRenderBillboardParticlesSoftMSAA;
			break;
		case PT_BILLBOARD_ODEPTHSOFT:
			pParticleTech = g_pRenderBillboardParticlesODepthSoftMSAA;
			break;
		case PT_VOLUME_HARD:
			pParticleTech = g_pRenderVolumeParticlesHardMSAA;
			break;
		case PT_VOLUME_SOFT:
			pParticleTech = g_pRenderVolumeParticlesSoftMSAA;
			break;
		};
	}

	if( PT_BILLBOARD_HARD != g_ParticleTechnique &&
		PT_BILLBOARD_ODEPTH != g_ParticleTechnique )
	{
		// Unbind the depth stencil texture from the device
		context->OMSetRenderTargets( 1, &pRTV, NULL );
		// Bind it instead as a shader resource view
		if( 1 == g_iSampleCount ) {
			g_pDepthTex->SetResource( g_pDepthStencilSRV );
		} else {
			g_pDepthMSAATex->SetResource( g_pDepthStencilSRV );
		}
	}

	// Render the particles
	context->IASetInputLayout( g_pParticleVertexLayout );
	ID3D11Buffer *pBuffers[1] = { g_pParticleVB };
	UINT stride[1] = { sizeof(PARTICLE_VERTEX) };
	UINT offset[1] = { 0 };
	context->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
	context->IASetIndexBuffer( g_pParticleIB, DXGI_FORMAT_R32_UINT, 0 );
    
	if( PT_VOLUME_HARD == g_ParticleTechnique ||
		PT_VOLUME_SOFT == g_ParticleTechnique )
	{
		g_pVolumeDiffTex->SetResource( g_pNoiseVolumeRV );
		g_pVolumeNormTex->SetResource( NULL );
	}
	else 
	{
		g_pVolumeDiffTex->SetResource( g_pParticleTexRV );
	}
	g_pColorGradient->SetResource( g_pColorGradTexRV );

	D3DX11_TECHNIQUE_DESC techDesc;
	pParticleTech->GetDesc( &techDesc );

	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		pParticleTech->GetPassByIndex( p )->Apply(0,context);
		context->DrawIndexed( MAX_PARTICLES, 0, 0 );
	}
   
	// unbind the depth from the resource so we can set it as depth next time around
	ID3D11ShaderResourceView* Nulls[2] = {NULL,NULL};
	context->PSSetShaderResources( 0, 2, Nulls );
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void SoftParticles::OnD3D11DestroyDevice()
{
	SAFE_RELEASE( g_pEffect11 );
	SAFE_RELEASE( g_pParticleVertexLayout );
	SAFE_RELEASE( g_pParticleVB );
	SAFE_RELEASE( g_pParticleIB );
	SAFE_RELEASE( g_pParticleTexRV );
	SAFE_RELEASE( g_pNoiseVolume );
	SAFE_RELEASE( g_pNoiseVolumeRV );
	SAFE_RELEASE( g_pColorGradTexRV );

	SAFE_DELETE_ARRAY( g_pCPUParticles );
	SAFE_DELETE_ARRAY( g_pCPUParticleIndices );
	SAFE_DELETE_ARRAY( g_pParticleDepthArray );
}

//--------------------------------------------------------------------------------------
float SoftParticles::RPercent()
{
	float ret = (float)((rand()%20000) - 11000 );
	return ret/11000.0f;
}

//--------------------------------------------------------------------------------------
void SoftParticles::EmitParticle( PARTICLE_VERTEX* pParticle )
{
	pParticle->Pos.x = 0.0f;
	pParticle->Pos.y = 0.7f;
	pParticle->Pos.z = 3.0f;

	pParticle->Vel.x = 1.0f;
	pParticle->Vel.y = 0.3f*RPercent();
	pParticle->Vel.z = 0.3f*RPercent();

	D3DXVec3Normalize( &pParticle->Vel, &pParticle->Vel );
	pParticle->Vel *= g_ParticleVel;

	pParticle->Life = 0.0f;
	pParticle->Size = 0.0f;
}

//--------------------------------------------------------------------------------------
// Create a VB for particles
//--------------------------------------------------------------------------------------
HRESULT SoftParticles::CreateParticleBuffers( ID3D11Device* pd3dDevice )
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vbdesc;
	vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbdesc.ByteWidth = MAX_PARTICLES*sizeof(PARTICLE_VERTEX);
	vbdesc.CPUAccessFlags = 0;
	vbdesc.MiscFlags = 0;
	vbdesc.Usage = D3D11_USAGE_DEFAULT;
	V_RETURN( pd3dDevice->CreateBuffer( &vbdesc, NULL, &g_pParticleVB ) );

	vbdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vbdesc.ByteWidth = MAX_PARTICLES*sizeof(DWORD);
	V_RETURN( pd3dDevice->CreateBuffer( &vbdesc, NULL, &g_pParticleIB ) );

	g_pCPUParticles = new PARTICLE_VERTEX[ MAX_PARTICLES ];
	if( !g_pCPUParticles )
		return E_OUTOFMEMORY;

	for( UINT i=0; i<MAX_PARTICLES; i++ )
	{
		g_pCPUParticles[i].Life = -1;	//kill all particles
	}

	g_pCPUParticleIndices = new DWORD[ MAX_PARTICLES ];
	if( !g_pCPUParticleIndices )
		return E_OUTOFMEMORY;
	g_pParticleDepthArray = new float[ MAX_PARTICLES ]; 
	if( !g_pParticleDepthArray )
		return E_OUTOFMEMORY;

	return hr;
}

//--------------------------------------------------------------------------------------
float SoftParticles::GetDensity( int x, int y, int z, CHAR4* pTexels, UINT VolumeSize )
{
	if( x < 0 )
		x += VolumeSize;
	if( y < 0 )
		y += VolumeSize;
	if( z < 0 )
		z += VolumeSize;

	x = x%VolumeSize;
	y = y%VolumeSize;
	z = z%VolumeSize;

	int index = x + y*VolumeSize + z*VolumeSize*VolumeSize;

	return (float)pTexels[index].w / 128.0f;
}

void SoftParticles::SetNormal( D3DXVECTOR3 Normal, int x, int y, int z, CHAR4* pTexels, UINT VolumeSize )
{
	if( x < 0 )
		x += VolumeSize;
	if( y < 0 )
		y += VolumeSize;
	if( z < 0 )
		z += VolumeSize;

	x = x%VolumeSize;
	y = y%VolumeSize;
	z = z%VolumeSize;

	int index = x + y*VolumeSize + z*VolumeSize*VolumeSize;

	pTexels[index].x = (char)(Normal.x * 128.0f);
	pTexels[index].y = (char)(Normal.y * 128.0f);
	pTexels[index].z = (char)(Normal.z * 128.0f);
}

//--------------------------------------------------------------------------------------
// Create and blur a noise volume texture
//--------------------------------------------------------------------------------------
HRESULT SoftParticles::CreateNoiseVolume( ID3D11Device* pd3dDevice, UINT VolumeSize )
{
	HRESULT hr = S_OK;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = new CHAR4[ VolumeSize*VolumeSize*VolumeSize ];
	InitData.SysMemPitch = VolumeSize*sizeof(CHAR4);
	InitData.SysMemSlicePitch = VolumeSize*VolumeSize*sizeof(CHAR4);

	// Gen a bunch of random values
	CHAR4* pData = (CHAR4*)InitData.pSysMem;
	for( UINT i=0; i<VolumeSize*VolumeSize*VolumeSize; i++ )
	{
		pData[i].w = (char)(RPercent() * 128.0f);
	}

	// Generate normals from the density gradient
	float heightAdjust = 0.5f;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 DensityGradient;
	for( UINT z=0; z<VolumeSize; z++ )
	{
		for( UINT y=0; y<VolumeSize; y++ )
		{
			for( UINT x=0; x<VolumeSize; x++ )
			{
				DensityGradient.x = GetDensity( x+1, y, z, pData, VolumeSize ) - GetDensity( x-1, y, z, pData, VolumeSize )/heightAdjust;
				DensityGradient.y = GetDensity( x, y+1, z, pData, VolumeSize ) - GetDensity( x, y-1, z, pData, VolumeSize )/heightAdjust;
				DensityGradient.z = GetDensity( x, y, z+1, pData, VolumeSize ) - GetDensity( x, y, z-1, pData, VolumeSize )/heightAdjust;

				D3DXVec3Normalize( &Normal, &DensityGradient );
				SetNormal( Normal, x,y,z, pData, VolumeSize );
			}
		}
	}

	D3D11_TEXTURE3D_DESC desc;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.Depth = desc.Height = desc.Width = VolumeSize;
	desc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	V_RETURN( pd3dDevice->CreateTexture3D( &desc, &InitData, &g_pNoiseVolume ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	SRVDesc.Texture3D.MipLevels = desc.MipLevels;
	SRVDesc.Texture3D.MostDetailedMip = 0;
	V_RETURN(pd3dDevice->CreateShaderResourceView( g_pNoiseVolume, &SRVDesc, &g_pNoiseVolumeRV ));
    
	delete InitData.pSysMem;
	return hr;
}

//--------------------------------------------------------------------------------------
// Update the particle VB using UpdateSubresource
//--------------------------------------------------------------------------------------
void SoftParticles::QuickDepthSort(DWORD* indices, float* depths, int lo, int hi)
{
//  lo is the lower index, hi is the upper index
//  of the region of array a that is to be sorted
	int i=lo, j=hi;
	float h;
	int index;
	float x=depths[(lo+hi)/2];

	//  partition
	do
	{    
		while (depths[i] > x) i++; 
		while (depths[j] < x) j--;
		if (i<=j)
		{
			h=depths[i]; depths[i]=depths[j]; depths[j]=h;
			index = indices[i]; indices[i] = indices[j]; indices[j] = index;
			i++; j--;
		}
	} while (i<=j);

	//  recursion
	if (lo<j) QuickDepthSort(indices, depths, lo, j);
	if (i<hi) QuickDepthSort(indices, depths, i, hi);
}

//--------------------------------------------------------------------------------------
// Sort the particle buffer
//--------------------------------------------------------------------------------------
void SoftParticles::SortParticleBuffer( D3DXVECTOR3 vEye, D3DXVECTOR3 vDir )
{
	if( !g_pParticleDepthArray || !g_pCPUParticleIndices )
		return;

	// assume vDir is normalized
	D3DXVECTOR3 vToParticle;
	//init indices and depths
	for( UINT i=0; i<MAX_PARTICLES; i++ )
	{
		g_pCPUParticleIndices[i] = i;
		vToParticle = g_pCPUParticles[i].Pos - vEye;
		g_pParticleDepthArray[i] = D3DXVec3Dot( &vDir, &vToParticle );
	}

	// Sort
	QuickDepthSort(g_pCPUParticleIndices, g_pParticleDepthArray, 0, MAX_PARTICLES-1);
}

//--------------------------------------------------------------------------------------
// Create a VB for particles
//--------------------------------------------------------------------------------------
void SoftParticles::AdvanceParticles(double fTime, float fTimeDelta )
{
	//emit new particles
	static double fLastEmitTime = 0;
	static UINT iLastParticleEmitted = 0;

	if( !g_bAnimateParticles )
	{
		fLastEmitTime = fTime;
		return;
	}

	float fEmitRate = g_fEmitRate;
	float fParticleMaxSize = g_fParticleMaxSize;
	float fParticleMinSize = g_fParticleMinSize;

	if( PT_VOLUME_HARD == g_ParticleTechnique ||
		PT_VOLUME_SOFT == g_ParticleTechnique )
	{
		fEmitRate *= 3.0f;	//emit 1/3 less particles if we're doing volume
		fParticleMaxSize *= 1.5f;	//1.5x the max radius
		fParticleMinSize *= 1.5f;	//1.5x the min radius
	}

	UINT NumParticlesToEmit = (UINT)( (fTime - fLastEmitTime)/fEmitRate );
	if( NumParticlesToEmit > 0 )
	{
		for( UINT i=0; i<NumParticlesToEmit; i++ )
		{
			EmitParticle( &g_pCPUParticles[iLastParticleEmitted] );
			iLastParticleEmitted = (iLastParticleEmitted+1) % MAX_PARTICLES;
		}
		fLastEmitTime = fTime;
	}

	D3DXVECTOR3 vel;
	float lifeSq = 0;
	for( UINT i=0; i<MAX_PARTICLES; i++ )
	{
		if( g_pCPUParticles[i].Life > -1 )
		{
			// squared velocity falloff
			lifeSq = g_pCPUParticles[i].Life*g_pCPUParticles[i].Life;

			// Slow down by 50% as we age
			vel = g_pCPUParticles[i].Vel * (1 - 0.5f*lifeSq);
			vel.y += 0.5f;	//(add some to the up direction, becuase of buoyancy)

			g_pCPUParticles[i].Pos += vel*fTimeDelta;
			g_pCPUParticles[i].Life += fTimeDelta/g_fParticleLifeSpan;
			g_pCPUParticles[i].Size = fParticleMinSize + (fParticleMaxSize-fParticleMinSize) * g_pCPUParticles[i].Life;

			if( g_pCPUParticles[i].Life > 0.99f )
				g_pCPUParticles[i].Life = -1;
		}
	}
}

//--------------------------------------------------------------------------------------
// Update the particle VB using UpdateSubresource
//--------------------------------------------------------------------------------------
void SoftParticles::UpdateParticleBuffers( ID3D11DeviceContext* pd3dDevice )
{
	pd3dDevice->UpdateSubresource( g_pParticleVB, NULL, NULL, g_pCPUParticles, 0, 0 );
	pd3dDevice->UpdateSubresource( g_pParticleIB, NULL, NULL, g_pCPUParticleIndices, 0, 0 );
}

HRESULT SoftParticles::LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect )
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