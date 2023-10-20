
#define USES_TEXCOORD0

#include "360_Globals.h"
#include "X360Antialias.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
    VS_OUTPUT OUT;

    OUT.Position  = IN.Position;
    OUT.TexCoord0 = IN.TexCoord0;
    
    return OUT;
}

#if ( AA_SOLUTION == AA_NONE )

   float4 ps_main( VS_OUTPUT IN ) : COLOR
   {
      float3 color = tex2D( TexMap0, IN.TexCoord0 );
      
      return float4( color * 4.0, 1.0 );
   }
   
#elif ( AA_SOLUTION == AA_USE_TWO_FRAME_PIXEL_OFFSET )

   float  PS_BlurDampingFactor  : register( c16 );
   float2 PS_SampleOffsets[ 4 ] : register( c17 );

   float4 ps_main( VS_OUTPUT IN ) : COLOR
   {
      float3 tMinus2 = tex2D( TexMap[ 0 ], IN.TexCoord0 ); // Current Frame - 2
      float3 tMinus1 = tex2D( TexMap[ 1 ], IN.TexCoord0 ); // Current Frame - 1
      float3 tMinus0 = tex2D( TexMap[ 2 ], IN.TexCoord0 ); // Current Frame

      float3 blend21 = ( tMinus1 + tMinus2 ) * 0.5;
      float3 blend10 = ( tMinus0 + tMinus1 ) * 0.5;
      
      float3 delta = abs( blend10 - blend21 );
      
      float3 currentColor = tMinus0;
      float3 lastColor    = tMinus1;
      
      int i;
      
      for ( i = 0; i < 4; i++ )
      {
         lastColor    += tex2D( TexMap[ i * 2 + 3 ], IN.TexCoord0 + PS_SampleOffsets[ i ] );
         currentColor += tex2D( TexMap[ i * 2 + 4 ], IN.TexCoord0 + PS_SampleOffsets[ i ] );
      }
      
      lastColor    /= 5.0;
      currentColor /= 5.0;
      
      float3 finalColor = lerp( ( currentColor + lastColor ) * 0.5, currentColor, saturate( delta * PS_BlurDampingFactor ) );
      
      return float4( finalColor * 4.0, 1.0 );
   }

#elif ( AA_SOLUTION == AA_USE_ONE_FRAME_PIXEL_OFFSET_WITHOUT_AF ) || ( AA_SOLUTION == AA_USE_ONE_FRAME_PIXEL_OFFSET_WITH_AF )

   float4 ps_main( VS_OUTPUT IN ) : COLOR
   {
      float3 shift0 = tex2D( TexMap0, IN.TexCoord0 );
      float3 shift1 = tex2D( TexMap1, IN.TexCoord0 );
      
      return float4( ( shift0 + shift1 ) * 2.0, 1.0 );
   }

#elif ( AA_SOLUTION == AA_USE_SIMPLE_BLUR )

   float2 PS_SampleOffsets[ 5 ] : register( c16 );

   float4 ps_main( VS_OUTPUT IN ) : COLOR
   {
      float3 color = 0.0;
      
      int i;
      
      for ( i = 0; i < 5; i++ )
      {
         color += tex2D( TexMap[ i ], IN.TexCoord0 + PS_SampleOffsets[ i ] );
      }
      
      return float4( color * ( 4.0 / 5.0 ), 1.0 );
   }

#elif ( AA_SOLUTION == AA_USE_HARDWARE_2X ) || ( AA_SOLUTION == AA_USE_HARDWARE_4X )

   float PS_ColorScale : register( c16 );

   float4 ps_main( VS_OUTPUT IN ) : COLOR
   {
      float3 color = tex2D( TexMap0, IN.TexCoord0 );
      
      return float4( color * PS_ColorScale, 1.0 );
   }

#endif
