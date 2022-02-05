//TerrainBrush
#include "Header.hlsli"

cbuffer BrushBuffer : register(b10) // 16byte
{
    int type;
    float3 pickingPos; // 터레인 위에 피킹될 위치
    
    float range;
    float3 color;
}

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
    float3 worldPos : POSITIONT;
};

PixelInput VS(VertexUVNormalTangent input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    output.worldPos = output.pos.xyz;
    output.viewDir = normalize(output.worldPos - invView._41_42_43);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

float3 BrushColor(float3 pos)
{
    if (type == 0)
    {
        // 피킹된 위치에서 월드상의 정점을 향한 벡터
        // 피킹된 곳과 정점 사이의 값을 구해서
        float x = pos.x - pickingPos.x;
        float z = pos.z - pickingPos.z;
        
        float distance = sqrt(x * x + z * z);
        
        if (distance <= range) return color;
    }
    else if (type == 1)
    {
        float size = range * 0.5f;
        
        if (abs(pos.x - pickingPos.x) < size 
            && abs(pos.z - pickingPos.z) < size)
            
            return color;
    }
    
    return float3(0, 0, 0);
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
 
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 light = normalize(lightDirection);
    float3 normal = N;
    
    if (hasNormalMap)
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        normal = normalMapping * 2.0f - 1.0f; //0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN));
    }
    
    float diffuseIntensity = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        //Phong Shading
        //float3 reflection = normalize(reflect(light, normal));
        //specular = saturate(dot(-reflection, input.viewDir));
        //Blinn Poing Shading
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(normal, -halfWay));
        
        float4 specularIntensity = float4(1, 1, 1, 1);
        if (hasSpecularMap)
            specularIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specularIntensity * mSpecular;
    }
    
    float emissiveIntensity = 0.0f;
    
    if (mEmissive.a > 0.0f)
    {
        float t = saturate(dot(normal, -input.viewDir));
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;
    float4 ambient = albedo * ambientLight * mAmbient;
    float4 emissive = mEmissive * emissiveIntensity;
    
    float4 brushColor = float4(BrushColor(input.worldPos), 1.0f); 
    
    return diffuse + specular + ambient + emissive + brushColor;
}
/*
    롤 같은 거 보면 스킬 미리보기 나오잖아
    범위 출력 같은 거
*/