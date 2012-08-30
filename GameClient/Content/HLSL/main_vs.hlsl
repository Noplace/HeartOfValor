#include "constant_buffers.hlsli"

//--------------------------------------------------------------------------------------
// shader input/output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Position     : POSITION; // vertex position 
    float4 Diffuse      : COLOR0;      // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV    : TEXCOORD0;// vertex texture coords 
};

struct VS_OUTPUT
{
    float4 Position     : SV_POSITION; // vertex position 
    float4 Diffuse      : COLOR0;      // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV    : TEXCOORD0;   // vertex texture coords 
};

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT main( VS_INPUT input )
{
    VS_OUTPUT Output;
    //float3 vNormalWorldSpace;
    
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul( float4(input.Position,1.0f), g_mWorld );
    Output.Position = mul( Output.Position, g_mWorldViewProjection );

    //Output.Position = mul(input.Position, g_mWorldViewProjection);
    
    // Transform the normal from object space to world space    
    //vNormalWorldSpace = normalize(mul(input.Normal, (float3x3)g_mWorld)); // normal (world space)

    // Calc diffuse color    
    //Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
                        // g_MaterialAmbientColor;   
    //Output.Diffuse.a = 1.0f; 
    Output.Diffuse = input.Diffuse;
    // Just copy the texture coordinate through
    Output.TextureUV = input.TextureUV; 
    
    return Output;    
}


