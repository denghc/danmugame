cbuffer cbPerObject: register( b0 )
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

Texture2D ObjTexture : register( t0 );
SamplerState ObjSamplerState : register( s0 );
Texture2D colorTexture : register( t1 );

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct VS_OUTPUT_SCENE
{
    float4 Position          : SV_POSITION;
    float3 Normal            : TEXCOORD0;
    float2 Texture           : TEXCOORD1;
    float3 LightVector       : TEXCOORD2;
    float3 ViewVector        : TEXCOORD3;
    float3 CombVector        : TEXCOORD4;
    int shellNumber          : SHELLNUM;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VS_OUTPUT output;

	inPos *= 6.0f;
	inPos.w = 1.0f;
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
/*
VS_OUTPUT_SCENE VS( float4 input_Position : POSITION, float2 input_Texture : TEXCOORD, float3 input_Normal : NORMAL)
{
     VS_OUTPUT_SCENE output = (VS_OUTPUT_SCENE)0;
     
     float4 color = colorTexture.SampleLevel(samLinear,input_Texture,0);
     // the alpha channel specifies the length of hair 
     float lengthFraction = color.a;
     if(lengthFraction < 0.2) lengthFraction = 0.2;  
     float3 CombVector = combStrength*combVector;
     float3 pos = input_Position.xyz + (input_Normal + CombVector)*shellIncrement*shell*lengthFraction;
     output.Position = mul( float4(pos,1) , WorldViewProj );
     output.Normal = normalize(mul( input_Normal, World ));
     output.Texture = input_Texture;

     //transform the light and eye vectors to tangent space for per pixel lighting 
     float3 eyeVector = Eye - pos;
     output.ViewVector.x = dot(input.Tangent, eyeVector);
     output.ViewVector.y = dot(input.Binormal,eyeVector);
     output.ViewVector.z = dot(input.Normal,  eyeVector);
     float3 lightVector = Light - pos;
     output.LightVector.x = dot(input.Tangent,  lightVector);
     output.LightVector.y = dot(input.Binormal, lightVector);
     output.LightVector.z = dot(input.Normal,   lightVector);
     //transform the comb vector aswell, since this is going to be needed for 
     //transforming the fur tangent in the lighting calculations
      output.CombVector.x = dot(input.Tangent,  CombVector);
     output.CombVector.y = dot(input.Binormal, CombVector);
     output.CombVector.z = dot(input.Normal,   CombVector);

     output.shellNumber = shell-1;
     
     return output;
}

//pixel shader for shells
float4 PSShells(VS_OUTPUT_SCENE In) : SV_Target
{  
    float ka = 0.3;
    float kd = 0.7;
    float ks = 0.2;
    float specPower = 20.0;

    float4 outputColor;
    
    float2 Texture  = In.Texture * textureTilingFactor;
    float4 tangentAndAlpha  = furTextureArray.SampleLevel( samLinear, float3(Texture, In.shellNumber),0 );
    float4 offset   =  furOffsetArray.SampleLevel( samLinear, float3(Texture, In.shellNumber),0 );

    //get the fur color and local fur density
    Texture -= ( offset.xy - 0.5f )*2.0f;
    Texture /= textureTilingFactor;
    outputColor = colorTexture.Sample(samLinear,Texture);
    
    //discard the parts of the mesh marked as no fur, like the eyes and the feet
    if(outputColor.a < 0.01)
        discard;

    //calculate the opacity for alpha blending
    outputColor.a = tangentAndAlpha.a * offset.a; 
   
    //kajiya and kay lighting
    float3 lightVector  = normalize(In.LightVector);
    float3 viewVector   = normalize(In.ViewVector);
    float3 tangentVector= normalize((tangentAndAlpha.rgb - 0.5f) * 2.0f); //this is the tangent to the strand of fur
    tangentVector = normalize(tangentVector + In.CombVector);
    float TdotL = dot( tangentVector , lightVector);
    float TdotE = dot( tangentVector , viewVector);
    float sinTL = sqrt( 1 - TdotL*TdotL );
    float sinTE = sqrt( 1 - TdotE*TdotE );
    outputColor.xyz = ka*outputColor.xyz + kd*sinTL*outputColor.xyz + 
        ks*pow( abs((TdotL*TdotE + sinTL*sinTE)),specPower).xxx;
      
    //banks selfshadowing:
    float minShadow = 0.8;
    float shadow = (float(In.shellNumber)/float(numShells))*(1-minShadow) + minShadow;
    outputColor.xyz *= shadow;
      
    return outputColor;
}
*/
technique11 rsObjectTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0,VS()));
		SetPixelShader(CompileShader(ps_5_0,PS()));
		//SetBlendState(DisableBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        //SetDepthStencilState( EnableDepth, 1 );
	}
}