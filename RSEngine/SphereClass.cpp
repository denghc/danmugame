#include "SphereClass.h"
#include "SpherePS.h"
#include "SphereVS.h"
#include "SystemClass.h"

SphereClass::SphereClass(char* objFileName) : ObjModelClass(objFileName)
{
	m_is2D = false;
	D3DXMatrixScaling(&m_worldMatrix, 5.0f, 5.0f, 5.0f);
}


SphereClass::~SphereClass(void)
{
}

bool SphereClass::LoadTexture(ID3D11Device* device)
{
	m_texture = new TextureClass();
	m_texture->Initialize(device, L"decal.dds");
	return true;
}


void SphereClass::Shutdown()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}
	if (m_ps)
	{
		m_ps->Shutdown();
		delete m_ps;
		m_ps = 0;
	}
	if (m_vs)
	{
		m_vs->Shutdown();
		delete m_vs;
		m_vs = 0;
	}
	this->ShutdownBuffers();
	return;
	
}

bool SphereClass::InitializeShaders(ID3D11Device* device)
{
	m_texture = new TextureClass();
	m_texture->Initialize(device, L"decal.dds");
	m_vs = new SphereVS();
	m_ps = new SpherePS();
	m_vs->Initialize(device, SystemClass::GetWindowHandler());
	m_ps->Initialize(device, SystemClass::GetWindowHandler());
	return true;
}

void SphereClass::ShutdownBuffers()
{

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void SphereClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix)
{
	// do ALL the render works


	RenderBuffers(deviceContext);
	// Render the model using the color shader.
	m_vs->SetRenderParameters(deviceContext, m_worldMatrix, viewMatrix, projectionMatrix);
	m_ps->SetRenderParameters(deviceContext, m_texture->GetTexture());
	deviceContext->Draw(m_vertextCount, 0);
}