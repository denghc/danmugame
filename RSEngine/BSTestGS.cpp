#include "BSTestGS.h"
#include "CameraClass.h"
#include "CameraManager.h"
#include "Structures.h"

BSTestGS::BSTestGS()
{

}

BSTestGS::BSTestGS(std::string tag)
{
	m_tag = tag;
}


BSTestGS::~BSTestGS(void)
{
}


bool BSTestGS::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* gsFilename, CHAR* entryFuncName)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, gsFilename, entryFuncName);
	if(!result)
	{
		return false;
	}

	result = InitializeConstantBuffer(device);
	if (!result)
	{
		return false;
	}

	return true;

}

bool BSTestGS::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* gsFilename, CHAR* entryFuncName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* geometryShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	geometryShaderBuffer = 0;

	// Compile the pixel shader code.
	bool xresult = D3DXUtils::CompileShaderFromFile(COMPILE_TYPE_GS, hwnd, gsFilename, entryFuncName, &geometryShaderBuffer, &errorMessage);
	if (!xresult)
		return false;

	// Create the pixel shader from the buffer.
	result = device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &m_geometryShader);
	if(FAILED(result))
	{
		return false;
	}

	geometryShaderBuffer->Release();
	geometryShaderBuffer = 0;
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.

	return true;




}

bool BSTestGS::InitializeConstantBuffer(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC miscBufferDesc;
	ZeroMemory(&miscBufferDesc, sizeof(miscBufferDesc));
	miscBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	miscBufferDesc.ByteWidth = sizeof(MiscBufferType);
	miscBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	miscBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	miscBufferDesc.MiscFlags = 0;
	miscBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&miscBufferDesc, NULL, &m_miscBuffer);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

void BSTestGS::Shutdown()
{
	ShutdownShader();

	return;

}

void BSTestGS::ShutdownShader()
{

}

bool BSTestGS::SetRenderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView*)
{
	deviceContext->GSSetShader(m_geometryShader, NULL, 0);
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MiscBufferType* dataPtr;

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_miscBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MiscBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	CameraClass* mc = CameraManager::getInstance()->getCamera("mainCamera");
	
	dataPtr->cameraPos = mc->GetPosition();

	// Unlock the constant buffer.
	deviceContext->Unmap(m_miscBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->GSSetConstantBuffers(1, 1, &m_miscBuffer);


	


	return true;
}