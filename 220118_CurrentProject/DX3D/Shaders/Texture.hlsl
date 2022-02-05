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

// 픽셀에 넣을 값
struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

// 진입점: VS
// cpu에 있는 정점값이 들어옴.
PixelInput VS(VertexUV input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);       // output.pos =  (월드상의 좌표, view)
	output.pos = mul(output.pos, projection); // output.pos =  _카메라 기준의 좌표

	output.uv = input.uv;

	return output;
}

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
// 위의 VS값이 그대로 내려옴, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return diffuseMap.Sample(samp, input.uv);
}*/