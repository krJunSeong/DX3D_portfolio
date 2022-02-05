#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    return diffuseMap.Sample(samp, input.uv);
}

/*
#include "Header.hlsli"

// �ȼ��� ���� ��
struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

// ������: VS
// cpu�� �ִ� �������� ����.
PixelInput VS(VertexUV input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);       // output.pos =  (������� ��ǥ, view)
	output.pos = mul(output.pos, projection); // output.pos =  _ī�޶� ������ ��ǥ

	output.uv = input.uv;

	return output;
}

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
// ���� VS���� �״�� ������, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return diffuseMap.Sample(samp, input.uv);
}*/