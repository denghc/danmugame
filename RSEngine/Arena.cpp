#include "Arena.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

namespace LightningDemo
{

Arena::Arena(ID3D11Device* device,DXGI_SAMPLE_DESC back_buffer_sample_desc):
	m_device(device),
	m_lightning_renderer(device,back_buffer_sample_desc),
	m_back_buffer_sample_desc(back_buffer_sample_desc)

{
	m_device->AddRef();

	CreateLightning();
}

void Arena::CreateLightning()
{
	LightningStructure inter_coil_structure;
	{
		
		inter_coil_structure.ZigZagFraction = D3DXVECTOR2(0.45f, 0.55f);

		inter_coil_structure.ZigZagDeviationRight = D3DXVECTOR2(-5.0f,5.0f);		
		inter_coil_structure.ZigZagDeviationUp = D3DXVECTOR2(-5.0f,5.0f);

		inter_coil_structure.ZigZagDeviationDecay = 0.5f;
		
		
		inter_coil_structure.ForkFraction = D3DXVECTOR2(0.45f, 0.55f);

		inter_coil_structure.ForkZigZagDeviationRight = D3DXVECTOR2(-1.0f,1.0f);
		inter_coil_structure.ForkZigZagDeviationUp = D3DXVECTOR2(-1.0f,1.0f);
		inter_coil_structure.ForkZigZagDeviationDecay = 0.5f;
		
		
		inter_coil_structure.ForkDeviationRight = D3DXVECTOR2(-1.0f,1.0f);
		inter_coil_structure.ForkDeviationUp = D3DXVECTOR2(-1.0f,1.0f);
		inter_coil_structure.ForkDeviationForward = D3DXVECTOR2(0.0f,1.0f);
		inter_coil_structure.ForkDeviationDecay = 0.5f;
		
		inter_coil_structure.ForkLength = D3DXVECTOR2(1.0f,2.0f);
		inter_coil_structure.ForkLengthDecay = 0.01f;
	}

	LightningStructure fence_structure;
	{
		
		fence_structure.ZigZagFraction = D3DXVECTOR2(0.45f, 0.55f);

		fence_structure.ZigZagDeviationRight = D3DXVECTOR2(-1.0f,1.0f);		
		fence_structure.ZigZagDeviationUp = D3DXVECTOR2(-1.0f,1.0f);

		fence_structure.ZigZagDeviationDecay = 0.5f;
		
		
		fence_structure.ForkFraction = D3DXVECTOR2(0.45f, 0.55f);

		fence_structure.ForkZigZagDeviationRight = D3DXVECTOR2(-1.0f,1.0f);
		fence_structure.ForkZigZagDeviationUp = D3DXVECTOR2(-1.0f,1.0f);
		fence_structure.ForkZigZagDeviationDecay = 0.5f;
		
		
		fence_structure.ForkDeviationRight = D3DXVECTOR2(-1.0f,1.0f);
		fence_structure.ForkDeviationUp = D3DXVECTOR2(-1.0f,1.0f);
		fence_structure.ForkDeviationForward = D3DXVECTOR2(-1.0f,1.0f);
		fence_structure.ForkDeviationDecay = 0.5f;
		
		fence_structure.ForkLength = D3DXVECTOR2(1.0f,2.0f);
		fence_structure.ForkLengthDecay = 0.01f;
	}

	{
		m_blue_beam.BoltWidth = D3DXVECTOR2(0.125f,0.5f);
		m_blue_beam.ColorInside = D3DXVECTOR3(1,1,1);
		m_blue_beam.ColorOutside = D3DXVECTOR3(0,0,1);
		m_blue_beam.ColorFallOffExponent = 2.0f;
	}

	{
		m_red_beam.BoltWidth = D3DXVECTOR2(0.5f,0.5f);
		m_red_beam.ColorInside =  D3DXVECTOR3(1,1,1);
		m_red_beam.ColorOutside = D3DXVECTOR3(1,0,0);
		m_red_beam.ColorFallOffExponent = 5.0f;
	}

	{
		ReadSeeds();
		m_inter_coil_lightning->Structure = inter_coil_structure;
		m_fence_lightning->Structure = fence_structure;
	}

}

Arena::~Arena()
{
	m_lightning_renderer.DestroyLightning(m_fence_lightning);
	m_lightning_renderer.DestroyLightning(m_inter_coil_lightning);

	m_device->Release();
}

void Arena::Matrices(const D3DXMATRIX& view, const D3DXMATRIX& projection)
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	m_lightning_renderer.SetMatrices(world, view, projection);
}

void Arena::Time(float time, float delta_time)
{
	m_time = time;
	m_lightning_renderer.SetTime(time);
}

void Arena::RenderTargetResize(unsigned width, unsigned height)
{
	m_lightning_renderer.OnRenderTargetResize(width, height);
}

void Arena::Render()
{
	m_lightning_renderer.Begin();

	if(false){
		m_lightning_renderer.Render(m_fence_lightning,m_red_beam,1.0f,Settings.AnimationSpeed, false);
	}
	if(true){
		m_lightning_renderer.Render(m_inter_coil_lightning,m_blue_beam,1.0f,Settings.AnimationSpeed, false);
	}

	//m_lightning_renderer.End(Settings.Glow, Settings.BlurSigma);
}

struct SeedRecord
{
	SeedRecord():
		Closed(false)
	{
	}
	std::string Name;

	std::vector<D3DXVECTOR3> Vertices;
	std::vector<D3DXVECTOR3> InterpolatedVertices;

	bool Closed;
};

void Arena::ReadSeeds()
{
	m_fence_lightning =   m_lightning_renderer.CreatePathLightning(0x00,5);
	m_inter_coil_lightning =  m_lightning_renderer.CreatePathLightning(0x08,5);
}

}