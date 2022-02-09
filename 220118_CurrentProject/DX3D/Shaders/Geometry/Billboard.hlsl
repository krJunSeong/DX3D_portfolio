#include "../Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VertexSize VS(VertexSize input)
{
    return input; // -> Geometry Shader
}

static const float2 TEXCOORD[4] =
{
    // uv�� �迭�� �̸� ��Ƴ��� ����
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f)  
};

[maxvertexcount(4)]
void GS(point VertexSize input[1], inout TriangleStream<PixelInput> output) // ���� �ϳ��� �޴´�
{
    float3 up = float3(0, 1, 0);
    float3 forward = input[0].pos - invView._41_42_43; // ���޶󿡼� input�� ���� ����
    forward.y = 0;
    forward = normalize(forward);
    float3 right = normalize(cross(up, forward)); // right ����: ī�޶� �ٶ󺸸鼭 ����ϰ� ������ ��� ���ؼ�
    
    float2 halfSize = input[0].size * 0.5f;
    
    float4 vertices[4];
    vertices[0] = float4(input[0].pos.xyz - halfSize.x * right - halfSize.y * up, 1.0f);
    vertices[1] = float4(input[0].pos.xyz - halfSize.x * right + halfSize.y * up, 1.0f);
    vertices[2] = float4(input[0].pos.xyz + halfSize.x * right - halfSize.y * up, 1.0f);
    vertices[3] = float4(input[0].pos.xyz + halfSize.x * right + halfSize.y * up, 1.0f);
    
    PixelInput pixelInput;
    
    [unroll(4)] // for�� ���ѽ�Ű�� ��ɾ�
    for (int i = 0; i < 4; i++)
    {
        pixelInput.pos = mul(vertices[i], view);
        pixelInput.pos = mul(pixelInput.pos, projection);
        
        pixelInput.uv = TEXCOORD[i];
        
        output.Append(pixelInput); // vector input �ϵ� �־��ִ� ��ɾ�
    }
}

float4 PS(PixelInput input) : SV_Target
{
    return diffuseMap.Sample(samp, input.uv);
}