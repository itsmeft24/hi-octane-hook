
#define USES_TEXCOORD0

#include "360_Globals.h"
#include "360_HDR.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;
    
    OUT.Position  = IN.Position;
    OUT.TexCoord0 = IN.TexCoord0;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float3 tex0 = tex2D( TexMap0, IN.TexCoord0 );
   float3 tex1 = tex2D( TexMap1, IN.TexCoord0 );
   
   float3 color;
   int    i;

   for ( i = 0; i < 3; i++ )
   {
      if ( tex0[ i ] > tex1[ i ] )
      {
         float delta = max( 1.0 / 1024.0, tex0[ i ] * PS_HDR_Speed.x );
         
         color[ i ] = min( tex1[ i ] + delta, tex0[ i ] );
      }
      else
      {
         float delta = max( 1.0 / 1024.0, tex1[ i ] * PS_HDR_Speed.y );
         
         color[ i ] = max( tex1[ i ] - delta, tex0[ i ] );
      }
   }
   
   return float4( color, 1 );
}
