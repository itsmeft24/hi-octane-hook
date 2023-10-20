
#define USES_TEXCOORD0
#define USES_TEXCOORD1

#include "360_Globals.h"

float PS_EffectIntensity : register( c16 );

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = IN.Position;

    OUT.TexCoord0 = IN.TexCoord0;
    OUT.TexCoord1 = IN.TexCoord1;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float3 originalColor = tex2D( TexMap1, IN.TexCoord1 );

   float3 noiseColor = tex2D( TexMap0, IN.TexCoord0 );
      
   float3 finalColor = lerp( originalColor, originalColor * noiseColor, PS_EffectIntensity );
   
   return float4( finalColor, 1.0 );
}
