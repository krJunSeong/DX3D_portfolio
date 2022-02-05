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
// cpu�� �ִ� �������� ����. Ʃ�丮����� ������ǥ��
PixelInput VS(VertexInput input)// �������� VS
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);	// output.pos =  (������� ��ǥ, view)
    output.pos = mul(output.pos, projection); // output.pos =  _ī�޶� ������ ��ǥ

    output.color = input.color;

    return output;
}

// pixelShader = Shader::AddPS(L"Tutorial.hlsl");
// ���� VS���� �״�� ������, pixelShader
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}

/*
 altho
	�簢

 perspective
	�簢��, ���ٰ�

WVP:
	4x4 �������� ����

	x, y, z�� 3
	3 x 4 ��� ������ �� �Ǵϱ�
	w�� �־
	x y z w �� �ȴ�.

	�׷��� shader������ float 4 �� �޴´�.

*/