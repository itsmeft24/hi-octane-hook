
#define USES_BUMP
#define USES_COLOR
#define USES_TEXCOORD0
#define USES_TEXGEN1
#define USES_TEXGEN2
#define USES_WORLDPOSITION
#define USES_WORLDNORMAL
#define USES_FOG

#define REFLECTION_TEXTURE TexMap4

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float3 texGloss0   = tex2D( TexMap1, IN.TexCoord0 );
   float2 texBump0    = tex2D( TexMap2, IN.TexGen1   ) * float2( 2, 2 ) - float2( 1, 1 );
   float2 texBump1    = tex2D( TexMap3, IN.TexGen2   ) * float2( 2, 2 ) - float2( 1, 1 );
   
   LIGHT_INPUT L;
   
   L.WorldPosition      = IN.WorldPosition;
   L.WorldNormal        = CalculateBumpedNormal( IN, texBump0 + texBump1 );
   L.VertexColor        = 0;
   L.TexDiffuse0        = tex2D( TexMap0, IN.TexCoord0 + L.WorldNormal.xy * 0.1 );  // uses the vector calculated above
   L.GlossPower         = texGloss0.r;
   L.GlossLevel         = texGloss0.g;
   L.ReflectionLevel    = texGloss0.b;
   L.WantAmbient        = 1;
   L.WantDiffuse        = 1;
   L.WantSpecular       = 1;
   L.WantReflection     = 1;
   L.WantFresnel        = 1;
   
   return CalculateFinalColor( IN, CalculateLighting( L ), IN.Color.a );
}
