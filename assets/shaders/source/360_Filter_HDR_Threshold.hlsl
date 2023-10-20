
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
   float3 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   float luminance = max( max( texDiffuse0.r, texDiffuse0.g ), texDiffuse0.b );
   
   if ( luminance >= 0.25 )
   {
      return float4( texDiffuse0, 1 );
   }
   else
   {
      return float4( 0, 0, 0, 1 );
   }
}
