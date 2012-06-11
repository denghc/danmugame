#pragma once
#include "Utility.h"
#include "EffectVariable.h"
#include "Geometry.h"
#include "ColorRenderBuffer.h"
#include <vector>
#include <map>

namespace LightningDemo
{

	namespace Effect = Utility::Effect;
	namespace Geometry = Utility::Geometry;
		struct SubdivideVertex
	{

		SubdivideVertex():
			Start(D3DXVECTOR3(0,0,0)),
			End(D3DXVECTOR3(0,0,0)),
			Up(D3DXVECTOR3(0,0,0)),
			Level(0)
		{
		}

		SubdivideVertex(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR3& up):
			Start(start),
			End(end),
			Up(up),
			Level(0)
			{
			}
		
		D3DXVECTOR3	Start;
		D3DXVECTOR3	End;
		D3DXVECTOR3	Up;


		UINT		Level;


		static std::vector<D3D11_INPUT_ELEMENT_DESC> GetLayout()
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> r;

			r.push_back
			(
				Utility::InputElementDesc
				(
					"Start",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					0, 
					D3D11_INPUT_PER_VERTEX_DATA, 
					0 
				)
			);
			r.push_back
			(
				Utility::InputElementDesc
				(
					"End",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					12, 
					D3D11_INPUT_PER_VERTEX_DATA, 
					0 
				)
			);
			r.push_back
			(
				Utility::InputElementDesc
				(
					"Up",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					12 + 12, 
					D3D11_INPUT_PER_VERTEX_DATA, 
					0 
				)
			);

			r.push_back
			(
				Utility::InputElementDesc
				(
					"Level",
					0,
					DXGI_FORMAT_R32_UINT,
					0,
					12 + 12 + 12, 
					D3D11_INPUT_PER_VERTEX_DATA, 
					0 
				)
			);


			

			return r;
		}
		
	};

	// Geometric properties of a single bolt
	// this one matches the constant buffer layout in the Lightning.fx file
	struct LightningStructure
	{
		// for ZigZag pattern
		D3DXVECTOR2 ZigZagFraction;
		D3DXVECTOR2 ZigZagDeviationRight;		
		
		D3DXVECTOR2 ZigZagDeviationUp;
		float		ZigZagDeviationDecay;
		float Dummy0;						// dummy to match HLSL padding
		
		// for Fork pattern
		D3DXVECTOR2 ForkFraction;
		D3DXVECTOR2 ForkZigZagDeviationRight;
		
		D3DXVECTOR2 ForkZigZagDeviationUp;
		float		ForkZigZagDeviationDecay;
		float Dummy1;						// dummy to match HLSL padding
		
		D3DXVECTOR2 ForkDeviationRight;
		D3DXVECTOR2 ForkDeviationUp;
		
		D3DXVECTOR2 ForkDeviationForward;
		float		ForkDeviationDecay;
		float Dummy2;						// dummy to match HLSL padding

		D3DXVECTOR2	ForkLength;
		float		ForkLengthDecay;

	};

	class LightningRenderer;

	class LightningSeed
	{
		friend class LightningRenderer;
	
	public:

		LightningStructure Structure;
		
	protected:
		unsigned int GetSubdivisions()
		{
			return m_subdivisions;
		}
		virtual unsigned int GetMaxNumVertices()
		{
			
			return GetNumVertices(m_subdivisions);
		}

		unsigned int GetNumBoltVertices(unsigned int level)
		{
			unsigned int result = 1;
			for(unsigned int i = 0; i < level; ++i)
			{
				
				if(m_pattern_mask & ( 1 << i))
					result *= 3;
				else
					result *= 2;
			}
			return result;
		}

		virtual unsigned int GetNumVertices(unsigned int level)
		{
			return 0;
		}

		virtual void SetChildConstants()
		{
		}
		
		void SetConstants()
		{	
			m_constants_lightning_structure = Structure;
			SetChildConstants();

		}

		virtual void RenderFirstPass()
		{
			ID3D11Buffer* zero = 0;
			UINT nought = 0;
			ID3D11DeviceContext* context;
			m_device->GetImmediateContext(&context);

			context->IASetVertexBuffers(0,1,&zero,&nought,&nought);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			context->IASetInputLayout(0);

			m_tech_first_pass->GetPassByIndex(0)->Apply(0,context);
			context->Draw(GetNumVertices(0),0);
		}

		ID3DX11EffectTechnique*		GetFirstPassTechnique()
		{
			return m_tech_first_pass;
		}
		
		ID3DX11EffectTechnique*		GetSubdivideTechnique()
		{
			return m_tech_subdivide;
		}

		int GetPatternMask()
		{
			return m_pattern_mask;
		}

		int		m_pattern_mask;
		unsigned int m_subdivisions;

		ID3D11Device*	m_device;

		ID3DX11Effect*			m_effect;
		ID3DX11EffectTechnique*	m_tech_first_pass;
		ID3DX11EffectTechnique*	m_tech_subdivide;

		Effect::ConstantBuffer  m_constants_lightning_structure;
		
		LightningSeed():
			m_constants_lightning_structure(0,"LightningStructure")
		{
		}
		LightningSeed(ID3DX11Effect* effect, ID3DX11EffectTechnique* first_pass, ID3DX11EffectTechnique* subdivide_pass,  int pattern_mask, unsigned int subdivisions);
		virtual ~LightningSeed();

	private:

	};

}