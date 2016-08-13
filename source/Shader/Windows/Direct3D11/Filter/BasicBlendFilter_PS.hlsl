#include "../PixelShader.h"

cbuffer cbNormal : register( b3 )
{
	float4  g_BlendRatio ;
};

SamplerState g_SrcSampler   : register( s0 ) ;
Texture2D    g_SrcTexture   : register( t0 ) ;

SamplerState g_BlendSampler : register( s1 ) ;
Texture2D    g_BlendTexture : register( t1 ) ;

float4 Normal_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );
	Color.rgb  = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Normal_AlphaCh_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;
	float  Ta;
	float  RSa;

	Color         = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor    = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );
	BlendColor.a *= g_BlendRatio.r;

	RSa     = ( 1.0f - BlendColor.a );
	Ta      =   RSa * Color.a         + BlendColor.a;
	Color   = ( RSa * Color.a * Color + BlendColor.a * BlendColor ) / Ta;
	Color.a = Ta;

	return Color;
}

float4 Add_AlphaCh_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;
	float  Ta;
	float  RSa;

	Color         = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor    = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );
	BlendColor.a *= g_BlendRatio.r;

	RSa     = ( 1.0f - BlendColor.a );
	Ta      =   Color.a * RSa   + BlendColor.a;
	Color   = ( Color.a * Color + BlendColor.a * BlendColor ) / Ta;
	Color.a = Ta;
	Color   = saturate( Color );

	return Color;
}

float4 Multiple_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	Color.rgb  = lerp( Color.rgb, Color.rgb * BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Difference_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	Color.rgb  = lerp( Color.rgb, saturate( Color.rgb - BlendColor.rgb ), g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Add_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	Color.rgb  = lerp( Color.rgb, saturate( Color.rgb + BlendColor.rgb ), g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Screen_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;
	float3 One;

	One        = 1.0f;
	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	Color.rgb = lerp( Color.rgb, One - (( One - Color.rgb ) * ( One - BlendColor.rgb )), g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Overlay_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	if( Color.r < 0.5 )
	{
		BlendColor.r = Color.r * BlendColor.r * 2.0;
	}
	else
	{
		BlendColor.r = 2.0 * ( Color.r + BlendColor.r - Color.r * BlendColor.r ) - 1.0;
	}

	if( Color.g < 0.5 )
	{
		BlendColor.g = Color.g * BlendColor.g * 2.0;
	}
	else
	{
		BlendColor.g = 2.0 * ( Color.g + BlendColor.g - Color.g * BlendColor.g ) - 1.0;
	}

	if( Color.b < 0.5 )
	{
		BlendColor.b = Color.b * BlendColor.b * 2.0;
	}
	else
	{
		BlendColor.b = 2.0 * ( Color.b + BlendColor.b - Color.b * BlendColor.b ) - 1.0;
	}
	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Dodge_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	if( BlendColor.r <= 0.999 )
	{
		BlendColor.r = Color.r / ( 1.0 - BlendColor.r );
	}
	else
	{
		BlendColor.r = 1.0;
	}

	if( BlendColor.g <= 0.999 )
	{
		BlendColor.g = Color.g / ( 1.0 - BlendColor.g );
	}
	else
	{
		BlendColor.g = 1.0;
	}

	if( BlendColor.b <= 0.999 )
	{
		BlendColor.b = Color.b / ( 1.0 - BlendColor.b );
	}
	else
	{
		BlendColor.b = 1.0;
	}

	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Burn_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	if( BlendColor.r > 0.0001 )
	{
		BlendColor.r = 1.0 - ( 1.0 - Color.r ) / BlendColor.r ;
	}

	if( BlendColor.g > 0.0001 )
	{
		BlendColor.g = 1.0 - ( 1.0 - Color.g ) / BlendColor.g ;
	}

	if( BlendColor.b > 0.0001 )
	{
		BlendColor.b = 1.0 - ( 1.0 - Color.b ) / BlendColor.b ;
	}

	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Darken_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	if( Color.r < BlendColor.r )
	{
		BlendColor.r = Color.r ;
	}

	if( Color.g < BlendColor.g )
	{
		BlendColor.g = Color.g ;
	}

	if( Color.b < BlendColor.b )
	{
		BlendColor.b = Color.b ;
	}

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Lighten_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	if( Color.r > BlendColor.r )
	{
		BlendColor.r = Color.r ;
	}

	if( Color.g > BlendColor.g )
	{
		BlendColor.g = Color.g ;
	}

	if( Color.b > BlendColor.b )
	{
		BlendColor.b = Color.b ;
	}

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 SoftLight_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	if( BlendColor.r < 0.5 )
	{
		BlendColor.r = pow( Color.r, ( 1.0f - BlendColor.r ) * 2.0 ) ;
	}
	else
	{
		BlendColor.r = pow( Color.r, 0.5 / BlendColor.r );
	}

	if( BlendColor.g < 0.5 )
	{
		BlendColor.g = pow( Color.g, ( 1.0f - BlendColor.g ) * 2.0 ) ;
	}
	else
	{
		BlendColor.g = pow( Color.g, 0.5 / BlendColor.g );
	}

	if( BlendColor.b < 0.5 )
	{
		BlendColor.b = pow( Color.b, ( 1.0f - BlendColor.b ) * 2.0 ) ;
	}
	else
	{
		BlendColor.b = pow( Color.b, 0.5 / BlendColor.b );
	}

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 HardLight_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	if( BlendColor.r < 0.5 )
	{
		BlendColor.r = Color.r * BlendColor.r * 2.0;
	}
	else
	{
		BlendColor.r = 2.0 * ( Color.r + BlendColor.r - Color.r * BlendColor.r ) - 1.0;
	}

	if( BlendColor.g < 0.5 )
	{
		BlendColor.g = Color.g * BlendColor.g * 2.0;
	}
	else
	{
		BlendColor.g = 2.0 * ( Color.g + BlendColor.g - Color.g * BlendColor.g ) - 1.0;
	}

	if( BlendColor.b < 0.5 )
	{
		BlendColor.b = Color.b * BlendColor.b * 2.0;
	}
	else
	{
		BlendColor.b = 2.0 * ( Color.b + BlendColor.b - Color.b * BlendColor.b ) - 1.0;
	}

	BlendColor = saturate( BlendColor );

	Color.rgb = lerp( Color.rgb, BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Exclusion_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	Color.rgb = lerp( Color.rgb, Color.rgb + BlendColor.rgb - 2.0 * Color.rgb * BlendColor.rgb, g_BlendRatio.r * BlendColor.a );

	return Color;
}

float4 Multiple_AOnly_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 Color;
	float4 BlendColor;

	Color      = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	BlendColor = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	Color.a = lerp( Color.a, BlendColor.a * Color.a, g_BlendRatio.r );

	return Color;
}

float4 PMA_Multiple_AOnly_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
	float4 PremulColor;
	float4 LinearColor;
	float4 PremulBlendColor;
	float4 LinearBlendColor;
	float4 OutputLinearColor;
	float4 OutputPremulColor;

	PremulColor         = g_SrcTexture.Sample(   g_SrcSampler,   In.TexCoords0 );
	PremulBlendColor    = g_BlendTexture.Sample( g_BlendSampler, In.TexCoords1 );

	LinearColor         = PremulColor      / PremulColor.a;
	LinearColor.a       = PremulColor.a;
	LinearBlendColor    = PremulBlendColor / PremulBlendColor.a;
	LinearBlendColor.a  = PremulBlendColor.a;
	
	OutputLinearColor   = LinearColor;
	OutputLinearColor.a = lerp( LinearColor.a, LinearBlendColor.a * LinearColor.a, g_BlendRatio.r );

	OutputPremulColor   = OutputLinearColor * OutputLinearColor.a;
	OutputPremulColor.a = OutputLinearColor.a;

	return OutputPremulColor;
}








