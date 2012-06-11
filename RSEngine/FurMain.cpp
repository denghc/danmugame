#include "FurMain.h"
#include "CameraClass.h"
#include "CameraManager.h"

FurMain::FurMain()
{
	SetVariables();
}
FurMain::~FurMain()
{
	OnD3D11DestroyDevice();
}
void FurMain::SetVariables()
{
	g_pEffect = NULL;
	g_pVertexLayout = NULL;
	g_pVertexLayoutArrow = NULL;
	g_bShowHelp = false;
		
	g_pWorldVariable				 = NULL;
	g_pWorldViewProj				 = NULL;
	g_pEyeVec						 = NULL;
	g_pLightVec                      = NULL; 
	g_pShellNumberShaderVariable     = NULL;
	g_pNumShellsNumberShaderVariable = NULL;
	g_pShellIncrementShaderVariable  = NULL;
	g_pMaxOpacityShaderVariable      = NULL;
	g_pCombStrengthShaderVariable    = NULL;
	g_pCombVectorShaderVariable      = NULL;

	g_pTechniqueRenderMesh            = NULL;
	g_pTechniqueRenderShells          = NULL;
	g_pTechniqueRenderFinsLineAdj     = NULL;

	g_bToggleWireFrame = false;
	g_numShells = 15;
	g_shellIncrement = 0.2f;
	g_bShowShells = true;
	g_bShowFins = true;
	g_maxOpacity = 0.6f;
	g_combVector = D3DXVECTOR3(1.0f,-0.3f,0.0f);
	g_combStrength = 0.3f;
}

//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependent on the back buffer
//--------------------------------------------------------------------------------------
HRESULT FurMain::OnD3D11CreateDevice( ID3D11Device* pd3dDevice)
{
	DXGI_SURFACE_DESC desc;
	desc.Width = 800;
	desc.Height = 600;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	
	//unsigned int num_quality_levels = 0;
	//pd3dDevice->CheckMultisampleQualityLevels(desc.Format, desc.SampleDesc.Count,&num_quality_levels);

	HRESULT hr = S_OK;

	ID3D11DeviceContext* pd3dImmediateContext;
	pd3dDevice->GetImmediateContext(&pd3dImmediateContext);

	//create the raster states
	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FrontCounterClockwise = false;
	rasterizerState.DepthBias = 0;
	rasterizerState.DepthBiasClamp = 0.0f;
	rasterizerState.SlopeScaledDepthBias = 0.0f;
	rasterizerState.DepthClipEnable = false;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = true;
	rasterizerState.AntialiasedLineEnable = false;
	hr = pd3dDevice->CreateRasterizerState( &rasterizerState, &g_pRasterStateSolid );
	//pd3dImmediateContext->RSSetState(g_pRasterStateSolid);  
	rasterizerState.CullMode = D3D11_CULL_NONE;
	hr = pd3dDevice->CreateRasterizerState( &rasterizerState, &g_pRasterStateSolidNoCull );
	rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerState.CullMode = D3D11_CULL_NONE;
	hr = pd3dDevice->CreateRasterizerState( &rasterizerState, &g_pRasterStateWireFrame );

	//Create the effect
	DWORD dwShaderFlags = 0;
	V_RETURN( LoadEffectFromFile(pd3dDevice, L"FurShellsAndFins.hlsl", &g_pEffect) );

	// Obtain the techniques
	g_pTechniqueRenderMesh        = g_pEffect->GetTechniqueByName("RenderMesh");
	g_pTechniqueRenderShells      = g_pEffect->GetTechniqueByName("RenderShells");
	g_pTechniqueRenderFinsLineAdj = g_pEffect->GetTechniqueByName("RenderFinsLineAdj");

	// Obtain the variables
	g_pWorldVariable = g_pEffect->GetVariableByName( "World" )->AsMatrix();
	g_pWorldViewProj = g_pEffect->GetVariableByName( "WorldViewProj" )->AsMatrix();
	g_pEyeVec = g_pEffect->GetVariableByName( "Eye" )->AsVector();
	g_pLightVec = g_pEffect->GetVariableByName( "Light" )->AsVector();
	g_pShellNumberShaderVariable = g_pEffect->GetVariableByName( "shell" )->AsScalar();
	g_pNumShellsNumberShaderVariable = g_pEffect->GetVariableByName( "numShells" )->AsScalar();
	g_pShellIncrementShaderVariable = g_pEffect->GetVariableByName( "shellIncrement" )->AsScalar();
	g_pMaxOpacityShaderVariable = g_pEffect->GetVariableByName( "maxOpacity" )->AsScalar();
	g_pCombStrengthShaderVariable = g_pEffect->GetVariableByName( "combStrength" )->AsScalar(); 
	g_pCombVectorShaderVariable = g_pEffect->GetVariableByName( "combVector" )->AsVector();

	//set the variables
	g_pNumShellsNumberShaderVariable->SetInt(g_numShells + 1);
	//g_pShellIncrementShaderVariable->SetFloat(g_shellIncrement);
	g_pMaxOpacityShaderVariable->SetFloat(g_maxOpacity);
	g_pCombStrengthShaderVariable->SetFloat(g_combStrength);
	D3DXVECTOR3 comb( g_combVector.x, g_combVector.y, g_combVector.z );
	D3DXVec3Normalize(&g_combVector,&g_combVector);
	g_pCombVectorShaderVariable->SetFloatVector((float*)&g_combVector);

	// Define the input layout for mesh
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);
	D3DX11_PASS_DESC PassDesc;
	g_pTechniqueRenderShells->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	V_RETURN( pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &g_pVertexLayout ) );

	// Initialize the world matrices
	D3DXMatrixIdentity( &g_World );
	//g_Camera.SetWorldMatrix( g_World );
	//g_World = *(g_Camera.GetWorldMatrix());

	//initialize eye in object space vector  
	D3DXMATRIX worldInverse;
	D3DXMatrixInverse(&worldInverse,NULL,&g_World);
	vectorMatrixMultiply(&eyeInObjectSpace,worldInverse,vecEye);
	g_pEyeVec->SetFloatVector((float*)eyeInObjectSpace);

	//initialize light in object space vector
	vecLight = D3DXVECTOR3(-100.0f,300.0f,-200.0f);
	vectorMatrixMultiply(&lightInObjectSpace,worldInverse,vecLight);
	g_pLightVec->SetFloatVector((float*)lightInObjectSpace);

	//load the textures

	//fin texture
	V_RETURN(loadTextureFromFile(MEDIAPATH(FurTextureFin.dds),"finTexture",pd3dDevice)); 
	//fin offset
	V_RETURN(loadTextureFromFile(MEDIAPATH(FurTextureOffsetFin.dds),"finOffset",pd3dDevice));  
	//fur color base
	V_RETURN(loadTextureFromFile(L"decal.dds","colorTexture",pd3dDevice)); 

	LPCTSTR g_furTextureNames[] = 
	{
		MEDIAPATH(FurTexture00.dds),
		MEDIAPATH(FurTexture01.dds),
		MEDIAPATH(FurTexture02.dds),
		MEDIAPATH(FurTexture03.dds),
		MEDIAPATH(FurTexture04.dds),
		MEDIAPATH(FurTexture05.dds),
		MEDIAPATH(FurTexture06.dds),
		MEDIAPATH(FurTexture07.dds),
		MEDIAPATH(FurTexture08.dds),
		MEDIAPATH(FurTexture09.dds),
		MEDIAPATH(FurTexture10.dds),
		MEDIAPATH(FurTexture11.dds),
		MEDIAPATH(FurTexture12.dds),
		MEDIAPATH(FurTexture13.dds),
		MEDIAPATH(FurTexture14.dds),
		MEDIAPATH(FurTexture15.dds),
	};

	LPCTSTR g_furOffsetNames[] = 
	{
		MEDIAPATH(FurTextureOffset00.dds),
		MEDIAPATH(FurTextureOffset01.dds),
		MEDIAPATH(FurTextureOffset02.dds),
		MEDIAPATH(FurTextureOffset03.dds),
		MEDIAPATH(FurTextureOffset04.dds),
		MEDIAPATH(FurTextureOffset05.dds),
		MEDIAPATH(FurTextureOffset06.dds),
		MEDIAPATH(FurTextureOffset07.dds),
		MEDIAPATH(FurTextureOffset08.dds),
		MEDIAPATH(FurTextureOffset09.dds),
		MEDIAPATH(FurTextureOffset10.dds),
		MEDIAPATH(FurTextureOffset11.dds),
		MEDIAPATH(FurTextureOffset12.dds),
		MEDIAPATH(FurTextureOffset13.dds),
		MEDIAPATH(FurTextureOffset14.dds),
		MEDIAPATH(FurTextureOffset15.dds),
	};

	//fur texture array
	//load all the textures for the fins into one texture array
	ID3D11Texture2D* furTexture = NULL;
	ID3D11ShaderResourceView* furTextureRV = NULL;
	ID3DX11EffectShaderResourceVariable*   furTextureArray = g_pEffect->GetVariableByName( "furTextureArray" )->AsShaderResource();
	V_RETURN( LoadTextureArray( pd3dDevice, g_furTextureNames, sizeof(g_furTextureNames)/sizeof(g_furTextureNames[0]), &furTexture, &furTextureRV) );
	furTextureArray->SetResource( furTextureRV );

	SAFE_RELEASE( furTexture );
	SAFE_RELEASE( furTextureRV );
	furTexture = NULL;
	furTextureRV = NULL;

	//fur offset texture array
	//load all the offset textures for the fins into one texture array
	furTextureArray = g_pEffect->GetVariableByName( "furOffsetArray" )->AsShaderResource();
	V_RETURN( LoadTextureArray( pd3dDevice, g_furOffsetNames, sizeof(g_furOffsetNames)/sizeof(g_furOffsetNames[0]), &furTexture, &furTextureRV) );
	furTextureArray->SetResource( furTextureRV );

	SAFE_RELEASE( furTexture );
	SAFE_RELEASE( furTextureRV );
	furTexture = NULL;
	furTextureRV = NULL;

	//---------------------------------
	//resize swap chain
	//---------------------------------

	//----------------------------------
	// frame move
	//----------------------------------

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D10 device
//--------------------------------------------------------------------------------------
void FurMain::OnD3D11FrameRender1(ID3D11DeviceContext* pd3dImmediateContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{    		
	D3DXMATRIX mWorldView;
	D3DXMATRIX mWorldViewProj;

	//------------------------------------------------------------------------------------
	//render the comb directional control, and update the value of the comb vector
	//------------------------------------------------------------------------------------

	//g_combVector = g_combDirectionWidget.GetLightDirection();
	//D3DXVec3Normalize(&g_combVector,&g_combVector);
	//g_pCombVectorShaderVariable->SetFloatVector((float*)&g_combVector);

	//------------------------------------------------------------------------------------
	//render fur
	//------------------------------------------------------------------------------------

	// Update matrices and vectors used in the fur shaders

	// World matrix
	g_World = worldMatrix;
	g_pWorldVariable->SetMatrix( (float*)&g_World );
	// World*View*Proj matrix
	D3DXMatrixMultiply( &mWorldView, &g_World, &viewMatrix );
	D3DXMatrixMultiply( &mWorldViewProj, &mWorldView, &projectionMatrix );
	g_pWorldViewProj->SetMatrix( (float*)&mWorldViewProj );
	// Eye vec in object space
	CameraClass* cc = CameraManager::getInstance()->getCamera("mainCamera");
	vecEye = cc->GetPosition();

	D3DXMATRIX worldInverse;
	D3DXMatrixInverse(&worldInverse,NULL,&g_World);
	vectorMatrixMultiply(&eyeInObjectSpace,worldInverse,vecEye);
	g_pEyeVec->SetFloatVector((float*)eyeInObjectSpace);
	// Light vec in object space    
	vectorMatrixMultiply(&lightInObjectSpace, worldInverse, vecLight);
	g_pLightVec->SetFloatVector((float*)lightInObjectSpace);

	//set the desired raster state
	g_bToggleWireFrame ? pd3dImmediateContext->RSSetState(g_pRasterStateWireFrame): pd3dImmediateContext->RSSetState(g_pRasterStateSolid);

	//render the mesh, shells, and the fins
	pd3dImmediateContext->IASetInputLayout( g_pVertexLayout );

	//render the mesh
	g_pShellNumberShaderVariable->SetInt(0);
	g_pTechniqueRenderMesh->GetPassByIndex(0)->Apply(0,pd3dImmediateContext);

	//reset the raster state to solid for rendering the UI 
	//pd3dImmediateContext->RSSetState(g_pRasterStateSolid);
}



//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void FurMain::OnD3D11DestroyDevice()
{
	SAFE_RELEASE( g_pEffect );
	SAFE_RELEASE( g_pVertexLayout );
	SAFE_RELEASE( g_pVertexLayoutArrow );
	SAFE_RELEASE( g_pRasterStateWireFrame);
	SAFE_RELEASE( g_pRasterStateSolid);
	SAFE_RELEASE( g_pRasterStateSolidNoCull);
	SAFE_RELEASE( g_MeshLineAdjIB);
	SAFE_RELEASE( g_MeshVB );
}

//--------------------------------------------------------------------------------------
// Helper functions
//--------------------------------------------------------------------------------------

//load a single texture and associated view
HRESULT FurMain::loadTextureFromFile(LPCWSTR file,LPCSTR shaderTextureName, ID3D11Device* pd3dDevice)
{

	HRESULT hr;

	D3DX11_IMAGE_LOAD_INFO texLoadInfo;
	texLoadInfo.MipLevels = 8;
	texLoadInfo.MipFilter = D3DX11_FILTER_TRIANGLE;
	texLoadInfo.Filter = D3DX11_FILTER_TRIANGLE;
	ID3D11Resource *pRes = NULL;

	V_RETURN( D3DX11CreateTextureFromFile(pd3dDevice, file, &texLoadInfo, NULL, &pRes, &hr ) );
	if( pRes )
	{
		ID3D11Texture2D* texture;
		ID3D11ShaderResourceView* textureRview;
		ID3DX11EffectShaderResourceVariable* textureRVar;

		pRes->QueryInterface( __uuidof( ID3D11Texture2D ), (LPVOID*)&texture );
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc( &desc );
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = desc.MipLevels;
		V_RETURN (pd3dDevice->CreateShaderResourceView( texture, &SRVDesc, &textureRview));
		textureRVar = g_pEffect->GetVariableByName( shaderTextureName )->AsShaderResource();
		textureRVar->SetResource( textureRview );

		SAFE_RELEASE( texture );
		SAFE_RELEASE( textureRview );
	}

	SAFE_RELEASE( pRes );

	return S_OK;
}

// loads a texture array and associated view from a series of textures on disk
HRESULT FurMain::LoadTextureArray( ID3D11Device* pd3dDevice, LPCTSTR* szTextureNames, int iNumTextures, ID3D11Texture2D** ppTex2D, ID3D11ShaderResourceView** ppSRV)
{
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(D3D11_TEXTURE2D_DESC) );

	WCHAR szTextureName[MAX_PATH];
	WCHAR str[MAX_PATH];
	for(int i=0; i<iNumTextures; i++)
	{
		//wsprintf(szTextureName, L"%S%.2d.dds", sTexturePrefix, i+offset); 
		//V_RETURN( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, szTextureName ) );

		ID3D11Resource *pRes = NULL;
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		ZeroMemory( &loadInfo, sizeof( D3DX11_IMAGE_LOAD_INFO ) );
		loadInfo.Width = D3DX11_FROM_FILE;
		loadInfo.Height  = D3DX11_FROM_FILE;
		loadInfo.Depth  = D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = 14;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.BindFlags = 0;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
		loadInfo.Filter = D3DX10_FILTER_TRIANGLE;
		loadInfo.MipFilter = D3DX10_FILTER_TRIANGLE;

		D3DX11CreateTextureFromFile( pd3dDevice, szTextureNames[i], &loadInfo, NULL, &pRes, &hr );
		if( pRes )
		{
			ID3D11Texture2D* pTemp;
			pRes->QueryInterface( __uuidof( ID3D11Texture2D ), (LPVOID*)&pTemp );
			pTemp->GetDesc( &desc );

			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			if(loadInfo.Format != desc.Format)   
				return false;

			if(!(*ppTex2D))
			{
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.ArraySize = iNumTextures;
				pd3dDevice->CreateTexture2D( &desc, NULL, ppTex2D);
			}

			ID3D11DeviceContext* pd3dContext;
			pd3dDevice->GetImmediateContext(&pd3dContext);
			for(UINT iMip=0; iMip < desc.MipLevels; iMip++)
			{
				pd3dContext->Map(pTemp, iMip, D3D11_MAP_READ, 0, &mappedTex2D );

				pd3dContext->UpdateSubresource( (*ppTex2D), D3D11CalcSubresource( iMip, i, desc.MipLevels ),NULL,mappedTex2D.pData,mappedTex2D.RowPitch,0 );

				pd3dContext->Unmap(pTemp, iMip);
			}

			SAFE_RELEASE( pRes );
			SAFE_RELEASE( pTemp );
		}
		else
		{
			return false;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
	SRVDesc.Texture2DArray.ArraySize = iNumTextures;
	pd3dDevice->CreateShaderResourceView( *ppTex2D, &SRVDesc, ppSRV );

	return hr;
}

void FurMain::vectorMatrixMultiply(D3DXVECTOR3* vecOut,const D3DXMATRIX matrix,const D3DXVECTOR3 vecIn)
{
	vecOut->x = vecIn.x*matrix(0,0) + vecIn.y*matrix(1,0) + vecIn.z*matrix(2,0) + matrix(3,0);
	vecOut->y = vecIn.x*matrix(0,1) + vecIn.y*matrix(1,1) + vecIn.z*matrix(2,1) + matrix(3,1);
	vecOut->z = vecIn.x*matrix(0,2) + vecIn.y*matrix(1,2) + vecIn.z*matrix(2,2) + matrix(3,2);
}

HRESULT FurMain::LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect )
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

