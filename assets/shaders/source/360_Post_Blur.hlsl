
#define USES_TEXCOORD0

#include "360_Globals.h"

float2 PS_BlurOffset[ 5 ] : register( c16 );
float  PS_BlurKernel[ 5 ] : register( c32 );

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = IN.Position;
    OUT.TexCoord0 = IN.TexCoord0;
    
    return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float3 centerColor = tex2D( TexMap2, IN.TexCoord0 );
   
   float3 blurredColor =

      centerColor * PS_BlurKernel[ 2 ].x +
      
      tex2D( TexMap0,  IN.TexCoord0 + PS_BlurOffset[ 0 ] ) * PS_BlurKernel[ 0 ] +
      tex2D( TexMap1,  IN.TexCoord0 + PS_BlurOffset[ 1 ] ) * PS_BlurKernel[ 1 ] +
      tex2D( TexMap3,  IN.TexCoord0 + PS_BlurOffset[ 3 ] ) * PS_BlurKernel[ 3 ] +
      tex2D( TexMap4,  IN.TexCoord0 + PS_BlurOffset[ 4 ] ) * PS_BlurKernel[ 4 ];
      
   return float4( blurredColor, 1 );
}
