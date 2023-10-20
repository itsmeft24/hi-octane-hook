
#define POSITION_ALREADY_IN_WORLD_SPACE

#define USES_PARTICLE
#define USES_TEXCOORD0
#define USES_WORLDSHADOWMAP
#define USES_FOG

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   VS_OUTPUT OUT;
   
   FOG_OUTPUT fog = CalculateFog( IN.Position );
   
   float maxScale = fog.Distance * 0.20;

   float4 worldPosition = IN.Position + VS_ParticleDeltaVectors[ IN.Data.x ] * min( IN.Data.y, maxScale );
   
   OUT.Position      = mul( worldPosition, VS_WorldViewProjMatrix );
   OUT.Color         = float4( 1, 1, 1, saturate( IN.Data.z ) );
   OUT.TexCoord0     = IN.TexCoord0;
   OUT.TexCoord2     = ( worldPosition.xz - VS_WorldShadowMapRegion.xy ) * VS_WorldShadowMapRegion.zw;
   OUT.TexCoord2.y   = 1 - OUT.TexCoord2.y;
   OUT.Fog           = fog.Fog;
   
   return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexCoord0 );
   
   LIGHT_OUTPUT L;

   L.NonAmbientColor = texDiffuse0.rgb * 1.25;
   L.AmbientColor    = texDiffuse0.rgb * 0.75;

   return CalculateFinalColor( IN, L, texDiffuse0.a * IN.Color.a );
}
