#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4 g_MinB_ScaleB ;
} ;

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;

float4 BrightnessScale_PS(STRETCHRECT_VS_OUTPUT In) : SV_TARGET0
{
	float4 Color;
	float3 Hsb;
	float3 Rgb;
	float Max;
	float Min;
	float Delta;
	float3 Half;
	float3 One;

	Half = 0.5f;
	One  = 1.0f;

	Color = g_SrcTexture.Sample( g_SrcSampler, In.TexCoords0 ) ;
	Rgb = Color.rgb;
	
	Max = max( Rgb.r, max( Rgb.g, Rgb.b ) );
	Min = min( Rgb.r, min( Rgb.g, Rgb.b ) );
	Delta = Max - Min;

	Hsb.z = ( Max + Min ) / 2.0;
	if( Max == Min )
	{
		Hsb.y = 0.0f;
		
		Rgb.r = 1.0f;
		Rgb.g = 0.0f;
		Rgb.b = 0.0f;
	}
	else
	{
		float3 tRGB;
		float t;

		if( Hsb.z > 0.5 )
		{
			Hsb.y = 2 - Max - Min; 
		}
		else
		{
			Hsb.y = Max + Min;
		}
		Hsb.y = Delta / Hsb.y;
		
		tRGB = ( float3( Max, Max, Max ) - Rgb ) / Delta;
		if( Rgb.r == Max )
		{
			Rgb.r = 1.0f;
			t = tRGB.b - tRGB.g;
			if( tRGB.b > tRGB.g )
			{
				Rgb.b = 0.0f;
				Rgb.g = t ;
			}
			else
			{
				Rgb.b = -t ;
				Rgb.g = 0.0f;
			}
		}
		else
		if( Rgb.g == Max )
		{
			Rgb.g = 1.0f;
			t = tRGB.r - tRGB.b;
			if( tRGB.r > tRGB.b )
			{
				Rgb.r = 0.0f;
				Rgb.b = t ;
			}
			else
			{
				Rgb.r = -t ;
				Rgb.b = 0.0f;
			}
		}
		else
		{
			Rgb.b = 1.0f;
			t = tRGB.g - tRGB.r;
			if( tRGB.g > tRGB.r )
			{
				Rgb.g = 0.0f;
				Rgb.r = t ;
			}
			else
			{
				Rgb.g = -t ;
				Rgb.r = 0.0f ;
			}
		}
	}

	Rgb += ( Half - Rgb ) * ( 1.0f - Hsb.y );

	Hsb.z = saturate( ( Hsb.z - g_MinB_ScaleB.x ) * g_MinB_ScaleB.y ) ;

	if( Hsb.z <= 0.5f )
	{
		Rgb *= Hsb.z * 2.0f;
	}
	else
	{
		Rgb += ( One - Rgb ) * ( ( Hsb.z - 0.5f ) * 2.0f );
	}
	
	Color.r = Rgb.r;
	Color.g = Rgb.g;
	Color.b = Rgb.b;
	
	return Color;
}
