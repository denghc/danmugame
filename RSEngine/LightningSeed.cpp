#include "LightningSeed.h"
using namespace std;

namespace LightningDemo
{

LightningSeed::LightningSeed(ID3DX11Effect* effect, ID3DX11EffectTechnique* first_pass, ID3DX11EffectTechnique* subdivide, int pattern_mask, unsigned int subdivisions):
	m_device(0),
	m_effect(effect),

	m_tech_first_pass(first_pass),
	m_tech_subdivide(subdivide),

	m_constants_lightning_structure(effect,"LightningStructure"),

	m_pattern_mask(pattern_mask),
	m_subdivisions(subdivisions)

{
	HRESULT hr;

	m_effect->AddRef();

	effect->GetDevice(&m_device);
}


LightningSeed::~LightningSeed(void)
{
	m_effect->Release();
	m_device->Release();

}
}