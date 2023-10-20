
#define USES_TEXCOORD0

#include "360_Globals.h"
#include "360_HDR.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;
    
    OUT.Position  = IN.Position;
    OUT.TexCoord0 = IN.TexCoord0;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float3 frameBuffer = tex2D( TexMap0, IN.TexCoord0 * PS_HDR_UVOffset.zw + PS_HDR_UVOffset.xy );

   float luminance = max( max( frameBuffer.r, frameBuffer.g ), frameBuffer.b );
   
   if ( luminance < 0.25 )
   {
      frameBuffer += saturate( frameBuffer - PS_HDR_Threshold.x ) * PS_HDR_Mix.y;
   }
   
   float3 glow = tex2D( TexMap1, IN.TexCoord0 ) * PS_HDR_Mix.x;
   
   float3 final = max( frameBuffer, glow );
   
   return float4( final, 1 );
}
