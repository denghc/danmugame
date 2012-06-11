///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"
#include "D3DClass.h"

TextClass::TextClass()
{
	D3DXMatrixIdentity(&m_worldMatrix);
	m_Font = 0;
	m_FontShader = 0;
	m_isOrtho = true;
	m_sentenceList.clear();
}


TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}


void TextClass::SetAttributes(int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;
}

bool TextClass::Initialize(ID3D11Device* device)
{
	bool result;

	m_device = device;
	// Store the screen width and height.
	

	// Create the font object.
	m_Font = new FontClass();
	if(!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(m_device, "fontdata.txt", "font.dds");
	if(!result)
	{
		//MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass();
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_device);
	if(!result)
	{
		//MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}
	
	

	return true;
}


void TextClass::Shutdown()
{
	// Release all sentence.
	for (std::vector<SentenceType*>::iterator iter = m_sentenceList.begin(); iter != m_sentenceList.end();
		++iter)
	{
		ReleaseSentence(&(*iter));
	}
	m_sentenceList.clear();
	
	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if(m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


void TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	D3DClass::GetInstance()->TurnZBufferOff();
	D3DClass::GetInstance()->TurnOnAlphaBlending();
	// Draw all sentences in the list.
	for (std::vector<SentenceType*>::iterator iter = m_sentenceList.begin(); iter != m_sentenceList.end();
		++iter)
	{
		result = RenderSentence(deviceContext, *iter, m_worldMatrix, m_baseViewMatrix, orthoMatrix);
	}
	

	D3DClass::GetInstance()->TurnZBufferOn();
	D3DClass::GetInstance()->TurnOffAlphaBlending();
}


bool TextClass::InitializeSentence(SentenceType** sentence, std::string tag, unsigned int maxLength, ID3D11Device* device)
{
	VertexTextureType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	(*sentence)->tag = tag;
	if(!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexTextureType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexTextureType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
							   ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexTextureType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Store the position of the sentence
	sentence->positionX = positionX;
	sentence->positionY = positionY;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexTextureType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexTextureType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexTextureType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureType* verticesPtr;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexTextureType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + sentence->positionX);
	drawY = (float)((m_screenHeight / 2) - sentence->positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexTextureType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
								D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexTextureType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, m_worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), 
								  pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}

bool TextClass::AddText(std::string tag, unsigned int positionX, unsigned int positionY, float red, float green, float blue)
{
	SentenceType* newSt;
	InitializeSentence(&newSt, tag, 200, m_device);
	newSt->positionX = positionX;
	newSt->positionY = positionY;
	newSt->red = red;
	newSt->green = green;
	newSt->blue = blue;
	m_sentenceList.push_back(newSt);
	return true;
}

bool TextClass::DeleteText(std::string tag)
{
	/* find the text */
	for (std::vector<SentenceType*>::iterator iter = m_sentenceList.begin(); iter != m_sentenceList.end();
		++iter)
	{
		if ((*iter)->tag.compare(tag) == 0)
		{
			// found
			m_sentenceList.erase(iter);
			return true;
		}
	}
	// not found, return false
	return false;
}

bool TextClass::EditText(std::string tag, std::string newText, unsigned int positionX, unsigned int positionY, 
	float red, float green, float blue)
{
	for (std::vector<SentenceType*>::iterator iter = m_sentenceList.begin(); iter != m_sentenceList.end();
		++iter)
	{
		if ((*iter)->tag.compare(tag) == 0)
		{
			// found
			ID3D11DeviceContext* deviceContext;
			m_device->GetImmediateContext(&deviceContext);
			UpdateSentence(*iter, (char*)newText.c_str(), positionX, positionY, red, green, blue, deviceContext);
			return true;
		}
	}

	// not found
	return false;
}


bool TextClass::EditText(std::string tag, std::string newText)
{
	for (std::vector<SentenceType*>::iterator iter = m_sentenceList.begin(); iter != m_sentenceList.end();
		++iter)
	{
		if ((*iter)->tag.compare(tag) == 0)
		{
			// found
			ID3D11DeviceContext* deviceContext;
			m_device->GetImmediateContext(&deviceContext);
			UpdateSentence(*iter, (char*)newText.c_str(), deviceContext);
			return true;
		}
	}

	// not found
	return false;
}


bool TextClass::Update()
{
	return true;
}


