//NormalMapping

#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL; // ��ü
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR; // �þ߰�
};

PixelInput VS(VertexUVNormalTangent input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    // ������� ��ǥ, ī�޶� �������ġ - ������ ������� ��ġ, ī�޶󿡼� ������ �ٶ󺸴� ������ ���´�.
    output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);    // ���� ����ȭ, ���尪 �����༭ �ǽð� �ݿ�
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent); // ����
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1); // �˺��� = ���;
    
    if (hasDiffuseMap)  //��ǻ��������� ����
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 T = normalize(input.tangent);    // x
    float3 B = normalize(input.binormal);   // y
    float3 N = normalize(input.normal);     // z
    
    float3 light = normalize(lightDirection);
    
    float3 normal = N; // �븻�� ������ ��ǲ�� �븻.
    if (hasNormalMap) // ������ ��������ŷ� ����
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        float3 normal = normalMapping * 2.0f - 1.0f; // 0 ~ 1 ������ -1 ~ 1 ������ �ٲ�
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN));   // TBN: ������� ��ǥ��
    }
    
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
        
        float4 specularIntensity = float4(1, 1, 1, 1);
        if(hasSpecularMap)
            specularIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specularIntensity * mSpecular; // pow: A�� B��(����) ��Ų��. 
    }
    
    float4 diffuse = albedo * diffuseIntensisty * mDiffuse; // mSpecular: ������ ��� �� ������ ����
    float4 ambient = albedo * ambientLight * mAmbient;         // �ڱ� ���� 10%
    
    return diffuse + specular + ambient; // ���� �����༭ ��� �ݻ�� ���.
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


��ָ��� �ٽɳ���
    ��ָ��� �� �̷��� �������
��ָ��� ����, ������ ���� ���������� �������. �̷� �����Ͱ� �����µ� Ǫ������ ��� ������ ������ ����Ű�� ������ ����
z���� �������� b�� ���� �Ǳ� ������ Ǫ������ ���ϴ�.
���Ͱ� �¿�� ����Ű�� ���� ���� �ұत��, y�� �ʷϻ��� �������� ��. �̰� ���ָ��̴�.

xy���� ������ �������̴ϱ�, �������� �� uv�������� ���, ź��Ʈ, �װ��� ���� ���̳��. 

"��ü���� �츮�� ��"


*/