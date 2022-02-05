cbuffer WorldBuffer : register(b0)
{
    matrix world;
}

// cBuffer: �������, world�� 0�� ����
cbuffer ViewBuffer : register(b1)
{
    matrix view;
}

// cBuffer: �������, world�� 0�� ����
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv : UV;
};

// �ȼ��� ���� ��
struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

// cpu�� �ִ� �������� ����. Ʃ�丮����� ������ǥ��
PixelInput VS(VertexInput input)// �������� VS
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view); // output.pos =  (������� ��ǥ, view)
    output.pos = mul(output.pos, projection); // output.pos =  _ī�޶� ������ ��ǥ

    output.uv = input.uv;

    return output;
}

Texture2D map : register(t0);
SamplerState samp : register(s0);

// ���� VS���� �״�� ������
float4 PS(PixelInput input) : SV_TARGET
{
    return map.Sample(samp, input.uv);
}
