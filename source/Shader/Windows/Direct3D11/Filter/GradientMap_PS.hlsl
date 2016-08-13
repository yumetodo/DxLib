#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 RGBToY ;
} ;

SamplerState g_SrcSampler           : register( s0 ) ;
Texture2D    g_SrcTexture           : register( t0 ) ;

SamplerState g_GradientMapSampler   : register( s1 ) ;
Texture2D    g_GradientMapTexture   : register( t1 ) ;

float4 GradientMap_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 SrcColor ;

	SrcColor = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	DestColor = g_GradientMapTexture.Sample( g_GradientMapSampler, float2( dot( SrcColor.rgb, RGBToY.rgb ), 0 ) ) ;
	DestColor.a *= SrcColor.a ;

	return DestColor ;
}

float4 GradientMapRev_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 DestColor ;
	float4 SrcColor ;

	SrcColor = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	DestColor = g_GradientMapTexture.Sample( g_GradientMapSampler, float2( 1.0f - dot( SrcColor.rgb, RGBToY.rgb ), 0 ) ) ;
	DestColor.a *= SrcColor.a ;

	return DestColor ;
}
