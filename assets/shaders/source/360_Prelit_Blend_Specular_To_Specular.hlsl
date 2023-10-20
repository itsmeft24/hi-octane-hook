
#define USES_COLOR
#define USES_TEXCOORD0
#define USES_TEXCOORD1
#define USES_WORLDPOSITION
#define USES_WORLDNORMAL
#define USES_FOG

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   float3 texGloss0   = tex2D( TexMap1, IN.TexCoord0 );
   
   float4 texDiffuse1 = tex2D( TexMap2, IN.TexCoord1 );
   float3 texGloss1   = tex2D( TexMap3, IN.TexCoord1 );

   LIGHT_INPUT L0;
   
   L0.WorldPosition      = IN.WorldPosition;
   L0.WorldNormal        = IN.WorldNormal;
   L0.VertexColor        = IN.Color;
   L0.TexDiffuse0        = texDiffuse0;
   L0.GlossPower         = texGloss0.r;
   L0.GlossLevel         = texGloss0.g;
   L0.ReflectionLevel    = 0;
   L0.WantAmbient        = 0;
   L0.WantDiffuse        = 0;
   L0.WantSpecular       = 1;
   L0.WantReflection     = 0;
   L0.WantFresnel        = 0;
   
   LIGHT_INPUT L1;
   
   L1.WorldPosition      = IN.WorldPosition;
   L1.WorldNormal        = IN.WorldNormal;
   L1.VertexColor        = IN.Color;
   L1.TexDiffuse0        = texDiffuse1;
   L1.GlossPower         = texGloss1.r;
   L1.GlossLevel         = texGloss1.g;
   L1.ReflectionLevel    = 0;
   L1.WantAmbient        = 0;
   L1.WantDiffuse        = 0;
   L1.WantSpecular       = 1;
   L1.WantReflection     = 0;
   L1.WantFresnel        = 0;
   
   LIGHT_OUTPUT from = CalculateLighting( L0 );
   LIGHT_OUTPUT to   = CalculateLighting( L1 );
      
   return CalculateFinalColor( IN, CalculateBlendColor( from, to, IN.Color ), 1 );
}
