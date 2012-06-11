////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();
	bool Initialize(ID3D11Device*, char*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();
	bool NameEqual(const char* name);
private:
	ID3D11ShaderResourceView* m_texture;
	char m_textureFileName[200];
};

#endif