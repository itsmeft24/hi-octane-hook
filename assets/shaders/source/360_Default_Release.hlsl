
#define USES_TEXCOORD0

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   return float4( texDiffuse0.rgb * 0.25, texDiffuse0.a );
}
