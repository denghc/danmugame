#include "PathLightning.h"
using namespace std;

namespace LightningDemo
{
PathLightning::PathLightning(ID3DX11Effect* effect,int pattern_mask, unsigned int subdivisions):
	LightningSeed
	(
		effect,
		effect->GetTechniqueByName("Subdivide"),
		effect->GetTechniqueByName("Subdivide"),
		pattern_mask,
		subdivisions
	),
	m_path_segments(0),
	m_subdivide_layout(0)

{
	m_destAngle = 0.0f;
	UpdateSegments();
	m_subdivide_layout = Geometry::CreateInputLayout(m_device, SubdivideVertex::GetLayout(),m_tech_subdivide);
}

PathLightning::~PathLightning()
{
	m_subdivide_layout->Release();
	delete m_path_segments;
}

void PathLightning::UpdateSegments()
{
	delete m_path_segments;

	std::vector<SubdivideVertex> vertices;
	std::vector<LightningPathSegment> segments2;
	const float r = 50.0f;
	const float angle = 0.02f;
	for(int i=0; i<40; i++)
	{
		float ra = i*r/2;
		segments2.push_back(LightningPathSegment(
			D3DXVECTOR3(ra*cos(m_destAngle),r*sin(i*angle),ra*sin(m_destAngle)),
			D3DXVECTOR3((ra+r)*cos(m_destAngle),r*sin(i*angle),(ra+r)*sin(m_destAngle)),
			D3DXVECTOR3(0,1,0)));
	}
	std::copy(segments2.begin(), segments2.end(), back_inserter(vertices));
	m_path_segments = new Geometry::SimpleVertexBuffer<SubdivideVertex>(m_device,vertices);
	m_destAngle += 0.01f;
}

}