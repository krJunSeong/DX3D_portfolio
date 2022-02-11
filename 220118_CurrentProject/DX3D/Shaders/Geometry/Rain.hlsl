#include "../Header.hlsli"

struct VertexOutput
{
    float3 pos : POSITION;
    float2 size : SIZE;
    float2 distance : DISTANCE;     // ���İ� ����
    float4 color : COLOR;           // ���� ��
    float3 velocity : VELOCITY;     // �������� ����
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
    float alpha : ALPHA;
};

cbuffer RainBuffer : register(b10)
{
    float3 velocity;
    float drawDistance;
    
    float4 color;
    
    float3 origin;
    float time;
    
    float3 size;
}

VertexOutput VS(VertexSize input)
{
    VertexOutput output;
    
    float3 v = velocity;
    v.xz /= input.size.y * 0.1f;    // size y���� ���� �� ���ϰ�.
    output.velocity = v;
    
    float3 displace = time * v;     // �ð��� ���� �̵��ϰ� �����
    
    // ���� ������ ȿ�� �ڽ� �ȿ� ���ּ� ���� ������ ȿ����,                  ������ �߽ɿ� ���ߴ� ��
    input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);

    float alpha = cos(time + (input.pos.x + input.pos.z));
    alpha = saturate(1.5f + alpha / drawDistance * 2.0f);
    
    output.distance.x = drawDistance;
    output.distance.y = alpha;
    
    output.color = color;
    
    output.pos = input.pos.xyz;
    output.size = input.size;
    
    return output; // -> Geometry Shader
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
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> output) // ���� �ϳ��� �޴´�
{
    float3 up = normalize(-input[0].velocity);
    float3 forward = input[0].pos - invView._41_42_43; // ī�޶󿡼� input�� ���� ����
    forward = normalize(forward);
    float3 right = normalize(cross(up, forward)); // right ����: ī�޶� �ٶ󺸸鼭 ����ϰ� ������ ��� ���ؼ�
    
    float2 halfSize = input[0].size * 0.5f;
    
    float4 vertices[4];
    vertices[0] = float4(input[0].pos - halfSize.x * right - halfSize.y * up, 1.0f);
    vertices[1] = float4(input[0].pos - halfSize.x * right + halfSize.y * up, 1.0f);
    vertices[2] = float4(input[0].pos + halfSize.x * right - halfSize.y * up, 1.0f);
    vertices[3] = float4(input[0].pos + halfSize.x * right + halfSize.y * up, 1.0f);
    
    PixelInput pixelInput;
    pixelInput.color = input[0].color;

    // for�� ���ѽ�Ű�� ��ɾ�
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        pixelInput.pos = mul(vertices[i], view);
        pixelInput.pos = mul(pixelInput.pos, projection);
        
        pixelInput.uv = TEXCOORD[i];
    
        pixelInput.alpha = 0.2f * saturate(1 - pixelInput.pos.z / input[0].distance.x)
        * input[0].distance.y;
        
        output.Append(pixelInput);
    }
}

float4 PS(PixelInput input) : SV_Target
{
    float4 result = diffuseMap.Sample(samp, input.uv);
    
    result.a *= input.alpha;
    
    return result * input.color;
}