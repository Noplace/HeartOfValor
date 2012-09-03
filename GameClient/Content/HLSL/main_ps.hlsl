#include "common.hlsli"
//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D    g_txDiffuse : register( t0 );
SamplerState g_samLinear : register( s0 );


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
// color with diffuse material color
//--------------------------------------------------------------------------------------
float4 main( VS_OUTPUT In ) : SV_TARGET
{ 
    // Lookup mesh texture and modulate it with diffuse
    return float4(1,1,1,1);//In.Diffuse;//g_txDiffuse.Sample( g_samLinear, In.TextureUV ) * In.Diffuse;
}
