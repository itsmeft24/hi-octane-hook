
#define USES_TEXCOORD0
#define USES_WORLDPOSITION
#define USES_WORLDNORMAL
#define USES_FOG

#define REFLECTION_TEXTURE TexMap1

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   LIGHT_INPUT L;
   
   L.WorldPosition      = IN.WorldPosition;
   L.WorldNormal        = IN.WorldNormal;
   L.VertexColor        = 0;
   L.TexDiffuse0        = texDiffuse0;
   L.GlossPower         = 0;
   L.GlossLevel         = 0;
   L.ReflectionLevel    = texDiffuse0.a;
   L.WantAmbient        = 1;
   L.WantDiffuse        = 1;
   L.WantSpecular       = 0;
   L.WantReflection     = 1;
   L.WantFresnel        = 1;
   
   return CalculateFinalColor( IN, CalculateLighting( L ), 1 );
}
