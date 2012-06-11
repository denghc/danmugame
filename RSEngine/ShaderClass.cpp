////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "shaderclass.h"
#include "Structures.h"

ShaderClass::ShaderClass()
{
}

ShaderClass::ShaderClass(string tag)
{
	m_tag = tag;
}

ShaderClass::ShaderClass(const ShaderClass& other)
{
}


ShaderClass::~ShaderClass()
{
}

bool ShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;



	return true;
}

void ShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool ShaderClass::InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*)
{
	return true;
}



bool ShaderClass::InitializeConstantBuffer(ID3D11Device* device)
{

	return true;
}

void ShaderClass::ShutdownShader()
{


	return;
}



bool ShaderClass::SetRenderParameters()
{


	return true;
}


string ShaderClass::GetTag()
{
	return m_tag;
}