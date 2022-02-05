cbuffer worldBuffer : register(b0)
{
    matrix world;
}

cbuffer viewBuffer : register(b1)
{
    matrix view;
}

cbuffer projectionBuffer : register(b2)
{
    matrix projection;
}

struct VertexInput
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};


//	vertexShader,     vertexShader = Shader::AddVS(L"Tutorial.hlsl");
// cpu에 있는 정점값이 들어옴. 튜토리얼씬의 정점좌표들
PixelInput VS(VertexInput input)// 진입점은 VS
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);	// output.pos =  (월드상의 좌표, view)
    output.pos = mul(output.pos, projection); // output.pos =  _카메라 기준의 좌표

    output.color = input.color;

    return output;
}

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
// 위의 VS값이 그대로 내려옴, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}

/*
 altho
	사각

 perspective
	사각뿔, 원근감

WVP:
	4x4 연산으로 행함

	x, y, z는 3
	3 x 4 행렬 연산은 안 되니까
	w를 넣어서
	x y z w 가 된다.

	그래서 shader에서는 float 4 를 받는다.

*/