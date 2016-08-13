float   ClipBrightness : register( c0 ) ;
float4  RGBToY         : register( c1 ) ;
float4  ClipFillColor  : register( c2 ) ;
sampler SrcTexture     : register( s0 ) ;

struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

struct VS_INPUT 
{
	float4 Position        : POSITION ;
	float4 Normal          : NORMAL ;
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
	float2 TexCoords1      : TEXCOORD1 ;
} ;

VS_OUTPUT VS_main( VS_INPUT In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    
    Out.Position = In.Position;
    Out.TexCoords0 = In.TexCoords0;

    return Out;
}

float4 PS_Less(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;
    clip( dot( RGBToY, Color ) - ClipBrightness ) ;
	
    return Color;
}

float4 PS_Less_ClipFill(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;
    if( dot( RGBToY, Color ) < ClipBrightness )
    {
        return ClipFillColor;
    }
    else
    {
        return Color;
    }
}

float4 PS_Greater(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;
    clip( ClipBrightness - dot( RGBToY, Color ) ) ;

    return Color;
}

float4 PS_Greater_ClipFill(VS_OUTPUT In) : COLOR
{   
    float4 Color;

    Color = tex2D( SrcTexture, In.TexCoords0 ) ;
    if( dot( RGBToY, Color ) > ClipBrightness )
    {
        return ClipFillColor;
    }
    else
    {
        return Color;
    }
}
