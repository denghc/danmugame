cbuffer cbPerObject
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};


VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, worldMatrix);
	output.Pos = mul(output.Pos, viewMatrix);
	output.Pos = mul(output.Pos, projectionMatrix);
	output.worldPos = mul(inPos, worldMatrix);

	output.normal = mul(normal, worldMatrix);

    output.TexCoord = inTexCoord;

    return output;
}


float4 PS(VS_OUTPUT input) : SV_TARGET
{
	//return float4(0.0f, 1.0f, 1.0f, 1.0f);
	input.normal = normalize(input.normal);	
	float4 finalColor;
	//If material has a diffuse texture map, set it now
	//if (hasTexture)
		finalColor = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
	//else 
	//	finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	return finalColor;
}
