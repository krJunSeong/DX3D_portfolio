#include "../Header.hlsli"

struct VertexOutput
{
    float3 pos : POSITION;
    float2 size : SIZE;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VertexOutput VS(VertexSize input)
{
    VertexOutput output;
    output.pos = input.pos.xyz;
    output.size = input.size;
    
    return output; // -> Geometry Shader
}

static const float2 TEXCOORD[4] =
{
    // uv값 배열로 미리 잡아놓은 거임
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> output) // 정점 하나만 받는다
{
    float3 up = invView._21_22_23;  // 111213 right, 212223 UpVector, 313233 ForwardVector
    float3 forward = input[0].pos - invView._41_42_43; // 차메라에서 input을 향한 벡터
    forward = normalize(forward);
    float3 right = normalize(cross(up, forward));; // right 벡터: 카메라를 바라보면서 깔끔하게 정점으 찍기 위해서
    
    float2 halfSize = input[0].size * 0.5f;
    
    float4 vertices[4];
    vertices[0] = float4(input[0].pos - halfSize.x * right - halfSize.y * up, 1.0f);
    vertices[1] = float4(input[0].pos - halfSize.x * right + halfSize.y * up, 1.0f);
    vertices[2] = float4(input[0].pos + halfSize.x * right - halfSize.y * up, 1.0f);
    vertices[3] = float4(input[0].pos + halfSize.x * right + halfSize.y * up, 1.0f);
    
    PixelInput pixelInput;

    // for문 제한시키는 명령어
    // Append: vector input 하듯 넣어주는 명령어
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        pixelInput.pos = mul(vertices[i], view);
        pixelInput.pos = mul(pixelInput.pos, projection);
        
        pixelInput.uv = TEXCOORD[i];
        
        output.Append(pixelInput);
    }
}

cbuffer SpriteBuffer : register(b10)
{
    float2 maxFrame;
    float2 curFrame;
}

float4 PS(PixelInput input) : SV_Target
{
    // 0 ~ 1 가는 걸 쪼개서 출력
    float2 uv = (input.uv / maxFrame) + (curFrame / maxFrame);
    
    return diffuseMap.Sample(samp, uv) * mDiffuse;
}