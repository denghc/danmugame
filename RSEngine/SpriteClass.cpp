#include "SpriteClass.h"
#include "Structures.h"
#include "SpriteVS.h"
#include "SpritePS.h"
#include "SystemClass.h"
#include "ShaderManager.h"
#include "D3DClass.h"

SpriteClass::SpriteClass(void)
{
	D3DXMatrixIdentity(&m_worldMatrix);
	m_isOrtho = true;
}


SpriteClass::~SpriteClass(void)
{

}

void SpriteClass::Shutdown()
{

}

void SpriteClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTextureType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.d
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;

}


bool SpriteClass::Initialize(ID3D11Device* device)
{
	m_previousPosX = -1;
	m_previousPosY = -1;

	bool result;
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	result = InitializeShaders(device);
	if (!result)
	{
		return false;
	}

	return true;


}

bool SpriteClass::Update()
{
	return true;
}

bool SpriteClass::UpdateAttributes(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int frame)
{
	m_previousPosX = m_PosX;
	m_previousPosY = m_PosY;
	m_PosX = positionX;
	m_PosY = positionY;
	//single frame bitmap
	if (frame == -1)
	{
		
	}
	//multi frame bitmap
	else
	{
		m_FrameID = frame;
	}

	// have to update
	if (m_PosX != m_previousPosX || m_PosY != m_previousPosY || m_FrameID != m_previousFrameID)
	{
		
		VertexTextureType* vertices;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexTextureType* verticesPtr;
		HRESULT result;
		float left, right, top, bottom;
		left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

		// Calculate the screen coordinates of the right side of the bitmap.
		right = left + (float)m_bitmapWidth;

		// Calculate the screen coordinates of the top of the bitmap.
		top = (float)(m_screenHeight / 2) - (float)positionY;

		// Calculate the screen coordinates of the bottom of the bitmap.
		bottom = top - (float)m_bitmapHeight;


		/*
			for test
		*/
// 		left = -1.0f;
// 		right = 1.0f;
// 		top = 1.0f;
// 		bottom = -1.0f;


		// Create the vertex array.
		vertices = new VertexTextureType[m_vertexCount];
		if(!vertices)
		{
			return false;
		}

		// Load the vertex array with data.
		// First triangle.
		vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

		vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
		vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

		// Second triangle.
		vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
		vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

		vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

		// Lock the vertex buffer so it can be written to.
		result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if(FAILED(result))
		{
			return false;
		}

		// Get a pointer to the data in the vertex buffer.
		verticesPtr = (VertexTextureType*)mappedResource.pData;

		// Copy the data into the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

		// Unlock the vertex buffer.
		deviceContext->Unmap(m_vertexBuffer, 0);

		// Release the vertex array as it is no longer needed.
		delete [] vertices;
		vertices = 0;


	}
	return true;

}

void SpriteClass::SetAttributes(int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight,
	D3DXMATRIX baseViewMatrix)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;
	m_baseViewMatrix = baseViewMatrix;
}

void SpriteClass::SetTexture(TextureClass* texture)
{
	m_Texture = texture;
}

bool SpriteClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Load the index array with data.
	for(i=0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}
	
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}
	
		// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void SpriteClass::ShutdownBuffers()
{
	// Release the index buffer.
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

	return;
}


void SpriteClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix)
{
	/*
		the world view project matrix has no use!
		we just use the m_posx and m_posy to render directly on screen
		but for the ubiquitous reason, we reserve the matrix attributes
	*/
	//bool result;
	D3DClass::GetInstance()->TurnZBufferOff();
	RenderBuffers(deviceContext);
	m_vs->SetRenderParameters(deviceContext, m_worldMatrix, m_baseViewMatrix, projMatrix);
	m_ps->SetRenderParameters(deviceContext, m_Texture->GetTexture());
	deviceContext->DrawIndexed(m_indexCount, 0, 0);
	D3DClass::GetInstance()->TurnZBufferOn();
}


bool SpriteClass::LoadTexture(ID3D11Device* device, char* fileName)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, fileName);
	if(!result)
	{
		return false;
	}

	return true;
}



bool SpriteClass::InitializeShaders(ID3D11Device* device)
{
	m_vs = new SpriteVS();
	m_vs->Initialize(device, SystemClass::GetWindowHandler(), L"Sprite.fx", "TextureVertexShader");
	ShaderManager::GetInstance()->InsertShader(SHADER_TYPE_VS, m_vs);
	m_ps = new SpritePS();
	m_ps->Initialize(device, SystemClass::GetWindowHandler(), L"sprite.fx", "TexturePixelShader");
	ShaderManager::GetInstance()->InsertShader(SHADER_TYPE_PS, m_ps);
	return true;
}