
#define USES_TEXCOORD0

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = IN.Position;
    OUT.TexCoord0 = IN.TexCoord0;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float3 background = tex2D( TexMap0, IN.TexCoord0 );
   float  depth      = tex2D( TexMap1, IN.TexCoord0 ).r;
   
   depth =  PS_InverseDepthProjection.y / ( depth + PS_InverseDepthProjection.z );
   
   float alpha = ( depth - PS_DistanceFadeMinMax.x ) * PS_DistanceFadeMinMax.y;
   
   return float4( background, alpha );
}
