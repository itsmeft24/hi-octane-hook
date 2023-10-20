
#define USES_TEXCOORD0
#define USES_TEXCOORD1

#include "360_Globals.h"

float3 PS_DarkColor        : register( c16 );
float3 PS_MiddleColor      : register( c17 );
float3 PS_LightColor       : register( c18 );
float  PS_EffectIntensity  : register( c19 );
float  PS_NoiseFalloff     : register( c20 );

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

   const float3 luminanceVector = float3( 0.299, 0.587, 0.114 );

   float luminanceValue = dot( originalColor, luminanceVector ) * 2.0;
   
   float3 nightVisionColor;
   
   float3 darkColor    = luminanceValue * PS_DarkColor;
   float3 middleColor  = luminanceValue * PS_MiddleColor;
   float3 lightColor   = luminanceValue * PS_LightColor;
   
   if ( luminanceValue < 0.5 )
   {
      nightVisionColor = lerp( darkColor, middleColor, luminanceValue * 2.0 );
   }
   else
   {
      nightVisionColor = lerp( middleColor, lightColor, ( luminanceValue - 0.5 ) * 2.0 );
   }
   
   float3 noiseColor = tex2D( TexMap0, IN.TexCoord0 );
      
   nightVisionColor = lerp( nightVisionColor * noiseColor, nightVisionColor, saturate( luminanceValue * PS_NoiseFalloff ) );
   
   nightVisionColor *= 0.25;
   
   float3 finalColor = lerp( originalColor, nightVisionColor, PS_EffectIntensity );
   
   return float4( finalColor, 1.0 );
}
