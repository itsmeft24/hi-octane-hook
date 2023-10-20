
#define USES_COLOR
#define USES_TEXCOORD0
#define USES_TEXCOORD1
#define USES_TEXGEN0
#define USES_TEXGEN1
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
   
   float4 texDiffuse1 = tex2D( TexMap2, IN.TexCoord1 );
   float3 texDetail1  = tex2D( TexMap3, IN.TexGen1   );
   
   float3 from = CalculateDetailColor( texDiffuse0, texDetail0 );
   float3 to   = CalculateDetailColor( texDiffuse1, texDetail1 );
      
   return CalculateFinalColor( IN, lerp( from, to, IN.Color.a ) * IN.Color, 1 );
}
