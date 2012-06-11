#include "ComputeShaderClass.h"
#include "ShaderManager.h"

ComputeShaderClass::ComputeShaderClass(string tag)
{
	m_tag = tag;
}

ComputeShaderClass::ComputeShaderClass()
{

}

ComputeShaderClass::~ComputeShaderClass(void)
{
}

bool ComputeShaderClass::Register()
{
	ShaderManager* sm = ShaderManager::GetInstance();
	sm->InsertShader(SHADER_TYPE_CS, this);
	return true;
}
