
struct VS_INPUT
{
   float4 Pos : POSITION;
   float4 T0  : TEXCOORD0;
};

struct VS_OUTPUT
{
   float2 T0  : TEXCOORD0;
};

sampler tex0   : register( s0 );
sampler tex1   : register( s1 );
sampler tex2   : register( s2 );
float4  tor    : register( c0 );
float4  tog    : register( c1 );
float4  tob    : register( c2 );
float4  consts : register( c3 );

VS_INPUT vs_main( VS_INPUT IN )
{
   return IN;
}

float4 ps_main( VS_OUTPUT IN ) : COLOR
{
   float4 c;

   c.x = tex2D( tex0, IN.T0 ).x;
   c.y = tex2D( tex1, IN.T0 ).x;
   c.z = tex2D( tex2, IN.T0 ).x;
   c.w = consts.x;

   float4 p;

   p.x = dot( tor, c );
   p.y = dot( tog, c );
   p.z = dot( tob, c );
   p.w = consts.w;
   
   return p;
}
