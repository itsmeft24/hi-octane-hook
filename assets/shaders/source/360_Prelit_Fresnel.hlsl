
#define USES_COLOR
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
   L.VertexColor        = IN.Color;
   L.TexDiffuse0        = texDiffuse0;
   L.GlossPower         = 0;
   L.GlossLevel         = 0;
   L.ReflectionLevel    = texDiffuse0.a * IN.Color.a;
   L.WantAmbient        = 0;
   L.WantDiffuse        = 0;
   L.WantSpecular       = 0;
   L.WantReflection     = 1;
   L.WantFresnel        = 1;
   
   return CalculateFinalColor( IN, CalculateLighting( L ), 1 );
}
