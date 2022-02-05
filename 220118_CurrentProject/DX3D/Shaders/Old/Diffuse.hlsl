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
    output.pos = mul(output.pos, view); // output.pos =  (월드상의 좌표, view)
    output.pos = mul(output.pos, projection); // output.pos =  _카메라 기준의 좌표

    output.uv = input.uv;
    
    // 법선 정규화, 
    float3 normal = normalize(mul(input.normal, (float3x3)world));
    
    // lightBuffer의 값 정규화
    float3 light = normalize(lightDirection); // 상수버퍼3번
    
    // 하나를 마이너스로 바꿔서 법선 방향바꿈.
    output.diffuse = dot(normal, -light);

    return output;
}

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
// 위의 VS값이 그대로 내려옴, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return diffuseMap.Sample(samp, input.uv) * input.diffuse;
}

/*  Light
        직접광
            - 난반사(diffuse)
                dot(-법선, light)
            
            - 정반사(Specular)
                


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
*/