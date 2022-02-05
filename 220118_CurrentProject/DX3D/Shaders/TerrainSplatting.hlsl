//TerrainSplatting
#include "Header.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
    
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.viewPos = invView._41_42_43;
    
    return output;
}

Texture2D alphaMap : register(t10);
Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);

float4 PS(LightPixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float4 alpha = alphaMap.Sample(samp, input.uv);
    float4 second = secondMap.Sample(samp, input.uv);
    float4 third = thirdMap.Sample(samp, input.uv);
    
    albedo = lerp(albedo, second, alpha.r);
    albedo = lerp(albedo, third, alpha.g);
    
    Material material = GetMaterial(input, albedo);
    
    float4 result = CalcLights(material);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
    
    return result + ambient + emissive;
}