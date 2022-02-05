#include "Header.hlsli"

// �ȼ��� ���� ��
struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 originPos : POSITION;
};

// ������: VS
// cpu�� �ִ� �������� ����.
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
// ���� VS���� �״�� ������, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return cubeMap.Sample(samp, input.originPos);
}