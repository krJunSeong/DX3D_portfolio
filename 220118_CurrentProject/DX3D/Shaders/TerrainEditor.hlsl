//TerrainEditor
#include "Header.hlsli"

cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 pickingPos;
    
    float range;
    float3 color;
}

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION0;
    float3 worldPos : POSITION1;
    float4 alpha : ALPHA;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    output.worldPos = output.pos.xyz;
    output.viewPos = invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.alpha = input.alpha;
    
    return output;
}

float3 BrushColor(float3 pos)
{
    if (type == 0)
    {
        float x = pos.x - pickingPos.x;
        float z = pos.z - pickingPos.z;
        
        float distance = sqrt(x * x + z * z);
        
        if (distance <= range)
            return color;
    }
    else if (type == 1)
    {
        float size = range * 0.5f;
        
        if (abs(pos.x - pickingPos.x) < size && abs(pos.z - pickingPos.z) < size)
            return color;
    }
    
    return float3(0, 0, 0);
}

Texture2D secondDiffuseMap : register(t11);
Texture2D thirdDiffuseMap : register(t12);

Texture2D secondSpecularMap : register(t13);
Texture2D thirdSpecularMap : register(t14);

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float4 second = secondDiffuseMap.Sample(samp, input.uv);
    float4 third = thirdDiffuseMap.Sample(samp, input.uv);
    
    albedo = lerp(albedo, second, input.alpha.r);
    albedo = lerp(albedo, third, input.alpha.g);
    
    Material material;
    material.normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
    material.diffuseColor = albedo;
    material.viewPos = input.viewPos;
    material.specularIntensity = SpecularMapping(input.uv);
    material.worldPos = input.worldPos;
    
    float4 result = CalcLights(material);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
    
    float4 brushColor = float4(BrushColor(input.worldPos), 1.0f);
    
    return result + ambient + emissive + brushColor;
}