float BlendRatio     : register( c0 ) ;
float4 RGBASelect    : register( c1 ) ;
sampler SrcTexture   : register( s0 ) ;
sampler BlendTexture : register( s1 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;


float4 Base_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	float4 BlendColor;
	float4 DestColor;
	
	Color = tex2D( SrcTexture, In.TexCoords0 );
	BlendColor = tex2D( BlendTexture, In.TexCoords1 );
	
	if( RGBASelect.r < 1.0 )
	{
		DestColor.r = Color.r;
	}
	else
	if( RGBASelect.r < 2.0 )
	{
		DestColor.r = Color.g;
	}
	else
	if( RGBASelect.r < 3.0 )
	{
		DestColor.r = Color.b;
	}
	else
	if( RGBASelect.r < 4.0 )
	{
		DestColor.r = Color.a;
	}
	else
	if( RGBASelect.r < 5.0 )
	{
		DestColor.r = BlendColor.r;
	}
	else
	if( RGBASelect.r < 6.0 )
	{
		DestColor.r = BlendColor.g;
	}
	else
	if( RGBASelect.r < 7.0 )
	{
		DestColor.r = BlendColor.b;
	}
	else
	{
		DestColor.r = BlendColor.a;
	}
	
	if( RGBASelect.g < 1.0 )
	{
		DestColor.g = Color.r;
	}
	else
	if( RGBASelect.g < 2.0 )
	{
		DestColor.g = Color.g;
	}
	else
	if( RGBASelect.g < 3.0 )
	{
		DestColor.g = Color.b;
	}
	else
	if( RGBASelect.g < 4.0 )
	{
		DestColor.g = Color.a;
	}
	else
	if( RGBASelect.g < 5.0 )
	{
		DestColor.g = BlendColor.r;
	}
	else
	if( RGBASelect.g < 6.0 )
	{
		DestColor.g = BlendColor.g;
	}
	else
	if( RGBASelect.g < 7.0 )
	{
		DestColor.g = BlendColor.b;
	}
	else
	{
		DestColor.g = BlendColor.a;
	}
	
	if( RGBASelect.b < 1.0 )
	{
		DestColor.b = Color.r;
	}
	else
	if( RGBASelect.b < 2.0 )
	{
		DestColor.b = Color.g;
	}
	else
	if( RGBASelect.b < 3.0 )
	{
		DestColor.b = Color.b;
	}
	else
	if( RGBASelect.b < 4.0 )
	{
		DestColor.b = Color.a;
	}
	else
	if( RGBASelect.b < 5.0 )
	{
		DestColor.b = BlendColor.r;
	}
	else
	if( RGBASelect.b < 6.0 )
	{
		DestColor.b = BlendColor.g;
	}
	else
	if( RGBASelect.b < 7.0 )
	{
		DestColor.b = BlendColor.b;
	}
	else
	{
		DestColor.b = BlendColor.a;
	}
	
	if( RGBASelect.a < 1.0 )
	{
		DestColor.a = Color.r;
	}
	else
	if( RGBASelect.a < 2.0 )
	{
		DestColor.a = Color.g;
	}
	else
	if( RGBASelect.a < 3.0 )
	{
		DestColor.a = Color.b;
	}
	else
	if( RGBASelect.a < 4.0 )
	{
		DestColor.a = Color.a;
	}
	else
	if( RGBASelect.a < 5.0 )
	{
		DestColor.a = BlendColor.r;
	}
	else
	if( RGBASelect.a < 6.0 )
	{
		DestColor.a = BlendColor.g;
	}
	else
	if( RGBASelect.a < 7.0 )
	{
		DestColor.a = BlendColor.b;
	}
	else
	{
		DestColor.a = BlendColor.a;
	}
	
	return DestColor;
}






float4 SR_SG_SB_BR_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 );
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).r;
	
	return Color;
}

float4 SR_SG_SB_BG_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 );
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).g;
	
	return Color;
}

float4 SR_SG_SB_BB_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 );
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).b;
	
	return Color;
}

float4 SR_SG_SB_BA_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 );
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).a;
	
	return Color;
}
























float4 SR_SR_SR_BR_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).rrrr;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).r;
	
	return Color;
}

float4 SG_SG_SG_BR_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).gggg;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).r;
	
	return Color;
}

float4 SB_SB_SB_BR_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).bbbb;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).r;
	
	return Color;
}




float4 SR_SR_SR_BG_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).rrrr;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).g;
	
	return Color;
}

float4 SG_SG_SG_BG_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).gggg;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).g;
	
	return Color;
}

float4 SB_SB_SB_BG_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).bbbb;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).g;
	
	return Color;
}




float4 SR_SR_SR_BB_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).rrrr;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).b;
	
	return Color;
}

float4 SG_SG_SG_BB_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).gggg;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).b;
	
	return Color;
}

float4 SB_SB_SB_BB_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).bbbb;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).b;
	
	return Color;
}




float4 SR_SR_SR_BA_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).rrrr;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).a;
	
	return Color;
}

float4 SG_SG_SG_BA_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).gggg;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).a;
	
	return Color;
}

float4 SB_SB_SB_BA_PS( VS_OUTPUT In ) : Color
{
	float4 Color;
	
	Color = tex2D( SrcTexture, In.TexCoords0 ).bbbb;
	Color.a = tex2D( BlendTexture, In.TexCoords1 ).a;
	
	return Color;
}




























float4 SR_SR_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrrr;
}

float4 SG_SR_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grrr;
}

float4 SB_SR_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brrr;
}

float4 SA_SR_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arrr;
}



float4 SR_SG_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgrr;
}

float4 SG_SG_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggrr;
}

float4 SB_SG_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgrr;
}

float4 SA_SG_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agrr;
}



float4 SR_SB_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbrr;
}

float4 SG_SB_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbrr;
}

float4 SB_SB_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbrr;
}

float4 SA_SB_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abrr;
}



float4 SR_SA_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rarr;
}

float4 SG_SA_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).garr;
}

float4 SB_SA_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).barr;
}

float4 SA_SA_SR_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aarr;
}













float4 SR_SR_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrgr;
}

float4 SG_SR_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grgr;
}

float4 SB_SR_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brgr;
}

float4 SA_SR_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).argr;
}



float4 SR_SG_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rggr;
}

float4 SG_SG_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gggr;
}

float4 SB_SG_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bggr;
}

float4 SA_SG_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aggr;
}



float4 SR_SB_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbgr;
}

float4 SG_SB_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbgr;
}

float4 SB_SB_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbgr;
}

float4 SA_SB_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abgr;
}



float4 SR_SA_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ragr;
}

float4 SG_SA_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gagr;
}

float4 SB_SA_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bagr;
}

float4 SA_SA_SG_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aagr;
}















float4 SR_SR_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrbr;
}

float4 SG_SR_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grbr;
}

float4 SB_SR_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brbr;
}

float4 SA_SR_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arbr;
}



float4 SR_SG_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgbr;
}

float4 SG_SG_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggbr;
}

float4 SB_SG_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgbr;
}

float4 SA_SG_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agbr;
}



float4 SR_SB_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbbr;
}

float4 SG_SB_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbbr;
}

float4 SB_SB_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbbr;
}

float4 SA_SB_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abbr;
}



float4 SR_SA_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rabr;
}

float4 SG_SA_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gabr;
}

float4 SB_SA_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).babr;
}

float4 SA_SA_SB_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aabr;
}




















float4 SR_SR_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrar;
}

float4 SG_SR_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grar;
}

float4 SB_SR_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brar;
}

float4 SA_SR_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arar;
}



float4 SR_SG_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgar;
}

float4 SG_SG_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggar;
}

float4 SB_SG_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgar;
}

float4 SA_SG_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agar;
}



float4 SR_SB_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbar;
}

float4 SG_SB_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbar;
}

float4 SB_SB_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbar;
}

float4 SA_SB_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abar;
}



float4 SR_SA_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).raar;
}

float4 SG_SA_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gaar;
}

float4 SB_SA_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).baar;
}

float4 SA_SA_SA_SR_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aaar;
}





































float4 SR_SR_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrrg;
}

float4 SG_SR_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grrg;
}

float4 SB_SR_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brrg;
}

float4 SA_SR_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arrg;
}



float4 SR_SG_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgrg;
}

float4 SG_SG_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggrg;
}

float4 SB_SG_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgrg;
}

float4 SA_SG_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agrg;
}



float4 SR_SB_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbrg;
}

float4 SG_SB_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbrg;
}

float4 SB_SB_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbrg;
}

float4 SA_SB_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abrg;
}



float4 SR_SA_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rarg;
}

float4 SG_SA_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).garg;
}

float4 SB_SA_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).barg;
}

float4 SA_SA_SR_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aarg;
}













float4 SR_SR_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrgg;
}

float4 SG_SR_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grgg;
}

float4 SB_SR_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brgg;
}

float4 SA_SR_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).argg;
}



float4 SR_SG_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rggg;
}

float4 SG_SG_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gggg;
}

float4 SB_SG_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bggg;
}

float4 SA_SG_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aggg;
}



float4 SR_SB_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbgg;
}

float4 SG_SB_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbgg;
}

float4 SB_SB_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbgg;
}

float4 SA_SB_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abgg;
}



float4 SR_SA_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ragg;
}

float4 SG_SA_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gagg;
}

float4 SB_SA_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bagg;
}

float4 SA_SA_SG_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aagg;
}















float4 SR_SR_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrbg;
}

float4 SG_SR_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grbg;
}

float4 SB_SR_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brbg;
}

float4 SA_SR_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arbg;
}



float4 SR_SG_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgbg;
}

float4 SG_SG_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggbg;
}

float4 SB_SG_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgbg;
}

float4 SA_SG_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agbg;
}



float4 SR_SB_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbbg;
}

float4 SG_SB_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbbg;
}

float4 SB_SB_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbbg;
}

float4 SA_SB_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abbg;
}



float4 SR_SA_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rabg;
}

float4 SG_SA_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gabg;
}

float4 SB_SA_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).babg;
}

float4 SA_SA_SB_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aabg;
}




















float4 SR_SR_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrag;
}

float4 SG_SR_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grag;
}

float4 SB_SR_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brag;
}

float4 SA_SR_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arag;
}



float4 SR_SG_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgag;
}

float4 SG_SG_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggag;
}

float4 SB_SG_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgag;
}

float4 SA_SG_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agag;
}



float4 SR_SB_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbag;
}

float4 SG_SB_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbag;
}

float4 SB_SB_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbag;
}

float4 SA_SB_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abag;
}



float4 SR_SA_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).raag;
}

float4 SG_SA_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gaag;
}

float4 SB_SA_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).baag;
}

float4 SA_SA_SA_SG_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aaag;
}





































float4 SR_SR_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrrb;
}

float4 SG_SR_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grrb;
}

float4 SB_SR_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brrb;
}

float4 SA_SR_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arrb;
}



float4 SR_SG_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgrb;
}

float4 SG_SG_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggrb;
}

float4 SB_SG_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgrb;
}

float4 SA_SG_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agrb;
}



float4 SR_SB_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbrb;
}

float4 SG_SB_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbrb;
}

float4 SB_SB_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbrb;
}

float4 SA_SB_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abrb;
}



float4 SR_SA_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rarb;
}

float4 SG_SA_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).garb;
}

float4 SB_SA_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).barb;
}

float4 SA_SA_SR_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aarb;
}













float4 SR_SR_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrgb;
}

float4 SG_SR_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grgb;
}

float4 SB_SR_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brgb;
}

float4 SA_SR_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).argb;
}



float4 SR_SG_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rggb;
}

float4 SG_SG_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gggb;
}

float4 SB_SG_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bggb;
}

float4 SA_SG_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aggb;
}



float4 SR_SB_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbgb;
}

float4 SG_SB_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbgb;
}

float4 SB_SB_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbgb;
}

float4 SA_SB_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abgb;
}



float4 SR_SA_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ragb;
}

float4 SG_SA_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gagb;
}

float4 SB_SA_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bagb;
}

float4 SA_SA_SG_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aagb;
}















float4 SR_SR_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrbb;
}

float4 SG_SR_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grbb;
}

float4 SB_SR_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brbb;
}

float4 SA_SR_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arbb;
}



float4 SR_SG_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgbb;
}

float4 SG_SG_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggbb;
}

float4 SB_SG_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgbb;
}

float4 SA_SG_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agbb;
}



float4 SR_SB_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbbb;
}

float4 SG_SB_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbbb;
}

float4 SB_SB_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbbb;
}

float4 SA_SB_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abbb;
}



float4 SR_SA_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rabb;
}

float4 SG_SA_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gabb;
}

float4 SB_SA_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).babb;
}

float4 SA_SA_SB_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aabb;
}




















float4 SR_SR_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrab;
}

float4 SG_SR_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grab;
}

float4 SB_SR_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brab;
}

float4 SA_SR_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arab;
}



float4 SR_SG_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgab;
}

float4 SG_SG_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggab;
}

float4 SB_SG_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgab;
}

float4 SA_SG_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agab;
}



float4 SR_SB_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbab;
}

float4 SG_SB_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbab;
}

float4 SB_SB_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbab;
}

float4 SA_SB_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abab;
}



float4 SR_SA_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).raab;
}

float4 SG_SA_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gaab;
}

float4 SB_SA_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).baab;
}

float4 SA_SA_SA_SB_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aaab;
}





































float4 SR_SR_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrra;
}

float4 SG_SR_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grra;
}

float4 SB_SR_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brra;
}

float4 SA_SR_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arra;
}



float4 SR_SG_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgra;
}

float4 SG_SG_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggra;
}

float4 SB_SG_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgra;
}

float4 SA_SG_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agra;
}



float4 SR_SB_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbra;
}

float4 SG_SB_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbra;
}

float4 SB_SB_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbra;
}

float4 SA_SB_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abra;
}



float4 SR_SA_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rara;
}

float4 SG_SA_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gara;
}

float4 SB_SA_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bara;
}

float4 SA_SA_SR_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aara;
}













float4 SR_SR_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrga;
}

float4 SG_SR_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grga;
}

float4 SB_SR_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brga;
}

float4 SA_SR_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arga;
}



float4 SR_SG_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgga;
}

float4 SG_SG_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggga;
}

float4 SB_SG_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgga;
}

float4 SA_SG_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agga;
}



float4 SR_SB_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbga;
}

float4 SG_SB_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbga;
}

float4 SB_SB_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbga;
}

float4 SA_SB_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abga;
}



float4 SR_SA_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).raga;
}

float4 SG_SA_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gaga;
}

float4 SB_SA_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).baga;
}

float4 SA_SA_SG_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aaga;
}















float4 SR_SR_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rrba;
}

float4 SG_SR_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).grba;
}

float4 SB_SR_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).brba;
}

float4 SA_SR_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).arba;
}



float4 SR_SG_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgba;
}

float4 SG_SG_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggba;
}

float4 SB_SG_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgba;
}

float4 SA_SG_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agba;
}



float4 SR_SB_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbba;
}

float4 SG_SB_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbba;
}

float4 SB_SB_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbba;
}

float4 SA_SB_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abba;
}



float4 SR_SA_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).raba;
}

float4 SG_SA_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gaba;
}

float4 SB_SA_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).baba;
}

float4 SA_SA_SB_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aaba;
}




















float4 SR_SR_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rraa;
}

float4 SG_SR_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).graa;
}

float4 SB_SR_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).braa;
}

float4 SA_SR_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).araa;
}



float4 SR_SG_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rgaa;
}

float4 SG_SG_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).ggaa;
}

float4 SB_SG_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bgaa;
}

float4 SA_SG_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).agaa;
}



float4 SR_SB_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).rbaa;
}

float4 SG_SB_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gbaa;
}

float4 SB_SB_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).bbaa;
}

float4 SA_SB_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).abaa;
}



float4 SR_SA_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).raaa;
}

float4 SG_SA_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).gaaa;
}

float4 SB_SA_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).baaa;
}

float4 SA_SA_SA_SA_PS( VS_OUTPUT In ) : Color
{
	return tex2D( SrcTexture, In.TexCoords0 ).aaaa;
}





