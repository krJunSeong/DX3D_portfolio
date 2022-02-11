#include "../Header.hlsli"

struct VertexInput
{
    float4 pos : POSITION;
    float2 size : SIZE;
    float2 random : RANDOM;
};

struct VertexOutput
{
    float3 pos : POSITION;
    float2 size : SIZE;
    float distance : DISTANCE;  // ���İ� ����
    float4 color : COLOR;        // ���� ��
    float3 velocity : VELOCITY; // �������� ����
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
    float alpha : ALPHA;
};

cbuffer SnowBuffer : register(b10)
{
    float3 velocity;
    float drawDistance;
    
    float4 color;
    
    float3 origin;
    float time;
    
    float3 size;
    float tubulence;    // �����
}

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    float3 displace = time * velocity; // �ð��� ���� �̵��ϰ� �����
    
    // ���� ������ ȿ�� �ڽ� �ȿ� ���ּ� ���� ������ ȿ����,                  ������ �߽ɿ� ���ߴ� ��
    input.pos.y = origin + size.y - (input.pos.y - displace.y) % size.y;

    input.pos.x += cos(time - input.random.x) * tubulence;
    input.pos.z += cos(time - input.random.y) * tubulence;
    
    input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);
    
    output.velocity = velocity;
    output.distance = drawDistance;
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
    
        pixelInput.alpha = saturate(1 - pixelInput.pos.z / input[0].distance.x) * 0.5f;
        
        output.Append(pixelInput);
    }
}

float4 PS(PixelInput input) : SV_Target
{
    float4 result = diffuseMap.Sample(samp, input.uv);
    
    result.a *= (1.0f - input.alpha) * 1.5f;
    
    return result * input.color;
}