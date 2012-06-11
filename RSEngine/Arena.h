#pragma once
#include "Utility.h"
#include "EffectVariable.h"
#include "Geometry.h"
#include "LightningRenderer.h"

namespace LightningDemo
{
	namespace Effect = Utility::Effect;
	namespace Geometry = Utility::Geometry;

class Arena
{
public:
	Arena(ID3D11Device* device,DXGI_SAMPLE_DESC back_buffer_sample_desc);
	~Arena();

	void Matrices(const D3DXMATRIX& view, const D3DXMATRIX& projection);
	void Time(float time, float delta_time);
	void RenderTargetResize(unsigned width, unsigned height);
	
	void Render();

	struct ArenaSettings
	{
		ArenaSettings():
		Glow(false),
		AnimationSpeed(10),BlurSigma(1.0f,1.0f,1.0f)
	{
	}

	float   AnimationSpeed;
	bool	Glow;
	D3DXVECTOR3	BlurSigma;

	} Settings;

	LightningRenderer	m_lightning_renderer;

private:

	void ReadSeeds();
	void CreateLightning();

	DXGI_SAMPLE_DESC m_back_buffer_sample_desc;
	
	ID3D11Device*		m_device;

	PathLightning*		m_inter_coil_lightning;
	PathLightning*		m_fence_lightning;

	LightningAppearance m_red_beam;
	LightningAppearance m_blue_beam;

	float m_time;
};


}
	