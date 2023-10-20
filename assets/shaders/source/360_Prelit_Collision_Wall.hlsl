
#define USES_SHADERCOLORSCALE
#define USES_WORLDPOSITION

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float3 eyeVector = PS_WorldCameraPosition - IN.WorldPosition;
   
   float alpha = 1.0 - saturate( ( length( eyeVector ) - 200.0 ) / 400.0 );
   
   return float4( PS_ShaderColorScale.rgb, PS_ShaderColorScale.a * alpha );
}
