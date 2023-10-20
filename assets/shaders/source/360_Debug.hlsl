
#define POSITION_ALREADY_IN_WORLD_SPACE

#define USES_COLOR

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   return GenerateVertexShaderOutput( IN );
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   return float4( IN.Color.rgb * 0.5, IN.Color.a );
}
