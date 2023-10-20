
#define USES_TEXCOORD0

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = ( float4( mul( IN.Position, VS_IconMatrix ).xy, VS_IconDepth.z, 1 ) + VS_ViewportOffset ) * VS_ViewportScale;
    OUT.TexCoord0 = IN.TexCoord0;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   return texDiffuse0;
}
