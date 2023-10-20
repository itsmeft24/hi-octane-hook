
#define POSITION_ALREADY_IN_WORLD_SPACE

#define USES_ECOSYSTEM
#define USES_TEXCOORD0
#define USES_FOG

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
	float4 worldPosition = IN.Position + IN.Delta * VS_VegetationVector;
	
	VS_OUTPUT OUT;
	
	OUT.Position	= mul( worldPosition, VS_WorldViewProjMatrix );
	OUT.Color		= float4( VS_VegetationColors[ IN.Data.y ].rgb, IN.Data.x / 128.0 );
	OUT.TexCoord0	= IN.TexCoord0;
	OUT.Fog			= CalculateFog( worldPosition ).Fog;
	
	return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   float4 color = texDiffuse0 * IN.Color;
   
   return CalculateFinalColor( IN, color );
}
