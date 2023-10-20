
#define POSITION_ALREADY_IN_WORLD_SPACE
#define TEXCOORD0_IS_FULL_VECTOR

#define USES_WORLDSHADOWMAP
#define USES_COLOR
#define USES_TEXGEN0
#define USES_FOG

#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   VS_OUTPUT OUT;
   
   IN.Position.xyz = IN.Position.xyz + ( VS_CrowdVectorX * IN.TexCoord0.z ) + ( VS_CrowdVectorY * IN.TexCoord0.w );
   
   IN.TexCoord0.x = IN.TexCoord0.x + IN.TexCoord0.z * VS_CrowdOffsetU;

   OUT.Position      = mul( IN.Position, VS_WorldViewProjMatrix );
   OUT.Color         = IN.Color;
   OUT.TexGen0       = mul( float3( IN.TexCoord0.xy, 1 ), VS_TexGenMatrix );
   OUT.TexCoord2     = ( IN.Position.xz - VS_WorldShadowMapRegion.xy ) * VS_WorldShadowMapRegion.zw;
   OUT.TexCoord2.y   = 1 - OUT.TexCoord2.y;
   OUT.Fog           = CalculateFog( IN.Position ).Fog;

   return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 texDiffuse0 = tex2D( TexMap0, IN.TexGen0 );
   
   float3 color = 0;
   float  alpha = 0;
   
   if ( texDiffuse0.a < 0.25 )
   {
      if ( texDiffuse0.a == 0.00 )
      {
         color = texDiffuse0;
         alpha = 1;
      }
   }
   else
   {
      color = texDiffuse0 * IN.Color;
      alpha = 1;
   }
   
   LIGHT_OUTPUT L;

   L.NonAmbientColor = color;
   L.AmbientColor    = 0;

   return CalculateFinalColor( IN, L, alpha );
}
