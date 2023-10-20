
#define USES_TEXCOORD0
#define USES_TEXCOORD1
#define USES_TEXCOORD2
#define USES_TEXCOORD3
#define USES_TEXCOORD4
#define USES_TEXCOORD5
#define USES_TEXCOORD6
#define USES_TEXCOORD7

#include "360_Globals.h"

float PS_BlurAmount : register( c16 );

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = IN.Position;

    OUT.TexCoord0 = IN.TexCoord0;
    OUT.TexCoord1 = IN.TexCoord1;
    OUT.TexCoord2 = IN.TexCoord2;
    OUT.TexCoord3 = IN.TexCoord3;
    OUT.TexCoord4 = IN.TexCoord4;
    OUT.TexCoord5 = IN.TexCoord5;
    OUT.TexCoord6 = IN.TexCoord6;
    OUT.TexCoord7 = IN.TexCoord7;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float alpha = tex2D( TexMap0, IN.TexCoord0 ).a * PS_BlurAmount;
   
   float start  = 1.0;
   float finish = 1.0 / 8.0;
   float range  = finish - start;
   float step   = range / 6.0;
   float total  = ( start - step * 0 ) + ( start - step * 1 ) + ( start - step * 2 ) + ( start - step * 3 ) + ( start - step * 4 ) + ( start - step * 5 ) + ( start - step * 6 );
      
   float3 texDiffuse0 =
      tex2D( TexMap1, IN.TexCoord1 ) * ( ( start - step * 0 ) / total ) +
      tex2D( TexMap2, IN.TexCoord2 ) * ( ( start - step * 1 ) / total ) +
      tex2D( TexMap3, IN.TexCoord3 ) * ( ( start - step * 2 ) / total ) +
      tex2D( TexMap4, IN.TexCoord4 ) * ( ( start - step * 3 ) / total ) +
      tex2D( TexMap5, IN.TexCoord5 ) * ( ( start - step * 4 ) / total ) +
      tex2D( TexMap6, IN.TexCoord6 ) * ( ( start - step * 5 ) / total ) +
      tex2D( TexMap7, IN.TexCoord7 ) * ( ( start - step * 6 ) / total );
   
   return float4( texDiffuse0, alpha );
}
