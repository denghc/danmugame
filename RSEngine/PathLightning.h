#pragma once
#include "Utility.h"
#include "EffectVariable.h"
#include "Geometry.h"
#include "ColorRenderBuffer.h"
#include <vector>
#include <map>

#include "LightningSeed.h"

namespace LightningDemo
{

	namespace Effect = Utility::Effect;
	namespace Geometry = Utility::Geometry;

	struct LightningPathSegment
	{
	LightningPathSegment(const D3DXVECTOR3&	start, const D3DXVECTOR3& end, const D3DXVECTOR3& up):
		Start(start),
		End(end),
		Up(up)
	{
	}
	LightningPathSegment(float sx, float sy, float sz, float ex, float ey, float ez, float ux = 0, float uy = 0, float uz = 1):
		Start(sx,sy,sz),
		End(ex, ey, ez),
		Up(ux, uy, uz)
	{
	}

	operator SubdivideVertex () const
	{
		SubdivideVertex result;

		result.Start = this->Start;
		result.End = this->End;
		result.Up = this->Up;

		result.Level = 0;

		return result;
	}
	
	D3DXVECTOR3	Start;
	D3DXVECTOR3	End;
	D3DXVECTOR3	Up;
};

	class LightningRenderer;

	class PathLightning : public LightningSeed
	{
		friend class LightningRenderer;
	public:

		virtual void RenderFirstPass()
		{

			m_path_segments->BindToInputAssembler();

			ID3D11DeviceContext* context;
			m_device->GetImmediateContext(&context);

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			context->IASetInputLayout(m_subdivide_layout);

			m_tech_first_pass->GetPassByIndex(0)->Apply(0,context);
			context->Draw(GetNumVertices(0),0);
		}

		virtual unsigned int GetNumVertices(unsigned int level)
		{
			return m_path_segments->NumVertices() * GetNumBoltVertices(level);
		}

		PathLightning(ID3DX11Effect* effect, int pattern_mask, unsigned int subdivisions);
		~PathLightning();

		Geometry::SimpleVertexBuffer<SubdivideVertex>* m_path_segments;

		void UpdateSegments();

	private:
		ID3D11InputLayout* m_subdivide_layout;
		float m_destAngle;
	};

}