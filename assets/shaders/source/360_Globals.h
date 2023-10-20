
#if defined( USES_SKIN ) || defined( USES_PUPPET )

   #define POSITION_ALREADY_IN_WORLD_SPACE

#endif

#ifndef REFLECTION_TEXTURE

   #define REFLECTION_TEXTURE TexMap0

#endif

#ifdef USES_LIGHTMAP

   #define USES_DYNAMICSHADOWMAP

#endif

float4x4 VS_WorldMatrix                : register( c0 );
float4x4 VS_WorldViewMatrix            : register( c4 );
float4x4 VS_ViewProjMatrix             : register( c8 );
float4x4 VS_WorldViewProjMatrix        : register( c12 );
float4x4 VS_ShadowWorldViewProjMatrix  : register( c16 );
float4x2 VS_TexGenMatrix               : register( c20 );

float3   VS_WorldCameraPosition        : register( c24 );
float3   VS_LocalCameraPosition        : register( c25 );

float2   VS_FogParameters              : register( c26 );

float4x2 VS_IconMatrix                 : register( c27 );
float4   VS_IconDepth                  : register( c31 );

float4   VS_ViewportScale              : register( c34 );
float4   VS_ViewportOffset             : register( c35 );

float3   VS_VegetationColors[ 7 ]      : register( c36 );
float4   VS_VegetationVector           : register( c43 );

float4   VS_ShrubberyScale             : register( c44 );
float4   VS_ShrubberyOffset            : register( c45 );
float2   VS_ShrubberyRange             : register( c46 );

float4   VS_ParticleDeltaVectors[ 4 ]  : register( c47 );
float3   VS_ParticleColor[ 4 ]         : register( c51 );

float4   VS_WorldShadowMapRegion       : register( c55 );

float3   VS_CrowdVectorX               : register( c56 );
float3   VS_CrowdVectorY               : register( c57 );
float    VS_CrowdOffsetU               : register( c58 );

float4x3 VS_BoneMatrixStart[ 63 ]      : register( c63 );

float4   PS_ShaderColorScale           : register( c0 );
float4   PS_ObjectColorScale           : register( c1 );
float4   PS_FogColor                   : register( c2 );

#ifndef USES_ECOLIGHTS

   float3   PS_AmbientColor            : register( c3 );
   float3   PS_SunlightColor           : register( c4 );
   float3   PS_SunlightDirection       : register( c5 );

#else

   float3   PS_AmbientColor            : register( c6 );
   float3   PS_SunlightColor           : register( c7 );
   float3   PS_SunlightDirection       : register( c8 );

#endif

float3   PS_WorldCameraPosition        : register( c9 );
float3x2 PS_ViewMatrix                 : register( c10 );
float2   PS_EnvMapScale                : register( c12 );
float2   PS_EnvMapOffset               : register( c13 );
float4   PS_InverseDepthProjection     : register( c14 );
float2   PS_DistanceFadeMinMax         : register( c15 );
float    PS_GlossPower                 : register( c16 );
float3   PS_ShadowColor                : register( c17 );

sampler  TexMap[ 16 ]                  : register( s0 );

sampler  TexMap0                       : register( s0 );
sampler  TexMap1                       : register( s1 );
sampler  TexMap2                       : register( s2 );
sampler  TexMap3                       : register( s3 );
sampler  TexMap4                       : register( s4 );
sampler  TexMap5                       : register( s5 );
sampler  TexMap6                       : register( s6 );
sampler  TexMap7                       : register( s7 );
sampler  TexMap8                       : register( s8 );
sampler  TexMap9                       : register( s9 );
sampler  TexMap10                      : register( s10 );
sampler  TexMap11                      : register( s11 );
sampler  TexMap12                      : register( s12 );
sampler  TexMap13                      : register( s13 );
sampler  TexMap14                      : register( s14 );
sampler  TexMap15                      : register( s15 );

struct VS_INPUT
{
   float4 Position         : POSITION;

   #ifdef USES_ECOSYSTEM

      float4 Delta         : NORMAL;

   #else

      float3 Normal        : NORMAL;

   #endif

   #ifdef USES_COLOR

      float4 Color         : COLOR;

   #endif

   #if defined( USES_TEXCOORD0 ) || defined( USES_TEXGEN0 )

      #ifdef TEXCOORD0_IS_FULL_VECTOR

         float4 TexCoord0  : TEXCOORD0;

      #else

         float2 TexCoord0  : TEXCOORD0;

      #endif

   #endif

   #if defined( USES_TEXCOORD1 ) || defined( USES_TEXGEN1 )

      float2 TexCoord1     : TEXCOORD1;

   #endif

   #if defined( USES_TEXCOORD2 ) || defined( USES_TEXGEN2 ) || defined( USES_LIGHTMAP )

      float2 TexCoord2     : TEXCOORD2;

   #endif

   #if defined( USES_TEXCOORD3 )

      float2 TexCoord3     : TEXCOORD3;

   #endif

   #if defined( USES_TEXCOORD4 )

      float2 TexCoord4     : TEXCOORD4;

   #endif

   #if defined( USES_TEXCOORD5 )

      float2 TexCoord5     : TEXCOORD5;

   #endif

   #if defined( USES_TEXCOORD6 )

      float2 TexCoord6     : TEXCOORD6;

   #endif

   #if defined( USES_TEXCOORD7 )

      float2 TexCoord7     : TEXCOORD7;

   #endif

   #ifdef USES_ECOSYSTEM

      int4   Data          : TEXCOORD1;

   #endif

   #ifdef USES_PARTICLE

      float3 Data          : TEXCOORD1;

   #endif

   #ifdef USES_SKIN

      float4 BlendIndices  : BLENDINDICES;
      float4 BlendWeight   : BLENDWEIGHT;

   #endif

   #ifdef USES_PUPPET

      float  BoneIndex     : TEXCOORD4;

   #endif

   #ifdef USES_BUMP

      float3 Tangent       : TANGENT;

   #endif
};

struct VS_OUTPUT
{
   float4 Position         : POSITION;

   #if defined( USES_COLOR ) || defined( USES_ECOSYSTEM ) || defined( USES_PARTICLE )

      float4 Color         : COLOR;

   #endif

   #ifdef USES_TEXCOORD0

      float2 TexCoord0     : TEXCOORD0;

   #endif

   #ifdef USES_TEXCOORD1

      float2 TexCoord1     : TEXCOORD1;

   #endif

   #if defined( USES_TEXCOORD2 ) || defined( USES_LIGHTMAP ) || defined( USES_WORLDSHADOWMAP )

      float2 TexCoord2     : TEXCOORD2;

   #endif

   #ifdef USES_TEXCOORD3

      float2 TexCoord3     : TEXCOORD3;

   #endif

   #ifdef USES_TEXCOORD4

      float2 TexCoord4     : TEXCOORD4;

   #endif

   #ifdef USES_TEXCOORD5

      float2 TexCoord5     : TEXCOORD5;

   #endif

   #ifdef USES_TEXCOORD6

      float2 TexCoord6     : TEXCOORD6;

   #endif

   #ifdef USES_TEXCOORD7

      float2 TexCoord7     : TEXCOORD7;

   #endif

   #ifdef USES_TEXGEN0

      float2 TexGen0       : TEXCOORD3;

   #endif

   #ifdef USES_TEXGEN1

      float2 TexGen1       : TEXCOORD4;

   #endif

   #ifdef USES_TEXGEN2
   
      float2 TexGen2       : TEXCOORD5;

   #endif

   #if defined( USES_DYNAMICSHADOWMAP )

      float3 TexShadow     : TEXCOORD6;

   #endif

   #ifdef USES_WORLDPOSITION

      float3 WorldPosition : TEXCOORD7;

   #endif

   #ifdef USES_WORLDNORMAL

      float3 WorldNormal   : TEXCOORD8;

   #endif

   #ifdef USES_BUMP

      float3 WorldTangent  : TEXCOORD9;
      float3 WorldBinormal : TEXCOORD10;

   #endif

   #ifdef USES_FOG

      float  Fog           : FOG;

   #endif
};

#ifdef USES_SKIN

   float4x3 CalculateSkinnedWorldMatrix( VS_INPUT IN )
   {
      return VS_BoneMatrixStart[ IN.BlendIndices.x ] * IN.BlendWeight.x + VS_BoneMatrixStart[ IN.BlendIndices.y ] * IN.BlendWeight.y + VS_BoneMatrixStart[ IN.BlendIndices.z ] * IN.BlendWeight.z + VS_BoneMatrixStart[ IN.BlendIndices.w ] * IN.BlendWeight.w;
   }

#endif

#ifdef USES_PUPPET

   float4x3 CalculateSkinnedWorldMatrix( VS_INPUT IN )
   {
      return VS_BoneMatrixStart[ IN.BoneIndex ];
   }

#endif

#ifdef USES_BUMP

   float3 CalculateBumpedNormal( VS_OUTPUT IN, float2 texBump )
   {
      float3 unitWorldNormal   = normalize( IN.WorldNormal   );
      float3 unitWorldTangent  = normalize( IN.WorldTangent  );
      float3 unitWorldBinormal = normalize( IN.WorldBinormal );
      
      return unitWorldNormal + texBump.r * unitWorldTangent + texBump.g * unitWorldBinormal;
   }

#endif

#ifdef USES_FOG

   struct FOG_OUTPUT
   {
      float3 EyeVector;
      float  Distance;
      float  Fog;
   };

   FOG_OUTPUT CalculateFog( float3 position )
   {
      FOG_OUTPUT FOG;

      #ifdef POSITION_ALREADY_IN_WORLD_SPACE

         FOG.EyeVector = position - VS_WorldCameraPosition;

      #else

         FOG.EyeVector = position - VS_LocalCameraPosition;

      #endif

      FOG.Distance = length( FOG.EyeVector );
      
      FOG.Fog = saturate( FOG.Distance * VS_FogParameters.x + VS_FogParameters.y );
      
      return FOG;
   }

#endif

VS_OUTPUT GenerateVertexShaderOutput( VS_INPUT IN )
{
   VS_OUTPUT OUT;

   #if defined( USES_SKIN ) || defined( USES_PUPPET )

      #define WORLD_MATRIX worldMatrix

      float4x3 worldMatrix = CalculateSkinnedWorldMatrix( IN );
      
      float4 worldPosition = float4( mul( IN.Position, worldMatrix ), 1 );

      OUT.Position = mul( worldPosition, VS_ViewProjMatrix );

   #else

      #define WORLD_MATRIX VS_WorldMatrix

      OUT.Position = mul( IN.Position, VS_WorldViewProjMatrix );

      #if defined( USES_WORLDPOSITION ) || defined( USES_WORLDSHADOWMAP )

         #ifdef POSITION_ALREADY_IN_WORLD_SPACE

            float3 worldPosition = IN.Position;

         #else

            float3 worldPosition = mul( IN.Position, WORLD_MATRIX );

         #endif

      #else

         float3 worldPosition = 0;

      #endif

   #endif

   #ifdef USES_DYNAMICSHADOWMAP

      OUT.TexShadow = mul( IN.Position, VS_ShadowWorldViewProjMatrix );

   #endif

   #ifdef USES_COLOR

      OUT.Color = IN.Color;

   #endif

   #ifdef USES_TEXCOORD0
   
      OUT.TexCoord0 = IN.TexCoord0;

   #endif

   #ifdef USES_TEXCOORD1
   
      OUT.TexCoord1 = IN.TexCoord1;

   #endif

   #ifdef USES_LIGHTMAP
   
      OUT.TexCoord2 = IN.TexCoord2;

   #endif

   #ifdef USES_TEXGEN0
   
      OUT.TexGen0 = mul( float3( IN.TexCoord0.xy, 1 ), VS_TexGenMatrix );

   #endif

   #ifdef USES_TEXGEN1
   
      OUT.TexGen1 = mul( float3( IN.TexCoord1.xy, 1 ), VS_TexGenMatrix );

   #endif

   #ifdef USES_TEXGEN2
   
      OUT.TexGen2 = mul( float3( IN.TexCoord2.xy, 1 ), VS_TexGenMatrix );

   #endif

   #ifdef USES_WORLDPOSITION
   
      OUT.WorldPosition = worldPosition;

   #endif

   #ifdef USES_WORLDSHADOWMAP
   
      OUT.TexCoord2 = ( worldPosition.xz - VS_WorldShadowMapRegion.xy ) * VS_WorldShadowMapRegion.zw;
      OUT.TexCoord2.y = 1 - OUT.TexCoord2.y;

   #endif

   #ifdef USES_WORLDNORMAL
   
      OUT.WorldNormal = mul( IN.Normal, WORLD_MATRIX );

   #endif

   #ifdef USES_BUMP
   
      OUT.WorldTangent  = mul( IN.Tangent, WORLD_MATRIX );
      OUT.WorldBinormal = cross( OUT.WorldNormal, OUT.WorldTangent );

   #endif

   #ifdef USES_FOG

      #ifdef POSITION_ALREADY_IN_WORLD_SPACE

         OUT.Fog = CalculateFog( worldPosition ).Fog;

      #else

         OUT.Fog = CalculateFog( IN.Position ).Fog;

      #endif

   #endif

   return OUT;
}

struct LIGHT_INPUT
{
   float3   WorldPosition;
   float3   WorldNormal;
   float4   VertexColor;
   float3   TexDiffuse0;
   float3   TexDiffuse1;
   float    GlossPower;
   float    GlossLevel;
   float    ReflectionLevel;
   bool     WantAmbient;
   bool     WantDiffuse;
   bool     WantSpecular;
   bool     WantReflection;
   bool     WantFresnel;
};

struct LIGHT_OUTPUT
{
   float3 NonAmbientColor;
   float3 AmbientColor;
};

LIGHT_OUTPUT CalculateLighting( LIGHT_INPUT IN )
{
   LIGHT_OUTPUT OUT;

   IN.WorldNormal = normalize( IN.WorldNormal );

   float3 eyeVector = normalize( PS_WorldCameraPosition - IN.WorldPosition );

   float viewAngle;

   if ( IN.WantFresnel )
   {
      viewAngle = saturate( 1 - dot( eyeVector, IN.WorldNormal ) );
   }

   float diffuseContribution = max( dot( IN.WorldNormal, PS_SunlightDirection ), 0 );

   #ifdef USES_TWOTONE

   float3 texDiffuse = lerp( IN.TexDiffuse0, IN.TexDiffuse1, viewAngle * 0.50 );

   #else

   float3 texDiffuse = IN.TexDiffuse0;

   #endif

   if ( IN.WantDiffuse )
   {
      OUT.NonAmbientColor  = texDiffuse * PS_SunlightColor * diffuseContribution;
      OUT.AmbientColor     = texDiffuse * PS_AmbientColor;
   }
   else
   {
      OUT.NonAmbientColor  = 0;
      OUT.AmbientColor     = texDiffuse * IN.VertexColor.rgb;
   }

   if ( IN.WantSpecular )
   {
      float3 specularContribution = 0;

      if ( diffuseContribution > 0 )
      {
         float3 refVector = normalize( 2 * diffuseContribution * IN.WorldNormal - PS_SunlightDirection );

         specularContribution = pow( max( dot( refVector, eyeVector ), 0 ), IN.GlossPower * PS_GlossPower + 1 ) * IN.GlossLevel * ( PS_SunlightColor + PS_AmbientColor );
      }

      OUT.NonAmbientColor += specularContribution;
   }

   if ( IN.WantReflection )
   {
      #ifdef USES_ENVMAP

         float2 reflectionCoordinates = mul( normalize( eyeVector - 4.0 * dot( eyeVector, IN.WorldNormal ) * IN.WorldNormal ), PS_ViewMatrix ) * PS_EnvMapScale + PS_EnvMapOffset;

         float3 reflectionContribution = tex2D( REFLECTION_TEXTURE, reflectionCoordinates );

      #else

         float3 reflectionCoordinates = reflect( eyeVector, IN.WorldNormal );

         float3 reflectionContribution = texCUBE( REFLECTION_TEXTURE, reflectionCoordinates );

      #endif

      if ( IN.WantFresnel )
      {
         IN.ReflectionLevel = IN.ReflectionLevel * viewAngle;
      }

      OUT.AmbientColor = max( OUT.AmbientColor, lerp( OUT.AmbientColor, reflectionContribution, IN.ReflectionLevel ) );
   }

   return OUT;
}

float3 CalculateDetailColor( float4 texDiffuse, float3 texDetail )
{
   return lerp( texDiffuse.rgb, texDiffuse.rgb * texDetail, texDiffuse.a );
}

LIGHT_OUTPUT CalculateBlendColor( LIGHT_OUTPUT from, float3 to, float4 vertexColor )
{
   LIGHT_OUTPUT L;

   L.NonAmbientColor = lerp( from.NonAmbientColor, 0,  vertexColor.a );
   L.AmbientColor    = lerp( from.AmbientColor,    to, vertexColor.a ) * vertexColor;

   return L;
}

LIGHT_OUTPUT CalculateBlendColor( float3 from, LIGHT_OUTPUT to, float4 vertexColor )
{
   LIGHT_OUTPUT L;

   L.NonAmbientColor = lerp( 0,    to.NonAmbientColor, vertexColor.a );
   L.AmbientColor    = lerp( from, to.AmbientColor,    vertexColor.a ) * vertexColor;

   return L;
}

LIGHT_OUTPUT CalculateBlendColor( LIGHT_OUTPUT from, LIGHT_OUTPUT to, float4 vertexColor )
{
   LIGHT_OUTPUT L;

   L.NonAmbientColor = lerp( from.NonAmbientColor, to.NonAmbientColor,  vertexColor.a );
   L.AmbientColor    = lerp( from.AmbientColor,    to.AmbientColor,     vertexColor.a ) * vertexColor;

   return L;
}

#ifdef USES_DYNAMICSHADOWMAP

float3 CalculateShadowColor( VS_OUTPUT IN, float3 surfaceColor )
{
   float surfaceAlpha = ( length( IN.TexShadow.xy * 2.0 - 1.0 ) - 0.8 ) * 5.0;

   if ( surfaceAlpha >= 1.0 )
   {
      return surfaceAlpha;
   }
   else
   {
      float offset = 1.0 / 1024.0;

      float depth1 = tex2D( TexMap5,  IN.TexShadow + float2(     0.0,  offset ) ).r;
      float depth2 = tex2D( TexMap5,  IN.TexShadow + float2( -offset,     0.0 ) ).r;
      float depth3 = tex2D( TexMap5, IN.TexShadow + float2(     0.0,     0.0 ) ).r;
      float depth4 = tex2D( TexMap5, IN.TexShadow + float2(  offset,     0.0 ) ).r;
      float depth5 = tex2D( TexMap5, IN.TexShadow + float2(     0.0, -offset ) ).r;

      float alpha1 = step( IN.TexShadow.z, depth1 );
      float alpha2 = step( IN.TexShadow.z, depth2 );
      float alpha3 = step( IN.TexShadow.z, depth3 );
      float alpha4 = step( IN.TexShadow.z, depth4 );
      float alpha5 = step( IN.TexShadow.z, depth5 );

      float3 shadowColor = lerp( surfaceColor, PS_ShadowColor, ( alpha1 + alpha2 + alpha3 + alpha4 + alpha5 ) * 0.20 );

      return lerp( shadowColor, surfaceColor, saturate( surfaceAlpha ) );
   }
}

#endif

float4 CalculateFinalColor( VS_OUTPUT IN, LIGHT_OUTPUT L, float alpha )
{
   #ifdef USES_LIGHTMAP

      float3 texLightmap = tex2D( TexMap6, IN.TexCoord2 );

      #ifdef USES_DYNAMICSHADOWMAP

      float3 shadowColor = CalculateShadowColor( IN, texLightmap );

      texLightmap = min( texLightmap, shadowColor );

      #endif

      L.AmbientColor    *= texLightmap;
      L.NonAmbientColor *= saturate( texLightmap.g * 8.0f - 4.0f );

   #endif

   #ifdef USES_WORLDSHADOWMAP

      float3 texShadowmap = tex2D( TexMap7, IN.TexCoord2 );

      #ifdef USES_ECOSYSTEM

      L.AmbientColor    = lerp( L.AmbientColor, L.NonAmbientColor, texShadowmap );
      L.NonAmbientColor = 0;

      #else

      L.NonAmbientColor *= texShadowmap;

      #endif

   #endif

   #ifdef USES_HDR

      float colorMultiplier = 1.00;

   #else

      float colorMultiplier = 1.00;

   #endif

   float4 color = float4( ( L.NonAmbientColor + L.AmbientColor ) * colorMultiplier, alpha );

   #ifdef USES_SHADERCOLORSCALE

      color = color * PS_ShaderColorScale;

   #endif

   #ifdef USES_OBJECTCOLORSCALE

      color = color * PS_ObjectColorScale;

   #endif

   #ifdef USES_FOG

      color = float4( lerp( PS_FogColor * 4.0f, color, IN.Fog ).xyz, color.a );

   #endif

   return color;
}

float4 CalculateFinalColor( VS_OUTPUT IN, float4 color )
{
   LIGHT_OUTPUT L;

   L.NonAmbientColor = 0;
   L.AmbientColor    = color.rgb;

   return CalculateFinalColor( IN, L, color.a );
}

float4 CalculateFinalColor( VS_OUTPUT IN, float3 color, float alpha )
{
   LIGHT_OUTPUT L;

   L.NonAmbientColor = 0;
   L.AmbientColor    = color;

   return CalculateFinalColor( IN, L, alpha );
}
