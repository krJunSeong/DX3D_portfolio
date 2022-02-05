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
    
    // 월드상의 좌표, 카메라 월드상위치 - 정점의 월드상의 위치, 카메라에서 정점을 바라보는 방향이 나온다.
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
    float3 normal = N;// 노말맵 없으면 인풋의 노말.
    
    if (hasNormalMap)
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        normal = normalMapping * 2.0f - 1.0f; // 0 ~ 1 범위를 -1 ~ 1 범위로 바꿈
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN)); // TBN: 월드상의 좌표계
    }
    
    // saturate: 일정 수 사이로 값 제한한다. //diffuseIntensisty: 강도
    float diffuseIntensity = saturate(dot(normal, -light)); // 하나를 마이너스로 바꿔서 법선 방향바꿈.
    
    float4 specular = 0;
    if (diffuseIntensity > 0) // 0 > : 물체가 빛을 받는다. 0: 빛을 안 받는다 
    {
        //Phong Shading
        //float3 reflection = normalize(reflect(light, -normal)); // 정규화까지 시켜야 코사인값만 남는다.
        //specular = saturate(dot(-reflection, input.viewDir));
        
        //Blinn Phong Shading
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(normal, -halfWay));
        
        
        float4 specularIntensity = float4(1, 1, 1, 1);
        if (hasSpecularMap)
            specularIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specularIntensity * mSpecular; // pow: A를 B승(제곱) 시킨다. 
    }
    
    float emissiveIntensity = 0.0f;
    
    if (mEmissive.a > 0.0f) // 0보다 크다는 건 강도가 있다는 뜻
    {
        float t = saturate(dot(normal, -input.viewDir)); // t: 보간값
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse; // mSpecular: 재질이 어떻게 빛 받을지 설정
    float4 ambient = albedo * ambientLight * mAmbient;
    float4 emissive = mEmissive * emissiveIntensity; // 알파값으로 외곽선쪽에 빛주는데 씀.
    
    return diffuse + specular + ambient + emissive; // 색을 더해줘서 흰색 반사빔 쏜다.
}

/*
  두벡터가 정규화된 상태면 내적했을 때 두 벡터 사이의 세타값을 반환한다.
내가 바라보면 법선이 이렇게 있고, 지금 한게 내가 바라보는 방향이랑 법선이랑 내적을 한 거거든.
이렇게 한 번 시야각 틀었어. 이렇게 내적했는데 cos(0) = 1, cos(90) = 0
마주보면 1, 끝에가면 0이 나오는 디퓨즈랑 똑ㄱ같지.

스무스스텝 이건 뭘까? 보간해준대.
    0 부터 1사이 값을. 그럼 러프랑 같은 거 아닌가. 최소 최대값 넣으면 둘 사이의 값을 넣어준다 이거지.
    일단. 이거봐봐 최소값은 익 높으면 0이 가까워지고.
    0 부터 1사이값 가질거잖아. 1ㅇ ㅣ되면 0이 되고
    0이 되면 1이 되는거지

    인텐시티니는 맥스는 0 ㅂt는 디퓨즈값, 1-t는 반대값.
    디퓨즈의 반대값은 끝으로 갈수록 더 커진다는 뜻임.
    
    물체를 봤을 때 자체발광을 준다는 것.
    빛을 안 바든 곳에 자체발광을 준다는 뜻임.
    
    결론적으로 딮즈의 반대방향으로 해서 값을 더 주는 것임.

*/