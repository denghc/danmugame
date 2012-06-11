#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <iostream>

namespace D3DXUtils
{
#define COMPILE_TYPE_PS 0x10
#define COMPILE_TYPE_GS 0x11
#define COMPILE_TYPE_VS 0x12
#define COMPILE_TYPE_CS 0x13

	bool CompileShaderFromFile(int compileType, HWND hwnd, WCHAR* vsFileName, CHAR* entryFuncName, 
		ID3D10Blob** vertexShaderBuffer, ID3D10Blob** errorMessage);

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
}
