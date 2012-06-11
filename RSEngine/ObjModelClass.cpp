#include "ObjModelClass.h"
// class for rendering .obj format file
#include "ObjLoader.h"
#include "Structures.h"
#include "PixelShaderClass.h"
#include "VertexShaderClass.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "TextureClass.h"

ObjModelClass::ObjModelClass(char* objFileName)
{
	m_psList.clear();
	m_vsList.clear();
	m_objFileName = objFileName;
	D3DXMatrixIdentity(&m_worldMatrix);
}

ObjModelClass::ObjModelClass()
{
	D3DXMatrixIdentity(&m_worldMatrix);
}

void ObjModelClass::Shutdown()
{
	ShutdownBuffers();
	return;
}

bool ObjModelClass::Update()
{
	return true;

}

ObjModelClass::~ObjModelClass(void)
{
}

bool ObjModelClass::Initialize(ID3D11Device* device)
{
	bool result;
	hasTexture = true;

	result = InitializeBuffers(device);
	if (FAILED(result))
	{
		return false;
	}

	result = InitializeShaders(device);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

int ObjModelClass::GetIndexCount()
{
	return 0;
}

void ObjModelClass::SetModelAttribute(bool isRH, bool computeNorm)
{
	m_isRHCoorSys = isRH;
	m_computeNormals = computeNorm;
}

bool ObjModelClass::InitializeBuffers(ID3D11Device* device)
{
	// Load the models from the file.
	ObjLoader objModel;
	subsetTextureFileName.clear();

	if( objModel.LoadRSObjModel(m_objFileName, m_isRHCoorSys, m_computeNormals,
		subsetIndexStart, subsetTextureFileName) == false )
	{
		//MessageBox(hwnd, L"Missing Shader File", MB_OK);
		//TRACE0( "Error loading 3D model!" );
		return false;
	}

	// load textures
	TextureManager* tm = TextureManager::GetInstance();
	for (int i = 0; i < subsetTextureFileName.size(); ++i)
	{
		if (subsetTextureFileName[i] != "")
			tm->CreateAndInsertTexture(subsetTextureFileName[i].c_str());
	}


	objModel.GetObjInfo(&m_triangleCount, &m_totalVerts, &m_meshTriangles, &m_subsetCount);

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof( ObjVertex ) * m_totalVerts;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory( &resourceData, sizeof( resourceData ) );

	void* tmp = objModel.GetVetexes();
	resourceData.pSysMem = tmp;

	HRESULT result;
	result = device->CreateBuffer( &vertexDesc, &resourceData, &m_vertexBuffer );

	if( FAILED(result) )
	{
		//DXTRACE_MSG( "Failed to create vertex buffer!" );
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * m_meshTriangles * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
		
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = objModel.GetIndices();
	device->CreateBuffer(&indexBufferDesc, &iinitData, &m_indexBuffer);

	//delete[] vertices;
	objModel.Release( );

	return true;
}


void ObjModelClass::ShutdownBuffers()
{
	
	char* m_objFileName;
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void ObjModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(ObjVertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ObjModelClass::LoadTexture(ID3D11Device*)
{

	return true;
}

void ObjModelClass::ReleaseTexture()
{
	//call texture manager to release
}

void ObjModelClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	PixelShaderClass* tps;
	VertexShaderClass* tvs;
	TextureManager* textureMgr = TextureManager::GetInstance();
	ShaderManager* shaderMgr = ShaderManager::GetInstance();

	RenderBuffers(deviceContext);

	for (int i = 0; i < m_subsetCount; ++i)
	{
		// set ps para
		std::string shaderName = m_psList[i];
		tps = shaderMgr->GetPS(shaderName);

		shaderName = m_vsList[i];
		tvs = shaderMgr->GetVS(shaderName);

		tvs->SetRenderParameters(deviceContext, m_worldMatrix, viewMatrix, projectionMatrix);
	
		std::string tstr = subsetTextureFileName[i];
		TextureClass* ttc = textureMgr->GetTexture(tstr.c_str());
		tps->SetRenderParameters(deviceContext, ttc->GetTexture());
			// set vs para
		int indexStart = subsetIndexStart[i];
		int indexDrawAmount =  subsetIndexStart[i+1] - subsetIndexStart[i];
		deviceContext->DrawIndexed( indexDrawAmount, indexStart, 0 );
		// draw indexed
	}
	return;
}
