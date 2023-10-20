
#define USES_TEXCOORD0
#define USES_WORLDPOSITION
#define USES_WORLDNORMAL
#define USES_SHADERCOLORSCALE
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
   L.VertexColor        = 0;
   L.TexDiffuse0        = texDiffuse0;
   L.GlossPower         = texGloss0.r;
   L.GlossLevel         = texGloss0.g;
   L.ReflectionLevel    = texGloss0.b;
   L.WantAmbient        = 1;
   L.WantDiffuse        = 1;
   L.WantSpecular       = 1;
   L.WantReflection     = 1;
   L.WantFresnel        = 1;
   
   #ifdef USES_TWOTONE
   
   L.TexDiffuse1 = tex2D( TexMap3, IN.TexCoord0 );
   
   #endif

   return CalculateFinalColor( IN, CalculateLighting( L ), texDiffuse0.a );
}
