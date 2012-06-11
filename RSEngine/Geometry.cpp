#include "Geometry.h"

namespace Utility
{
namespace Geometry
{

ID3D11InputLayout* CreateInputLayout(ID3D11Device* device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DX11EffectTechnique* technique)
{
	ID3D11InputLayout* result = 0;
	
    D3DX11_PASS_DESC PassDesc;
    technique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    device->CreateInputLayout( &layout[0], UINT(layout.size()), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &result);
   
	return result;
}
	
} // Geometry
}// Utility