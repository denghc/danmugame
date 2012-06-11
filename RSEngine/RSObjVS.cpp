#include "RSObjVS.h"

RSObjVS::RSObjVS(string tag) : VertexShaderClass(tag)
{

}

RSObjVS::RSObjVS(void)
{
}


RSObjVS::~RSObjVS(void)
{
}

bool RSObjVS::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* fileName, CHAR* entryFuncName)
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

bool RSObjVS::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, CHAR* entryFuncName)
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

	D3D11_INPUT_ELEMENT_DESC ObjVertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	unsigned int totalLayoutElements = ARRAYSIZE( ObjVertexLayout );

	// Create the vertex input layout.
	result = device->CreateInputLayout(ObjVertexLayout, totalLayoutElements, vertexShaderBuffer->GetBufferPointer(), 
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

bool RSObjVS::InitializeConstantBuffer(ID3D11Device* device)
{
	HRESULT result;

	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(RSObjBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_rsObjBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool RSObjVS::SetRenderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
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
	result = deviceContext->Map(m_rsObjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_rsObjBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_rsObjBuffer);

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);



	return true;
}

void RSObjVS::Shutdown()
{
	ShutdownShader();
	ShutdownConstantBuffer();
}

void RSObjVS::ShutdownConstantBuffer()
{
	if (m_rsObjBuffer)
	{
		m_rsObjBuffer->Release();
		m_rsObjBuffer = 0;
	}
}

void RSObjVS::ShutdownShader()
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