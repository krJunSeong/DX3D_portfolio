//ModelAnimator
#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
    
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);
    
    output.pos = mul(input.pos, transform);
    
    output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 lightDirection = normalize(light.direction);
    float3 normal = N;
    
    if (hasNormalMap)
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        normal = normalMapping * 2.0f - 1.0f; //0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN));
    }
    
    float diffuseIntensity = saturate(dot(normal, -lightDirection));
    
    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        float3 halfWay = normalize(input.viewDir + lightDirection);
        specular = saturate(dot(normal, -halfWay));
        
        float4 specularIntensity = float4(1, 1, 1, 1);
        if (hasSpecularMap)
            specularIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specularIntensity * mSpecular;
    }
    
    float emissiveIntensity = 0.0f;
    
    if (mEmissive.a > 0.0f)
    {
        float t = saturate(dot(normal, -input.viewDir));
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;
    float4 ambient = albedo * ambientLight * mAmbient;
    float4 emissive = mEmissive * emissiveIntensity;
    
    return diffuse + specular + ambient + emissive;
}
/*//Model
#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
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
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1);
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 light = normalize(lightDirection);
    float3 normal = N;// �븻�� ������ ��ǲ�� �븻.
    
    if (hasNormalMap)
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        normal = normalMapping * 2.0f - 1.0f; // 0 ~ 1 ������ -1 ~ 1 ������ �ٲ�
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN)); // TBN: ������� ��ǥ��
    }
    
    // saturate: ���� �� ���̷� �� �����Ѵ�. //diffuseIntensisty: ����
    float diffuseIntensity = saturate(dot(normal, -light)); // �ϳ��� ���̳ʽ��� �ٲ㼭 ���� ����ٲ�.
    
    float4 specular = 0;
    if (diffuseIntensity > 0) // 0 > : ��ü�� ���� �޴´�. 0: ���� �� �޴´� 
    {
        //Phong Shading
        //float3 reflection = normalize(reflect(light, -normal)); // ����ȭ���� ���Ѿ� �ڻ��ΰ��� ���´�.
        //specular = saturate(dot(-reflection, input.viewDir));
        
        //Blinn Phong Shading
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(normal, -halfWay));
        
        
        float4 specularIntensity = float4(1, 1, 1, 1);
        if (hasSpecularMap)
            specularIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specularIntensity * mSpecular; // pow: A�� B��(����) ��Ų��. 
    }
    
    float emissiveIntensity = 0.0f;
    
    if (mEmissive.a > 0.0f) // 0���� ũ�ٴ� �� ������ �ִٴ� ��
    {
        float t = saturate(dot(normal, -input.viewDir)); // t: ������
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse; // mSpecular: ������ ��� �� ������ ����
    float4 ambient = albedo * ambientLight * mAmbient;
    float4 emissive = mEmissive * emissiveIntensity; // ���İ����� �ܰ����ʿ� ���ִµ� ��.
    
    return diffuse + specular + ambient + emissive; // ���� �����༭ ��� �ݻ�� ���.
}

/*
  �κ��Ͱ� ����ȭ�� ���¸� �������� �� �� ���� ������ ��Ÿ���� ��ȯ�Ѵ�.
���� �ٶ󺸸� ������ �̷��� �ְ�, ���� �Ѱ� ���� �ٶ󺸴� �����̶� �����̶� ������ �� �Űŵ�.
�̷��� �� �� �þ߰� Ʋ����. �̷��� �����ߴµ� cos(0) = 1, cos(90) = 0
���ֺ��� 1, �������� 0�� ������ ��ǻ��� �Ȥ�����.

���������� �̰� ����? �������ش�.
    0 ���� 1���� ����. �׷� ������ ���� �� �ƴѰ�. �ּ� �ִ밪 ������ �� ������ ���� �־��ش� �̰���.
    �ϴ�. �̰ź��� �ּҰ��� �� ������ 0�� ���������.
    0 ���� 1���̰� �������ݾ�. 1�� �ӵǸ� 0�� �ǰ�
    0�� �Ǹ� 1�� �Ǵ°���

    ���ٽ�Ƽ�ϴ� �ƽ��� 0 ��t�� ��ǻ�, 1-t�� �ݴ밪.
    ��ǻ���� �ݴ밪�� ������ ������ �� Ŀ���ٴ� ����.
    
    ��ü�� ���� �� ��ü�߱��� �شٴ� ��.
    ���� �� �ٵ� ���� ��ü�߱��� �شٴ� ����.
    
    ��������� �K���� �ݴ�������� �ؼ� ���� �� �ִ� ����.

*/