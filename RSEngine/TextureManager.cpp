
#include "TextureManager.h"
#include "TextureClass.h"
#include "GraphicsClass.h"
TextureManager* TextureManager::m_instance = 0;

TextureManager::TextureManager(void)
{
}


TextureManager::~TextureManager(void)
{
}

void TextureManager::Initialize()
{
	this->m_textureList.clear();
}

void TextureManager::Shutdown()
{
	for (std::vector<TextureClass*>::iterator iter = this->m_textureList.begin(); iter != this->m_textureList.end(); ++iter)
	{
		TextureClass* rob = *iter;
		rob->Shutdown();
	}
	this->m_textureList.clear();
}

void TextureManager::InsertTexture(TextureClass* tc)
{
	this->m_textureList.push_back(tc);
}


void TextureManager::RemoveTexture(TextureClass* rob)
{
	for (std::vector<TextureClass*>::iterator iter = this->m_textureList.begin(); iter != this->m_textureList.end(); ++iter)
	{
		if (rob == *iter)
		{
			this->m_textureList.erase(iter);
			return;
		}
	}
}

TextureClass* TextureManager::GetTexture(const char* textureName)
{
	for (std::vector<TextureClass*>::iterator iter = this->m_textureList.begin(); iter != this->m_textureList.end(); ++iter)
	{
		if ((*iter)->NameEqual(textureName))
		{
			return (*iter);
		}
	}
	return NULL;
}

/*
	create a texture from file
	or if the texture already exists
	just return
*/
void TextureManager::CreateAndInsertTexture(const char* fileName)
{
	for (std::vector<TextureClass*>::iterator iter = this->m_textureList.begin(); iter != this->m_textureList.end(); ++iter)
	{
		if ((*iter)->NameEqual(fileName))
		{
			return;// already exists
		}
	}

	//else create

	TextureClass* tc = new TextureClass();

	tc->Initialize(GraphicsClass::m_D3D->GetDevice(), (char*)fileName);
	m_textureList.push_back(tc);
}