#include "360_Globals.h"

void vs_main( VS_INPUT IN,
			out float4 oPos : POSITION,
        	out float2 Depth : TEXCOORD0 )
{
	VS_OUTPUT OUT = GenerateVertexShaderOutput( IN );
	oPos = OUT.Position;
    Depth.xy = OUT.Position.zw;
}

void ps_main( float2 Depth : TEXCOORD0,
                out float4 Color : COLOR )
{
    Color = Depth.x / Depth.y;
}
