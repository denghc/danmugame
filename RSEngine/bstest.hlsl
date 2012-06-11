/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer MiscBuffer : register(b1)
{
	float3 cameraPos;
	//TODO: fill this struct
}
Texture2D shaderTexture;
SamplerState SampleType;
//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
};

struct GeometryInputType
{
    float4 position : SV_POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
GeometryInputType ColorVertexShader(VertexInputType input)
{
    GeometryInputType output;
	//output.position.xyzw = input.position.xyzw;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    
	output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Geometry Shader
////////////////////////////////////////////////////////////////////////////////
[maxvertexcount(6)]
void GS( point GeometryInputType input[1], inout TriangleStream<PixelInputType> TriStream )
{
    PixelInputType output;
    float len = 0.02f;
    //
    // Calculate the face normal
    //

	matrix mvp = mul(viewMatrix, projectionMatrix);
    float4 xvec = float4(0.75f, 0.0f, 0.0f, 1.0f);
	float4 yvec = float4(0.0f, 1.0f, 0.0f, 1.0f);
	/*
	float4 cap = float4(cameraPos, 1.0f);
	cap = normalize(cap);
	yvec = float4(cross(xvec.xyz, cap.xyz), 1.0f);
	yvec = normalize(yvec);
	*/
    //
    // Output the pyramid
    //
    
	output.position = input[0].position - xvec * len / 2 + yvec * len / 2;
	//output.position = mul(output.position, mvp);
    output.tex = float2(0.0f, 0.0f);
	output.position.w = 1.0f;
    TriStream.Append( output );

	output.position = input[0].position + xvec * len / 2 + yvec * len / 2;
	//output.position = mul(output.position, mvp);
    output.tex = float2(1.0f, 0.0f);
	output.position.w = 1.0f;
    TriStream.Append( output );

    output.position = input[0].position - xvec * len / 2 - yvec * len / 2;
	//output.position = mul(output.position, mvp);
    output.tex = float2(0.0f, 1.0f);
	output.position.w = 1.0f;
    TriStream.Append( output );
	
    TriStream.RestartStrip();

	output.position = input[0].position - xvec * len / 2 - yvec * len / 2;
    output.tex = float2(0.0f, 1.0f);
	//output.position = mul(output.position, mvp);
	output.position.w = 1.0f;
    TriStream.Append( output );

	output.position = input[0].position + xvec * len / 2 + yvec * len / 2;
    output.tex = float2(1.0f, 0.0f);
	//output.position = mul(output.position, mvp);
	output.position.w = 1.0f;
    TriStream.Append( output );   

	output.position = input[0].position + xvec * len / 2 - yvec * len / 2;
    output.tex = float2(1.0f, 1.0f);
	//output.position = mul(output.position, mvp);
	output.position.w = 1.0f;
    TriStream.Append( output );

 
 
    TriStream.RestartStrip();
}



////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	//if (textureColor.r == 1.0f)
	textureColor.a = 0.0f;
    return textureColor;
}
