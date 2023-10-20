
#define POSITION_ALREADY_IN_WORLD_SPACE

#define USES_ECOSYSTEM
#define USES_TEXCOORD0
#define USES_FOG

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
	float4 worldPosition = ( IN.Position * VS_ShrubberyScale + VS_ShrubberyOffset ) + ( IN.Delta * VS_VegetationVector );
	
	FOG_OUTPUT fog = CalculateFog( worldPosition );
	
	VS_OUTPUT OUT;
	
	OUT.Position	= mul( worldPosition, VS_WorldViewProjMatrix );
	OUT.Color		= float4( VS_VegetationColors[ IN.Data.y ].rgb, 1 - ( fog.Distance - VS_ShrubberyRange.x ) * VS_ShrubberyRange.y );
	OUT.TexCoord0	= IN.TexCoord0;
	OUT.Fog			= fog.Fog;
	
	return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   float4 color = texDiffuse0 * IN.Color;
   
   return CalculateFinalColor( IN, color );
}
