
#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = ( float4( mul( IN.Position, VS_IconMatrix ).xy, VS_IconDepth.z, 1 ) + VS_ViewportOffset ) * VS_ViewportScale;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   return PS_ObjectColorScale;
}
