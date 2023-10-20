
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
   float3 maxColor = tex2D( TexMap0, IN.TexCoord0 );
   
   float2 uv2 = min( PS_HDR_MaxTexCoord, IN.TexCoord0 + PS_HDR_OnePixelOffset );
   
   maxColor = max( maxColor, tex2D( TexMap1, float2( uv2.x, IN.TexCoord0.y ) ) );
   maxColor = max( maxColor, tex2D( TexMap2, float2( IN.TexCoord0.x, uv2.y ) ) );
   maxColor = max( maxColor, tex2D( TexMap3, uv2 ) );
  
   return float4( maxColor, 1 );
}
