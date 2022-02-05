#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float diffuse : DIFFUSE;
};


PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view); // output.pos =  (������� ��ǥ, view)
    output.pos = mul(output.pos, projection); // output.pos =  _ī�޶� ������ ��ǥ

    output.uv = input.uv;
    
    // ���� ����ȭ, 
    float3 normal = normalize(mul(input.normal, (float3x3)world));
    
    // lightBuffer�� �� ����ȭ
    float3 light = normalize(lightDirection); // �������3��
    
    // �ϳ��� ���̳ʽ��� �ٲ㼭 ���� ����ٲ�.
    output.diffuse = dot(normal, -light);

    return output;
}

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
// ���� VS���� �״�� ������, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return diffuseMap.Sample(samp, input.uv) * input.diffuse;
}

/*  Light
        ������
            - ���ݻ�(diffuse)
                dot(-����, light)
            
            - ���ݻ�(Specular)
                


        ������(ambient, �ݻ籤)
            �ڽ��� ������������ ��Ư�� �ۼ�Ʈ��ŭ �ݰ�


��ǻ���?

 �ʷϻ��� ���̴� ������ ���� ������ �ʷϻ��� ���� �׷��Ŵ�.
�̰� ������ ���� ����ε� 111, �� ������ ��� ���� �ݻ��Ѵ�.
010�̶��� ���� �����ϱ� �̰� �״�� �ݻ��ؼ� �״�� ���δ�.

  �׷��ٸ�? �̰� �� ���������� ���ϱ�? �ϴ� �̰� ������� �޾Ƽ� �� ������ �ݻ縦 ���ݾ�
�ٵ� �̰� 100 �̶�� �͹ۿ� �ݻ縦 ���ؼ� �׷��ϴ�.

  ��ǻ����� ���� �޾��� ��(������ ���� ����, �ٸ� ������� ������) � �� �� �ݻ��ϴ���
�װ� ��ǻ�� �ʿ� ����ִ�.

  �ʷϻ� �ؽ�ó�� ������ �̰� �ʷϻ��� �ݻ��ؼ� �ʷϻ����� ���̴°���.


���
    ��ǻ���: ���ݻ翡 ���� ������ ��

------------------------------------------------------------------------------------------------------------------

float3x3���� ����ȯ�� ��Ű�� �Ʒ��� ���� ����(4��, 4�� ����)
    11 ~ 13
    21 ~ 23
    31 ~ 33

����ŧ��(���ݻ�)
     ���� ������ ������ �� �¾Ƶ��;� ���� �νô�.
    ���� �̷��� ���߸� �̰Ÿ� �ٶ󺸴� �츮 ���� ���� �ž�.
    ��ü�� ���� �޾��� �� �� �ݻ簢, �� �ݻ簢�� �츮���� �¾� �������� �� �̶� ���ν��� ���ݻ��� �Ѵ�.
���ݻ�� ��������� ��ǻ��ʹ� �� �ݴ��, �����̶� ����Ʈ�� ���� �� ������ ��ο����� ȿ���� �ִµ�
���ݻ�� ������� �ݻ�ȴٴ� �� ���� �������ٴ¤� ���̰�, �̴� ���ó�� ���δٴ� ����.
    Ư�� ��Ȳ���� ���� �����شٴ� ����. �ݻ�Ǵ� ������ �츮 ���� �þ߰�.
    cos�� �̿��ؼ� 
        �þ߰��� ���ݻ簢���� �¾� �������� 1
        ������ ũ�� �� ������ �۰Գ�����
*/