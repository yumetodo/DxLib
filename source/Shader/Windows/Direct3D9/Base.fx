float4 FactorColor    : register( c5 ) ;
float4 ZeroHalfOneTwo : register( c0 ) ;

sampler SrcTexture          : register( s0 ) ;
sampler BlendTexture        : register( s1 ) ;
sampler RenderTargetTexture : register( s2 ) ;

struct VS_OUTPUT
{
	float4 Position    : POSITION ;
	float4 Diffuse     : COLOR0 ;
	float4 Specular    : COLOR1 ;
	float2 TexCoords0  : TEXCOORD0 ;
	float2 TexCoords1  : TEXCOORD1 ;
	float2 TexCoords2  : TEXCOORD2 ;
};


// ���l�����v�Z
float4 ps_alpha_ch_blend( float4 SrcColor, float2 RTTexCoords )
{
	float4 DestColor;
	float4 RetColor;
	float RSa;

	DestColor = tex2D( RenderTargetTexture, RTTexCoords );
	RSa = DestColor.a * ( 1.0f - SrcColor.a );
	RetColor.a = SrcColor.a + RSa;
	if( RetColor.a < 0.001f )
	{
		RetColor.r = 0.0f;
		RetColor.g = 0.0f;
		RetColor.b = 0.0f;
	}
	else
	{
		RetColor.rgb = ( RSa * DestColor.rgb + SrcColor.a * SrcColor.rgb ) / RetColor.a;
	}

	return RetColor;
}




// �e�N�X�`���Ȃ�
float4 ps_notex_normal( VS_OUTPUT In ) : COLOR
{
	float4 DestColor ;

#ifdef INVERSE_COLOR
	DestColor.rgb = ZeroHalfOneTwo.zzz - In.Diffuse.rgb ;
	DestColor.a = In.Diffuse.a ;
#else
	DestColor = In.Diffuse ;
#endif

#ifdef PRE_MUL_ALPHA
	DestColor.rgb *= In.Diffuse.a ;
#endif

#ifdef MUL_X_4
	DestColor.rgb *= 4.0f ;
#endif
#ifdef MUL_BLEND
	DestColor = lerp( ZeroHalfOneTwo.zzzz, DestColor, DestColor.a ) ;
#endif

#ifdef ALPHA_CH_DRAW
	DestColor = ps_alpha_ch_blend( DestColor, In.TexCoords0 ) ;
#endif

	return DestColor ;
}


// �u�����h�e�N�X�`������
float4 ps_normal( VS_OUTPUT In ) : COLOR
{
	float4 TexColor ;
	float4 DestColor ;

	TexColor = tex2D( SrcTexture, In.TexCoords0 ) ;
#ifdef IGNORE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz;
#endif
#ifdef INVERSE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz - TexColor.rgb;
#endif

	DestColor = TexColor * In.Diffuse ;

#ifdef IGNORE_TEXALPHA
	DestColor.a = In.Diffuse.a ;
#endif

#ifdef PRE_MUL_ALPHA
	DestColor.rgb *= In.Diffuse.a ;
#endif

#ifdef MUL_X_4
	DestColor.rgb *= 4.0f ;
#endif
#ifdef MUL_BLEND
	DestColor = lerp( ZeroHalfOneTwo.zzzz, DestColor, DestColor.a ) ;
#endif

#ifdef ALPHA_CH_DRAW
	return ps_alpha_ch_blend( DestColor, In.TexCoords1 ) ;
#else
	return DestColor ;
#endif
}

//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_NORMAL
float4 ps_blend_normal( VS_OUTPUT In ) : COLOR
{
	float4 TexColor ;
	float4 DestColor ;
	float4 BlendTexColor ;
#ifdef PRE_MUL_ALPHA
	float4 PremulTexColor;
	float4 PremulBlendTexColor ;
#endif

#ifdef PRE_MUL_ALPHA
	PremulBlendTexColor = tex2D( BlendTexture, In.TexCoords0 ) ;
	PremulTexColor      = tex2D( SrcTexture,   In.TexCoords1 ) ;

	BlendTexColor       = PremulBlendTexColor / PremulBlendTexColor.a;
	BlendTexColor.a     = PremulBlendTexColor.a;
	TexColor            = PremulTexColor      / PremulTexColor.a;
	TexColor.a          = PremulTexColor.a;
#else
	BlendTexColor       = tex2D( BlendTexture, In.TexCoords0 ) ;
	TexColor            = tex2D( SrcTexture,   In.TexCoords1 ) ;
#endif

#ifdef IGNORE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz;
#endif
#ifdef INVERSE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz - TexColor.rgb;
#endif

	DestColor = lerp( TexColor, BlendTexColor, FactorColor.a ) * In.Diffuse ;

#ifdef IGNORE_TEXALPHA
	DestColor.a = In.Diffuse.a ;
#endif

#ifdef MUL_X_4
	DestColor.rgb *= 4.0f ;
#endif

#ifdef MUL_BLEND
	DestColor = lerp( ZeroHalfOneTwo.zzzz, DestColor, DestColor.a ) ;
#endif

#ifdef PRE_MUL_ALPHA
	DestColor.rgb *= DestColor.a ;
#endif

#ifdef ALPHA_CH_DRAW
	return ps_alpha_ch_blend( DestColor, In.TexCoords2 ) ;
#else
	return DestColor ;
#endif
}

//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_WIPE
float4 ps_blend_wipe( VS_OUTPUT In ) : COLOR
{
	float4 TexColor ;
	float4 DestColor ;
	float4 BlendTexColor ;
#ifdef PRE_MUL_ALPHA
	float4 PremulTexColor;
	float4 PremulBlendTexColor ;
#endif

#ifdef PRE_MUL_ALPHA
	PremulBlendTexColor = tex2D( BlendTexture, In.TexCoords0 ) ;
	PremulTexColor      = tex2D( SrcTexture,   In.TexCoords1 ) ;

	BlendTexColor       = PremulBlendTexColor / PremulBlendTexColor.a ;
	BlendTexColor.a     = PremulBlendTexColor.a ;
	TexColor            = PremulTexColor      / PremulTexColor.a ;
	TexColor.a          = PremulTexColor.a ;
#else
	BlendTexColor       = tex2D( BlendTexture, In.TexCoords0 ) ;
	TexColor            = tex2D( SrcTexture,   In.TexCoords1 ) ;
#endif

#ifdef IGNORE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz;
#endif
#ifdef INVERSE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz - TexColor.rgb;
#endif

	DestColor = TexColor * In.Diffuse ;

#ifdef IGNORE_TEXALPHA
	DestColor.a = In.Diffuse.a ;
#endif

	DestColor.a *= saturate( ( BlendTexColor.a + FactorColor.a ) * FactorColor.b ) ;

#ifdef MUL_X_4
	DestColor.rgb *= 4.0f ;
#endif

#ifdef MUL_BLEND
	DestColor = lerp( ZeroHalfOneTwo.zzzz, DestColor, DestColor.a ) ;
#endif

#ifdef PRE_MUL_ALPHA
	DestColor.rgb *= DestColor.a ;
#endif

#ifdef ALPHA_CH_DRAW
	return ps_alpha_ch_blend( DestColor, In.TexCoords2 ) ;
#else
	return DestColor ;
#endif
}

//�u�����h�e�N�X�`������ DX_BLENDGRAPHTYPE_ALPHA
float4 ps_blend_alpha( VS_OUTPUT In ) : COLOR
{
	float4 TexColor ;
	float4 DestColor ;
	float4 BlendTexColor ;
#ifdef PRE_MUL_ALPHA
	float4 PremulTexColor;
	float4 PremulBlendTexColor ;
#endif

#ifdef PRE_MUL_ALPHA
	PremulBlendTexColor = tex2D( BlendTexture, In.TexCoords0 ) ;
	PremulTexColor      = tex2D( SrcTexture,   In.TexCoords1 ) ;

	BlendTexColor       = PremulBlendTexColor / PremulBlendTexColor.a;
	BlendTexColor.a     = PremulBlendTexColor.a;
	TexColor            = PremulTexColor      / PremulTexColor.a;
	TexColor.a          = PremulTexColor.a;
#else
	BlendTexColor       = tex2D( BlendTexture, In.TexCoords0 ) ;
	TexColor            = tex2D( SrcTexture,   In.TexCoords1 ) ;
#endif
	
#ifdef IGNORE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz;
#endif
#ifdef INVERSE_COLOR
	TexColor.rgb = ZeroHalfOneTwo.zzz - TexColor.rgb;
#endif
		
	DestColor = TexColor * In.Diffuse ;

#ifdef IGNORE_TEXALPHA
	DestColor.a = In.Diffuse.a ;
#endif

	DestColor.a *= BlendTexColor.a ;

#ifdef MUL_X_4
	DestColor.rgb *= 4.0f ;
#endif

#ifdef MUL_BLEND
	DestColor = lerp( ZeroHalfOneTwo.zzzz, DestColor, DestColor.a ) ;
#endif

#ifdef PRE_MUL_ALPHA
	DestColor.rgb *= DestColor.a ;
#endif

#ifdef ALPHA_CH_DRAW
	return ps_alpha_ch_blend( DestColor, In.TexCoords2 ) ;
#else
	return DestColor ;
#endif
}


//�}�X�N�����p�V�F�[�_�[
float4 ps_mask_blend( VS_OUTPUT In ) : COLOR
{
	float4 TexColor ;
	float4 DestColor ;
	float4 BlendTexColor ;

	BlendTexColor = tex2D( BlendTexture, In.TexCoords1 ) ;
	TexColor      = tex2D( SrcTexture,   In.TexCoords0 ) ;
		
	DestColor.rgb = TexColor.rgb ;
	DestColor.a   = BlendTexColor.a ;

	return DestColor ;
}

float4 ps_mask_blend_use_graph( VS_OUTPUT In ) : COLOR
{
	float4 TexColor ;
	float4 DestColor ;
	float4 BlendTexColor ;

	BlendTexColor = tex2D( BlendTexture, In.TexCoords1 ) ;
	TexColor      = tex2D( SrcTexture,   In.TexCoords0 ) ;
		
	DestColor.rgb = TexColor.rgb ;
#ifdef MASK_CH_R
	DestColor.a   = 1.0f - BlendTexColor.r ;
#endif
#ifdef MASK_CH_G
	DestColor.a   = 1.0f - BlendTexColor.g ;
#endif
#ifdef MASK_CH_B
	DestColor.a   = 1.0f - BlendTexColor.b ;
#endif
#ifdef MASK_CH_A
	DestColor.a   = 1.0f - BlendTexColor.a ;
#endif

	return DestColor ;
}

float4 ps_mask_blend_use_graph_reverse( VS_OUTPUT In ) : COLOR
{
	float4 TexColor ;
	float4 DestColor ;
	float4 BlendTexColor ;

	BlendTexColor = tex2D( BlendTexture, In.TexCoords1 ) ;
	TexColor      = tex2D( SrcTexture,   In.TexCoords0 ) ;
		
	DestColor.rgb = TexColor.rgb ;
#ifdef MASK_CH_R
	DestColor.a   = BlendTexColor.r ;
#endif
#ifdef MASK_CH_G
	DestColor.a   = BlendTexColor.g ;
#endif
#ifdef MASK_CH_B
	DestColor.a   = BlendTexColor.b ;
#endif
#ifdef MASK_CH_A
	DestColor.a   = BlendTexColor.a ;
#endif

	return DestColor ;
}

