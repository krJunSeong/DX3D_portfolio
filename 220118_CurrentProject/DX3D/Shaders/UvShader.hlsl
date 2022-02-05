cbuffer WorldBuffer : register(b0)
{
    matrix world;
}

// cBuffer: 상수버퍼, world는 0번 세팅
cbuffer ViewBuffer : register(b1)
{
    matrix view;
}

// cBuffer: 상수버퍼, world는 0번 세팅
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv : UV;
};

// 픽셀에 넣을 값
struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

// cpu에 있는 정점값이 들어옴. 튜토리얼씬의 정점좌표들
PixelInput VS(VertexInput input)// 진입점은 VS
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view); // output.pos =  (월드상의 좌표, view)
    output.pos = mul(output.pos, projection); // output.pos =  _카메라 기준의 좌표

    output.uv = input.uv;

    return output;
}

Texture2D map : register(t0);
SamplerState samp : register(s0);

// 위의 VS값이 그대로 내려옴
float4 PS(PixelInput input) : SV_TARGET
{
    return map.Sample(samp, input.uv);
}
