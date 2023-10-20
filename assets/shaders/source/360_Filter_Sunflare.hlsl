
#define USES_TEXCOORD0

#include "360_Globals.h"

float PS_FlareAlpha : register( c16 );

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = float4( IN.Position.xy, 0, 1 );
    OUT.TexCoord0 = IN.TexCoord0;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   float3 color = texDiffuse0.rgb * PS_FlareAlpha;
   float  alpha = lerp( 1.0, texDiffuse0.a, PS_FlareAlpha.x );
   
   return float4( color, alpha );
}
