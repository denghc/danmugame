// Main effect file. 
//	- Contains shared stuff 
//	- includes various effect files, split by context

// uniforms
cbuffer PerFrame
{
	matrix world;
	matrix view;
	matrix projection;
	
	matrix world_view;
	matrix world_view_projection;
	
	float time;
}

// Renderstates 
DepthStencilState DisableDepth
{
    DepthEnable = false;
    DepthWriteMask = 0;
};
DepthStencilState DisableDepthWrite
{
    DepthEnable = true;
    DepthWriteMask = 0;
};

DepthStencilState EnableDepth
{
    DepthEnable = true;
    DepthWriteMask = All;
    
};

RasterizerState	NoCull
{ 
	CullMode = None;
};

BlendState AdditiveBlending
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = Add;
    SrcBlendAlpha = Zero;
    DestBlendAlpha = Zero;
    BlendOpAlpha = Add;
    RenderTargetWriteMask[0] = 0x0F;
};
BlendState MaximumBlending
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = Max;
    SrcBlendAlpha = Zero;
    DestBlendAlpha = Zero;
    BlendOpAlpha = Add;
    RenderTargetWriteMask[0] = 0x0F;
};
BlendState NoBlending
{
    BlendEnable[0] = false;
};

SamplerState PointSample 
{
	Filter = Min_Mag_Mip_Point;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState LinearSample 
{
	Filter = Min_Mag_Mip_Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};


static const float e = 2.71828183f;
static const float pi = 3.14159265f;

#include "Lightning.hlsl"
#include "Imaging.hlsl"
