#include "Utils.h"
#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, char* filename)
{
	

	HRESULT result;
	// Load the texture in.
	WCHAR fn[200];
	c2w(fn, 200, filename);
	memcpy(m_textureFileName, filename, 200);


	result = D3DX11CreateShaderResourceViewFromFile(device, fn, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}
	
	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}


bool TextureClass::NameEqual(const char* name)
{
	if (strcmp(name, m_textureFileName) == 0)
		return true;
	return false;
}