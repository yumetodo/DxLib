sampler SrcTexture   : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

float4 Invert_PS(VS_OUTPUT In) : COLOR
{
	float4 Color;

	Color = tex2D( SrcTexture, In.TexCoords0 ) ;

	Color.rgb = float3(1,1,1) - Color.rgb;
	
	return Color;
}
