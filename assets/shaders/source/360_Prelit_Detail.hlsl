
#define USES_COLOR
#define USES_TEXCOORD0
#define USES_TEXGEN0
#define USES_FOG

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   float3 texDetail0  = tex2D( TexMap1, IN.TexGen0   );
   
   float3 color = CalculateDetailColor( texDiffuse0, texDetail0 ) * IN.Color;
   
   return CalculateFinalColor( IN, color, IN.Color.a );
}
