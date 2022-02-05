#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;   // ��ü
    float3 viewDir : VIEWDIR; // �þ߰�
    
};


PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    // ������� ��ǥ, ī�޶� �������ġ - ������ ������� ��ġ, ī�޶󿡼� ������ �ٶ󺸴� ������ ���´�.
    output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
    
    output.pos = mul(output.pos, view); 
    output.pos = mul(output.pos, projection); 

    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world); // ���� ����ȭ, ���尪 �����༭ �ǽð� �ݿ�
        
    return output;
}

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 normal = normalize(input.normal);
    float3 light = normalize(lightDirection); // �������3��    // lightBuffer�� �� ����ȭ
       
    // saturate: ���� �� ���̷� �� �����Ѵ�. //diffuseIntensisty: ����
    float diffuseIntensisty = saturate(dot(normal, -light)); // �ϳ��� ���̳ʽ��� �ٲ㼭 ���� ����ٲ�.
    
    float4 specular = 0;
    if (diffuseIntensisty > 0) // 0 > : ��ü�� ���� �޴´�. 0: ���� �� �޴´� 
    {
        // ���� �ݻ簢 ���ϴ� ����, �������� ǥ���� �ν��Ѵ�.
        float3 reflection = normalize(reflect(light, -normal)); // ����ȭ���� ���Ѿ� �ڻ��ΰ��� ���´�.
        
        // �þ߰� ����: ī�޶��� ������ �־ �Ǵµ� �Ϲ������δ� ī�޶� ��ġ���� ������ġ������ ���ͷ� ���� �ִ´�.
        // ī�޶� ���� �������� �ݻ��, ī�޶� ������Ʈ ���� ���͸� �����Ѵ�.
        specular = saturate(dot(-reflection, input.viewDir));
        
        float4 specularIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specularIntensity; // A�� B��(����) ��Ų��.
    }
    
    float4 diffuse = albedo * diffuseIntensisty;
    
    return diffuse + specular;  // ���� �����༭ ��� �ݻ�� ���.
}

/*  Light
        ������
            - ���ݻ�(diffuse)
                dot(-����, light)
                ������ ���ݻ�
            
            - ���ݻ�(Specular)
                ���� �ݻ��


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


���ݻ� ��ǻ��: ���� ���´�.
*/