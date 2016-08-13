#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4  g_ClipBrightness ;
	float4  g_RGBToY ;
	float4  g_ClipFillColor ;
} ;
	
SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;


float4 PS_Less(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;

    Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
    clip( dot( g_RGBToY, Color ) - g_ClipBrightness.r ) ;
	
    return Color;
}

float4 PS_Less_ClipFill(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;

    Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
    if( dot( g_RGBToY, Color ) < g_ClipBrightness.r )
    {
        return g_ClipFillColor;
    }
    else
    {
        return Color;
    }
}

float4 PS_Greater(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;

    Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
    clip( g_ClipBrightness.r - dot( g_RGBToY, Color ) ) ;

    return Color;
}

float4 PS_Greater_ClipFill(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{   
    float4 Color;

    Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
    if( dot( g_RGBToY, Color ) > g_ClipBrightness.r )
    {
        return g_ClipFillColor;
    }
    else
    {
        return Color;
    }
}
