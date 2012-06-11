#pragma once
#include "renderobject.h"
#include "TextureClass.h"

class SpriteVS;
class SpritePS;

class SpriteClass :
	public RenderObject
{
public:
	SpriteClass(void);
	~SpriteClass(void);

	virtual bool Update();
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);// just one call to handle all the render related logic
	virtual bool Initialize(ID3D11Device*);
	virtual bool UpdateAttributes(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int frame = -1);
	void SetAttributes(int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight, D3DXMATRIX baseViewMatrix);
	void SetTexture(TextureClass* texture); // set texture and load texture are 2 alternative choice to bind texture onto the sprite
	virtual bool LoadTexture(ID3D11Device* device, char* fileName);
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool InitializeShaders(ID3D11Device*);
protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
	int m_PosX, m_PosY;// current position
	int m_FrameID; // current frame
	int m_previousFrameID;
	SpritePS* m_ps;
	SpriteVS* m_vs;
	D3DXMATRIX m_baseViewMatrix;
};

