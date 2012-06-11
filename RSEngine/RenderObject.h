#pragma once
#include <d3d11.h>
#include <d3dx10math.h>

class RenderObject
{
public:
	RenderObject(void);
	virtual ~RenderObject(void);

	void SetVisible(bool v);
	bool IsVisible();
	virtual void Shutdown() = 0;

	// world matrix is not parameter anymore
	virtual void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX) = 0;// just one call to handle all the render related logic

	virtual bool Initialize(ID3D11Device*) = 0;
	//virtual bool Update() = 0;
	virtual bool Update() = 0;
	bool Is2D();

protected:
	D3DXMATRIX m_worldMatrix;
	bool m_visible;
	bool m_isOrtho;
};

