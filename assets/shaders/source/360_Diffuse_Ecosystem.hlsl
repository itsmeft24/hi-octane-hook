
#define USES_ECOLIGHTS
#define USES_TEXCOORD0
#define USES_WORLDNORMAL
#define USES_OBJECTCOLORSCALE
#define USES_FOG

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );

   LIGHT_INPUT L;
   
   L.WorldPosition      = 0;
   L.WorldNormal        = IN.WorldNormal;
   L.VertexColor        = 0;
   L.TexDiffuse0        = texDiffuse0;
   L.GlossPower         = 0;
   L.GlossLevel         = 0;
   L.ReflectionLevel    = 0;
   L.WantAmbient        = 1;
   L.WantDiffuse        = 1;
   L.WantSpecular       = 0;
   L.WantReflection     = 0;
   L.WantFresnel        = 0;
   
   return CalculateFinalColor( IN, CalculateLighting( L ), texDiffuse0.a );
}
