#include "TestPS.h"


TestPS::TestPS(void)
{
}


TestPS::~TestPS(void)
{
}

bool TestPS::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"Color.ps", "ColorPixelShader");
	if(!result)
	{
		return false;
	}

	return true;

}

bool TestPS::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* psFilename, CHAR* entryFuncName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, entryFuncName, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
		&pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	// Create the vertex shader from the buffer.

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	
	return true;




}

bool TestPS::InitializeConstantBuffer(ID3D11Device*)
{
	return true;
}

void TestPS::Shutdown()
{
	ShutdownShader();

	return;

}

void TestPS::ShutdownShader()
{

}

bool TestPS::SetRenderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView*)
{
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	return true;
}