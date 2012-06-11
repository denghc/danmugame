#pragma once
#include <vector>

class VertexShaderClass;
class PixelShaderClass;
class GeometryShaderClass;
class ComputeShaderClass;
class ShaderClass;

#define SHADER_TYPE_VS 1
#define SHADER_TYPE_PS 2
#define SHADER_TYPE_GS 3
#define SHADER_TYPE_CS 4



class ShaderManager
{
public:
	
	~ShaderManager(void);

	void Initialize();
	bool InsertShader(unsigned int stype, ShaderClass* sc);
	void Shutdown();
	bool RemoveShader(unsigned int stype, ShaderClass* sc);

	VertexShaderClass* GetVS(std::string tag);
	PixelShaderClass* GetPS(std::string tag);
	ComputeShaderClass* GetCS(std::string tag);
	GeometryShaderClass* GetGS(std::string tag);

	static ShaderManager* GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new ShaderManager();
		}
		return m_instance;
	}

private:
	ShaderManager(void);

private:
	static ShaderManager* m_instance;
	std::vector<VertexShaderClass*> m_VSList;
	std::vector<PixelShaderClass*> m_PSList;
	std::vector<GeometryShaderClass*> m_GSList;
	std::vector<ComputeShaderClass*> m_CSList;
};

