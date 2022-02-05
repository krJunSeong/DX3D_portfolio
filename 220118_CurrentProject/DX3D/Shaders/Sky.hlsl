#include "Header.hlsli"

// 픽셀에 넣을 값
struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 originPos : POSITION;
};

// 진입점: VS
// cpu에 있는 정점값이 들어옴.
PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.0f;
    output.pos = mul(output.pos, projection);
    
    output.originPos = input.pos.xyz;
    
    return output;
}

TextureCube cubeMap : register(t10);

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
// 위의 VS값이 그대로 내려옴, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return cubeMap.Sample(samp, input.originPos);
}