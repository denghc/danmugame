#pragma once

#include <vector>

class TextureClass;
class TextureManager
{
public:
	
	~TextureManager(void);
	void Initialize();
	void Shutdown();
	void InsertTexture(TextureClass* tc);
	void RemoveTexture(TextureClass* tc);
	TextureClass* GetTexture(const char* textureName);
	void CreateAndInsertTexture(const char* fileName);

	static TextureManager* GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new TextureManager();
		}
		return m_instance;
	}

private:
	TextureManager(void);

private:
	std::vector<TextureClass*> m_textureList;
	static TextureManager* m_instance;
};

