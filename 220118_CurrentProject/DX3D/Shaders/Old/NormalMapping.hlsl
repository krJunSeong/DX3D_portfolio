//NormalMapping

#include "Header.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL; // 객체
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR; // 시야각
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
    
    output.normal = mul(input.normal, (float3x3) world);    // 법선 정규화, 월드값 곱해줘서 실시간 반영
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent); // 외적
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = float4(1, 1, 1, 1); // 알베도 = 흰색;
    
    if (hasDiffuseMap)  //디퓨즈맵있으면 넣음
        albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 T = normalize(input.tangent);    // x
    float3 B = normalize(input.binormal);   // y
    float3 N = normalize(input.normal);     // z
    
    float3 light = normalize(lightDirection);
    
    float3 normal = N; // 노말맵 없으면 인풋의 노말.
    if (hasNormalMap) // 있으면 적용받은거로 세팅
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        float3 normal = normalMapping * 2.0f - 1.0f; // 0 ~ 1 범위를 -1 ~ 1 범위로 바꿈
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN));   // TBN: 월드상의 좌표계
    }
    
    // saturate: 일정 수 사이로 값 제한한다. //diffuseIntensisty: 강도
    float diffuseIntensisty = saturate(dot(normal, -light)); // 하나를 마이너스로 바꿔서 법선 방향바꿈.
    
    float4 specular = 0;
    if (diffuseIntensisty > 0) // 0 > : 물체가 빛을 받는다. 0: 빛을 안 받는다 
    {
        // 빛의 반사각 구하는 공식, 법선으로 표면을 인식한다.
        float3 reflection = normalize(reflect(light, -normal)); // 정규화까지 시켜야 코사인값만 남는다.
        
        // 시야각 정보: 카메라의 포워드 넣어도 되는데 일반적으로는 카메라 위치에서 정점위치까지의 벡터로 많이 넣는다.
         // 카메라를 향해 나랑오는 반사빛, 카메라가 오브젝트 보는 벡터를 내적한다.
        specular = saturate(dot(-reflection, input.viewDir));
        
        float4 specularIntensity = float4(1, 1, 1, 1);
        if(hasSpecularMap)
            specularIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, shininess) * specularIntensity * mSpecular; // pow: A를 B승(제곱) 시킨다. 
    }
    
    float4 diffuse = albedo * diffuseIntensisty * mDiffuse; // mSpecular: 재질이 어떻게 빛 받을지 설정
    float4 ambient = albedo * ambientLight * mAmbient;         // 자기 색의 10%
    
    return diffuse + specular + ambient; // 색을 더해줘서 흰색 반사빔 쏜다.
}

/*  Light
        직접광
            - 난반사(diffuse)
                dot(-법선, light)
                적당한 빛반사
            
            - 정반사(Specular)
                눈뽕 반사빔


        간접광(ambient, 반사광)
            자신의 고유재질에서 ㄴ특정 퍼센트만큼 반감


디퓨즈맵?

 초록색이 보이는 이유는 저기 빔에서 초록색을 쏴서 그런거다.
이거 재질은 원래 흰색인데 111, 이 재질은 모든 빛을 반사한다.
010이란느 빛을 받으니까 이걸 그대로 반사해서 그대로 보인다.

  그렇다면? 이건 왜 빨간색으로 보일까? 일단 이건 흰색빛을 받아서 이 재질은 반사를 하잖아
근데 이건 100 이라는 것밖에 반사를 못해서 그러하다.

  디퓨즈맵은 빛을 받았을 때(빛에도 색이 있음, 다만 흰색으로 가정함) 어떤 값 을 반사하는지
그게 디퓨즈 맵에 들어있다.

  초록색 텍스처가 있으면 이건 초록색을 반사해서 초록색으로 보이는거지.


결론
    디퓨즈맵: 난반사에 의한 고유의 색

------------------------------------------------------------------------------------------------------------------

float3x3으로 형변환을 시키면 아래와 같이 남음(4열, 4행 날라감)
    11 ~ 13
    21 ~ 23
    31 ~ 33

스펙큘러(정반사)
     빛이 내눈에 들어오면 딱 맞아들어와야 눈이 부시다.
    빛이 이렇게 비추면 이거를 바라보는 우리 눈이 있을 거야.
    물체의 빛을 받았을 때 이 반사각, 이 반사각이 우리눈과 맞아 떨어졌을 때 이때 눈부심을 정반사라고 한다.
정반사는 결론적으로 디퓨즈와는 좀 반대로, 법선이랑 라이트랑 각도 안 맞으면 어두워지는 효과가 있는데
정반사는 흰색으로 반사된다는 건 색이 더해졌다는ㄴ 뜻이고, 이는 흰색처럼 보인다는 거임.
    특정 상황에서 빛을 더해준다는 뜻임. 반사되는 각도와 우리 눈의 시야각.
    cos을 이용해서 
        시야각과 정반사각도가 맞아 떨어지면 1
        각도가 크게 돼 있으면 작게나오고


난반사 디퓨즈: 빛이 나온다.


노멀맵핑 핵심내용
    노멀맵이 왜 이렇게 생겼는지
노멀맵은 벡터, 법선에 대한 벡터정보가 담겨있음. 이런 데이터가 나오는데 푸른빛을 띄는 이유는 법선이 가리키는 포워드 방향
z축을 기준으로 b랑 같이 되기 때문에 푸른빛이 강하다.
벡터가 좌우로 가리키는 빛일 때는 불긍ㄴ색, y는 초록색이 강해지는 것. 이게 ㄴ멀맵이다.

xy축을 법선과 종법선이니까, 방향잡을 때 uv방향으로 잡고, 탄젠트, 그거의 수직 바이노멀. 

"입체감을 살리는 것"


*/