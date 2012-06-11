#include "utility.h"

#pragma warning (disable:4996)
namespace Utility
{
	using namespace std;

	HRESULT LoadEffectFromFile( ID3D11Device* pd3dDevice, WCHAR* szFileName, ID3DX11Effect** ppEffect )
	{
		HRESULT hr = S_OK;

		// Compile the effect file
		ID3DBlob* pBlobFX = NULL;
		ID3DBlob* pErrorBlob = NULL;
		hr = D3DX11CompileFromFile(szFileName, NULL, NULL, NULL, "fx_5_0", NULL, NULL, NULL, &pBlobFX, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			char* err = (char*)pErrorBlob->GetBufferPointer();
			SAFE_RELEASE(pErrorBlob);
			return hr;
		}

		// Create the effect
		hr = D3DX11CreateEffectFromMemory(pBlobFX->GetBufferPointer(), pBlobFX->GetBufferSize(), 0, pd3dDevice, ppEffect);
		if( FAILED(hr) )
		{
			OutputDebugString( TEXT("Failed to load effect file.") );
			return hr;
		}

		SAFE_RELEASE(pBlobFX);
		SAFE_RELEASE(pErrorBlob);

		return S_OK;
	}

	float Random(float lower, float upper)
	{
		return lower  + (upper - lower) * std::rand() / RAND_MAX;
	}

ID3DX11Effect* LoadEffect(ID3D11Device* device, wchar_t* filename)
{ 
	HRESULT hr;
	ID3DX11Effect* effect;

	ID3DBlob* errors = 0;
    hr = LoadEffectFromFile(device,filename,&effect);

	if( FAILED( hr ) )
    {
		if(0 != errors)
			MessageBoxA
			(
				0, 
				(LPCSTR)errors->GetBufferPointer(), 
				"Error", 
				MB_OK 
			);
		else
			MessageBoxA
			(
				0, 
				"error from D3DX11CreateEffectFromFile == 0" , 
				"Error", 
				MB_OK 
			);
		SAFE_RELEASE(errors);
		exit(0);
	}
	else if( 0 != errors)
	{
       OutputDebugStringA((LPCSTR)errors->GetBufferPointer());
       SAFE_RELEASE(errors);
	}
	return effect;
}

namespace Effect
{
	UINT NumPasses(ID3DX11EffectTechnique* technique)
	{
		D3DX11_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		return td.Passes;
	}
}
}


