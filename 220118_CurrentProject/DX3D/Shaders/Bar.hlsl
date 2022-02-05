#include "Header.hlsli"

cbuffer Value : register(b10)
{
    float maxValue;
    float curValue;
    float lerpValue;
}

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.position = mul(input.pos, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = diffuseMap.Sample(samp, input.uv);
    
    if (input.uv.x < curValue / maxValue)
        return color * mDiffuse;
    else if (input.uv.x < lerpValue / maxValue)
        return float4(color.rgb, 0.5f);
        
    
    return float4(0, 0, 0, 0);
}