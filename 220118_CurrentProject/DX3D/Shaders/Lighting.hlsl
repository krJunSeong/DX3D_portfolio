//ModelAnimator
#include "Header.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
    LightPixelInput output;
    
    matrix transform = 0;
    
    [branch]
    if (objectType == 1)
        transform = mul(SkinWorld(input.indices, input.weights), world);
    else
        transform = world;
    
    output.pos = mul(input.pos, transform);
    
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
    
    output.viewPos = invView._41_42_43;
    
    return output;
}

float4 PS(LightPixelInput input) : SV_Target
{
    Material material = GetMaterial(input);
    
    float4 result = CalcLights(material);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
    
    return result + ambient + emissive;
}