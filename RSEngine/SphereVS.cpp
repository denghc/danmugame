#include "SphereVS.h"
#include "Structures.h"

SphereVS::SphereVS(void)
{
}


SphereVS::~SphereVS(void)
{
}

bool SphereVS::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* fileName, CHAR* entryFuncName)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, fileName, entryFuncName);
	if(!result)
	{
		return false;
	}

	result = InitializeConstantBuffer(device);
	if(!result)
	{
		return false;
	}
	return true;
}

bool SphereVS::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, CHAR* entryFuncName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	unsigned int numElements;



	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;

	// Compile the vertex shader code.
	bool xresult = D3DXUtils::CompileShaderFromFile(COMPILE_TYPE_VS, hwnd, vsFileName, entryFuncName, &vertexShaderBuffer, &errorMessage);
	if (!xresult)
		return false;

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );

	// Create the vertex input layout.
	result = device->CreateInputLayout(solidColorLayout, totalLayoutElements, vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;



	return true;
}
bool SphereVS::InitializeConstantBuffer(ID3D11Device* device)
{

	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( D3DXMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	result = device->CreateBuffer( &constDesc, 0, &viewCB_ );

	if( FAILED( result ) )
	{
		return false;
	}

	result = device->CreateBuffer( &constDesc, 0, &projCB_ );

	if( FAILED( result ) )
	{
		return false;
	}

	result = device->CreateBuffer( &constDesc, 0, &worldCB_ );

	if( FAILED( result ) )
	{
		return false;
	}

	return true;
}
void SphereVS::Shutdown()
{
	this->ShutdownShader();
	this->ShutdownConstantBuffer();
}
void SphereVS::ShutdownShader()
{
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

bool SphereVS::SetRenderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.

	deviceContext->UpdateSubresource( worldCB_, 0, 0, &worldMatrix, 0, 0 );
	deviceContext->UpdateSubresource( viewCB_, 0, 0, &viewMatrix, 0, 0 );
	deviceContext->UpdateSubresource( projCB_, 0, 0, &projectionMatrix, 0, 0 );

	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );
	deviceContext->VSSetConstantBuffers( 1, 1, &viewCB_ );
	deviceContext->VSSetConstantBuffers( 2, 1, &projCB_ );

	// Unlock the constant buffer.

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);


	return true;
}

void SphereVS::ShutdownConstantBuffer()
{
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	if (viewCB_)
	{
		viewCB_->Release();
		viewCB_ = 0;
	}
	if (projCB_)
	{
		projCB_->Release();
		projCB_ = 0;
	}
	if (worldCB_)
	{
		worldCB_->Release();
		worldCB_= 0;
	}
}