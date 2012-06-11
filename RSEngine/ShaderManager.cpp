#include "ShaderManager.h"
#include "ComputeShaderClass.h"
#include "PixelShaderClass.h"
#include "VertexShaderClass.h"
#include "GeometryShaderClass.h"

ShaderManager* ShaderManager::m_instance = 0;

ShaderManager::ShaderManager(void)
{
}


ShaderManager::~ShaderManager(void)
{
}

void ShaderManager::Initialize()
{
	m_VSList.clear();
	m_PSList.clear();
	m_GSList.clear();
	m_CSList.clear();
}

bool ShaderManager::InsertShader(unsigned int stype, ShaderClass* sc)
{
	bool result;

	switch (stype)
	{
	case SHADER_TYPE_CS:
		m_CSList.push_back((ComputeShaderClass*)sc);	
		break;
	case SHADER_TYPE_GS:
		m_GSList.push_back((GeometryShaderClass*)sc);	
		break;
	case SHADER_TYPE_PS:
		m_PSList.push_back((PixelShaderClass*)sc);	
		break;
	case SHADER_TYPE_VS:
		m_VSList.push_back((VertexShaderClass*)sc);	
		break;
	}
	return true;
}

bool ShaderManager::RemoveShader(unsigned int stype, ShaderClass* sc)
{
	switch (stype)
	{
	case SHADER_TYPE_CS:
		for (std::vector<ComputeShaderClass*>::iterator iter = this->m_CSList.begin(); iter != this->m_CSList.end(); ++iter)
		{
			ComputeShaderClass* csc = *iter;
			if (csc == (ComputeShaderClass*)sc)
			{
				m_CSList.erase(iter);
				break;
			}
		}
		break;
	case SHADER_TYPE_PS:
		for (std::vector<PixelShaderClass*>::iterator iter = this->m_PSList.begin(); iter != this->m_PSList.end(); ++iter)
		{
			PixelShaderClass* csc = *iter;
			if (csc == (PixelShaderClass*)sc)
			{
				m_PSList.erase(iter);
				break;
			}
		}
		break;
	case SHADER_TYPE_VS:
		for (std::vector<VertexShaderClass*>::iterator iter = this->m_VSList.begin(); iter != this->m_VSList.end(); ++iter)
		{
			VertexShaderClass* csc = *iter;
			if (csc == (VertexShaderClass*)sc)
			{
				m_VSList.erase(iter);
				break;
			}
		}
		break;
	case SHADER_TYPE_GS:
		for (std::vector<GeometryShaderClass*>::iterator iter = this->m_GSList.begin(); iter != this->m_GSList.end(); ++iter)
		{
			GeometryShaderClass* csc = *iter;
			if (csc == (GeometryShaderClass*)sc)
			{
				m_GSList.erase(iter);
				break;
			}
		}
		break;
	}
	return true;
}

void ShaderManager::Shutdown()
{
	//release all 4 kinds of shaders

	for (std::vector<ComputeShaderClass*>::iterator iter = this->m_CSList.begin(); iter != this->m_CSList.end(); ++iter)
	{
		ComputeShaderClass* csc = *iter;
		csc->Shutdown();
	}
	this->m_CSList.clear();

	for (std::vector<PixelShaderClass*>::iterator iter = this->m_PSList.begin(); iter != this->m_PSList.end(); ++iter)
	{
		PixelShaderClass* csc = *iter;
		csc->Shutdown();
	}
	this->m_PSList.clear();

	for (std::vector<VertexShaderClass*>::iterator iter = this->m_VSList.begin(); iter != this->m_VSList.end(); ++iter)
	{
		VertexShaderClass* csc = *iter;
		csc->Shutdown();
	}
	this->m_VSList.clear();

	for (std::vector<GeometryShaderClass*>::iterator iter = this->m_GSList.begin(); iter != this->m_GSList.end(); ++iter)
	{
		GeometryShaderClass* csc = *iter;
		csc->Shutdown();
	}
	this->m_VSList.clear();
}

VertexShaderClass* ShaderManager::GetVS(string tag)
{
	for (std::vector<VertexShaderClass*>::iterator iter = this->m_VSList.begin(); iter != this->m_VSList.end(); ++iter)
	{
		VertexShaderClass* csc = *iter;
		if (tag.compare(csc->GetTag()) == 0)
		{
			return csc;
		}
	}
	return NULL;
}

PixelShaderClass* ShaderManager::GetPS(string tag)
{
	for (std::vector<PixelShaderClass*>::iterator iter = this->m_PSList.begin(); iter != this->m_PSList.end(); ++iter)
	{
		PixelShaderClass* csc = *iter;
		if (tag.compare(csc->GetTag()) == 0)
		{
			return csc;
		}
	}
	return NULL;
}

ComputeShaderClass* ShaderManager::GetCS(string tag)
{
	for (std::vector<ComputeShaderClass*>::iterator iter = this->m_CSList.begin(); iter != this->m_CSList.end(); ++iter)
	{
		ComputeShaderClass* csc = *iter;
		if (tag.compare(csc->GetTag()) == 0)
		{
			return csc;
		}
	}
	return NULL;
}

GeometryShaderClass* ShaderManager::GetGS(string tag)
{
	for (std::vector<GeometryShaderClass*>::iterator iter = this->m_GSList.begin(); iter != this->m_GSList.end(); ++iter)
	{
		GeometryShaderClass* csc = *iter;
		if (tag.compare(csc->GetTag()) == 0)
		{
			return csc;
		}
	}
	return NULL;
}