#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#include "fontclass.h"
#include "fontshaderclass.h"
#include "RenderObject.h"
#include <vector>


/*
	class for handling text
	supports add\delete\find\edit strings
*/

class TextClass : public RenderObject
{
private:

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();
	void SetAttributes(int, int, D3DXMATRIX);
	bool Initialize(ID3D11Device* device);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	virtual bool Update();

private:
	bool InitializeSentence(SentenceType**, std::string, unsigned int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	bool UpdateSentence(SentenceType*, char*, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);


// for high engine invoke
public:

	bool AddText(std::string tag, unsigned int positionX, unsigned int positionY, float red,
		float green, float blue);
	bool DeleteText(std::string tag);
	// can only change the text content
	bool EditText(std::string tag, std::string newText, unsigned int positionX, unsigned int positionY, float red,
		float green, float blue);
	bool EditText(std::string tag, std::string newText);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;
	std::vector<SentenceType*> m_sentenceList;
	//SentenceType* m_sentence1;
	//SentenceType* m_sentence2;

private:
	ID3D11Device* m_device;
};

#endif