bool erode;
bool dilate;
bool blur;

float4 QuadPositionTexCoords[] = 
{
	float4(-1,-1,0,1),
	float4(-1,+1,0,0),
	float4(+1,-1,1,1),
	float4(+1,+1,1,0),
};


struct QuadVS2PS
{
	float4	Position : SV_Position;
	float2	TexCoord : TexCoord;
};

void QuadVS( uint VertexId: SV_VertexID, out QuadVS2PS output)
{
	output.Position = float4(QuadPositionTexCoords[VertexId].xy, 0, 1);
	output.TexCoord = QuadPositionTexCoords[VertexId].zw;
}
 
Texture2D<float4> buffer;
float2	buffer_texel_size;


void AddBufferPS (in QuadVS2PS input, out float4 output : SV_Target) 
{
	output = buffer.SampleLevel(LinearSample, input.TexCoord,0);
}

cbuffer BlurParameters
{
	float3 BlurSigma;
};
static const int blur_search_width = 8;

static const int blur_search_start = -blur_search_width;
static const int blur_search_end = blur_search_width + 1;
static const float  blur_scale = 2.0f;


float Gaussian(float2 xy, float sigma)
{
	return exp(- (dot(xy,xy) / (2.0f * sigma * sigma ))) / (2.0f * pi * sigma * sigma);
}

float3 Gaussian(float2 xy, float3 sigma)
{
	float3 sigma_prime = sigma * sigma * 2;
	float3 d = dot(xy,xy);
	 
	return	exp(- d / sigma_prime) / ( pi * sigma_prime);

}
float3 Gaussian(float d, float3 sigma)
{
	float3 sigma_prime = sigma * sigma * 2;
	return	exp(- abs(d) / sigma_prime) / ( pi * sigma_prime);

}

void BlurBufferPS (in QuadVS2PS input, out float4 output : SV_Target, uniform bool horizontal) 
{

	float4 sum =  float4(0,0,0,0);
	

	if(horizontal)
	{
	
		for(int i = blur_search_start;  i < blur_search_end; ++i)
			sum.rgb += Gaussian(i, BlurSigma) * buffer.SampleLevel(LinearSample, input.TexCoord + buffer_texel_size * float2(0.5f + 2.0f * i,0.5f) ,0).rgb;
	}
	else
	{
		for(int i = blur_search_start;  i < blur_search_end; ++i)
			sum.rgb += Gaussian(i, BlurSigma) * buffer.SampleLevel(LinearSample, input.TexCoord + buffer_texel_size * float2(0.5f, 0.5f + 2 * i) ,0).rgb;

	}

	output = blur_scale * sum;
}


technique10 AddBuffer
{
    pass P0
    {       
		SetVertexShader  (CompileShader(vs_4_0, QuadVS()));
        SetGeometryShader(0);
        SetPixelShader   (CompileShader(ps_4_0, AddBufferPS()));
        
        SetRasterizerState(NoCull);
        SetBlendState(AdditiveBlending, float4(1.0f, 1.0f, 1.0f, 1.0f), ~0);
        SetDepthStencilState(DisableDepth, 0);
    }
}

technique10 BlurBufferHorizontal
{
    pass P0
    {       
		SetVertexShader  (CompileShader(vs_4_0, QuadVS()));
        SetGeometryShader(0);
        SetPixelShader   (CompileShader(ps_4_0, BlurBufferPS(true)));
        
        SetRasterizerState(NoCull);
        SetBlendState(NoBlending, float4(1.0f, 1.0f, 1.0f, 1.0f), ~0);
		SetDepthStencilState(DisableDepth, 0);
    }
}
technique10 BlurBufferVertical
{
    pass P0
    {       
		SetVertexShader  (CompileShader(vs_4_0, QuadVS()));
        SetGeometryShader(0);
        SetPixelShader   (CompileShader(ps_4_0, BlurBufferPS(false)));
        
        SetRasterizerState(NoCull);
        SetBlendState(NoBlending, float4(1.0f, 1.0f, 1.0f, 1.0f), ~0);
		SetDepthStencilState(DisableDepth, 0);
    }
}
void DownSample2x2PS (in QuadVS2PS input, out float4 output : SV_Target) 
{
	output = buffer.SampleLevel(LinearSample, input.TexCoord,0);
}

technique10 DownSample2x2
{
    pass P0
    {       
		SetVertexShader  (CompileShader(vs_4_0, QuadVS()));
        SetGeometryShader(0);
        SetPixelShader   (CompileShader(ps_4_0, DownSample2x2PS()));
        
        SetRasterizerState(NoCull);
        SetBlendState(NoBlending, float4(1.0f, 1.0f, 1.0f, 1.0f), ~0);
		SetDepthStencilState(DisableDepth, 0);
    }
}
