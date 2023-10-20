
#define USES_COLOR
#define USES_TEXCOORD0
#define USES_WORLDPOSITION
#define USES_WORLDNORMAL
#define USES_FOG

#define REFLECTION_TEXTURE TexMap2

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   float3 texGloss0   = tex2D( TexMap1, IN.TexCoord0 );

   LIGHT_INPUT L;
   
   L.WorldPosition      = IN.WorldPosition;
   L.WorldNormal        = IN.WorldNormal;
   L.VertexColor        = IN.Color;
   L.TexDiffuse0        = texDiffuse0;
   L.GlossPower         = texGloss0.r;
   L.GlossLevel         = texGloss0.g;
   L.ReflectionLevel    = texGloss0.b;
   L.WantAmbient        = 0;
   L.WantDiffuse        = 0;
   L.WantSpecular       = 1;
   L.WantReflection     = 1;
   L.WantFresnel        = 0;
   
   return CalculateFinalColor( IN, CalculateLighting( L ), texDiffuse0.a * IN.Color.a );
}
