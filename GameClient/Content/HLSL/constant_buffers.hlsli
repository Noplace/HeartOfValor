cbuffer cbPerObject : register( b0 )
{
    matrix  g_mWorldViewProjection  : packoffset( c0 );
    matrix  g_mWorld                : packoffset( c4 );
    float4  g_MaterialAmbientColor  : packoffset( c8 );
    float4  g_MaterialDiffuseColor  : packoffset( c9 );
}

cbuffer cbPerFrame : register( b1 )
{
    float3              g_vLightDir             : packoffset( c0 );
    float               g_fTime                 : packoffset( c0.w );
    float4              g_LightDiffuse          : packoffset( c1 );
};
