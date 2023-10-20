
#include "360_Globals.h"

VS_OUTPUT vs_main( VS_INPUT IN )
{
   VS_OUTPUT OUT;
   
   OUT.Position = 0;
   
   return OUT;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   return 0;
}
