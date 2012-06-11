#pragma once
#include "Utility.h"
#include "EffectVariable.h"
#include "Geometry.h"
#include "ColorRenderBuffer.h"
#include <vector>
#include <map>
#include <set>

#include "LightningSeed.h"

#include "PathLightning.h"

namespace LightningDemo
{

	namespace Effect = Utility::Effect;
	namespace Geometry = Utility::Geometry;

struct LightningAppearance
{
	D3DXVECTOR3 ColorInside;
	float		ColorFallOffExponent;		// to match HLSL packing rules
	
	D3DXVECTOR3 ColorOutside;
	float		Dummy1;						// dummy to match HLSL padding

	D3DXVECTOR2 BoltWidth;
};

class LightningRenderer
{
public:
	enum 
	{
		DecimationLevels = 2				// number of downsampling steps
	};
	static DXGI_FORMAT BackBufferFormat;

	LightningRenderer(ID3D11Device* device,DXGI_SAMPLE_DESC back_buffer_sample_desc );
	~LightningRenderer();

	PathLightning*		CreatePathLightning(int pattern_mask, unsigned int subdivisions);
	
	void DestroyLightning(LightningSeed* seed);
	
	void SetTime(float time);
	void SetMatrices(const D3DXMATRIX& world, const D3DXMATRIX& view,const D3DXMATRIX& projection);
	void OnRenderTargetResize(unsigned width, unsigned height);

	void Begin();
	void Render(LightningSeed* seed, const LightningAppearance& appearance, float charge, float animation_speed, bool as_lines);
	void End(bool glow, D3DXVECTOR3 blur_sigma);

	ID3D11DepthStencilView*			m_scene_depth_stencil_view;
	ID3D11RenderTargetView*			m_scene_render_target_view;

private:

	void	BuildSubdivisionBuffers();
	Geometry::SimpleVertexBuffer<SubdivideVertex>*		Subdivide(LightningSeed* seed) ;

	void	BuildDownSampleBuffers(unsigned int w, unsigned int h);
	void	DownSample(Utility::ColorRenderBuffer* buffer);

	void	BuildGradientTexture();
	void	SaveViewports();
	void	ResizeViewport(unsigned int w, unsigned int h);
	void	RestoreViewports();

	void	AddLightningSeed(LightningSeed* seed);
	void	RemoveLightningSeed(LightningSeed* seed);

	void DrawQuad(ID3DX11EffectTechnique* technique) ;
	
	D3D11_VIEWPORT  m_viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	D3D11_RECT		m_scissor_rects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	UINT m_num_viewports;
	UINT m_num_scissor_rects;

	ID3D11Device*	m_device;
	ID3DX11Effect*	m_effect;
	DXGI_SAMPLE_DESC m_back_buffer_sample_desc;

	ID3DX11EffectTechnique*	m_tech_bolt_out;
	ID3DX11EffectTechnique*	m_tech_lines_out;
	
	ID3DX11EffectTechnique*	m_tech_add_buffer;
	
	ID3DX11EffectTechnique*	m_tech_blur_buffer_horizontal;
	ID3DX11EffectTechnique*	m_tech_blur_buffer_vertical;
	
	Effect::ConstantBuffer  m_constants_lightning_appearance;
	Effect::ConstantBuffer  m_constants_lightning_structure;

	ID3DX11EffectTechnique*	m_tech_down_sample_2x2;

	std::set<LightningSeed*>	m_lightning_seeds;
	unsigned int				m_max_vertices;

	Effect::FloatVariable m_time;

	Effect::BoolVariable m_fork;
	Effect::IntVariable  m_subdivision_level;
	Effect::FloatVariable	m_animation_speed;
	Effect::FloatVariable	m_charge;

	Effect::MatrixVariable m_world;
	Effect::MatrixVariable m_view;
	Effect::MatrixVariable m_projection;
	Effect::MatrixVariable m_world_view;
	Effect::MatrixVariable m_world_view_projection;

	std::vector<SIZE>	m_down_sample_buffer_sizes;
	std::vector<Utility::ColorRenderBuffer*>		m_down_sample_buffers;

	Effect::ShaderResourceVariable m_buffer;
	Effect::ShaderResourceVariable m_gradient;
	Effect::Vector2Variable	m_buffer_texel_size;
	
	Effect::Vector3Variable		m_blur_sigma;

	ID3D11Texture2D*			m_gradient_texture;
	ID3D11ShaderResourceView*	m_gradient_texture_srv;

	Utility::ColorRenderBuffer		m_original_lightning_buffer;

	Utility::ColorRenderBuffer		m_lightning_buffer0;
	Utility::ColorRenderBuffer		m_lightning_buffer1;

	Utility::ColorRenderBuffer		m_small_lightning_buffer0;
	Utility::ColorRenderBuffer		m_small_lightning_buffer1;

	Geometry::SimpleVertexBuffer<SubdivideVertex>*	m_subdivide_buffer0;
	Geometry::SimpleVertexBuffer<SubdivideVertex>*	m_subdivide_buffer1;
	ID3D11InputLayout* m_subdivide_layout;
};

}