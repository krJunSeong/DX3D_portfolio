#include "Framework.h"

map<wstring, Shader*> Shader::shaders;

VertexShader* Shader::AddVS(wstring file)
{
    if (file.length() == 0) return nullptr;

    wstring key = file + L"VS";

    if (shaders.count(key) > 0)
        return (VertexShader*)shaders[key];

    shaders[key] = new VertexShader(file);

    return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
    if (file.length() == 0) return nullptr;

    wstring key = file + L"PS";

    if (shaders.count(key) > 0)
        return (PixelShader*)shaders[key];

    shaders[key] = new PixelShader(file);

    return (PixelShader*)shaders[key];
}

ComputeShader* Shader::AddCS(wstring file)
{
    wstring key = file + L"CS";

    if (shaders.count(key) > 0)
        return (ComputeShader*)shaders[key];

    shaders[key] = new ComputeShader(file);

    return (ComputeShader*)shaders[key];
}

GeometryShader* Shader::AddGS(wstring file)
{
    wstring key = file + L"GS";

    if (shaders.count(key) > 0)
        return (GeometryShader*)shaders[key];

    shaders[key] = new GeometryShader(file);

    return (GeometryShader*)shaders[key];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}



/*
#include "framework.h"

// ����ƽ�� �ܺο��� �ʱ�ȭ �� �ؾ���
map<wstring, Shader*> Shader::shaders;

VertexShader* Shader::AddVS(wstring file)
{
    if (file.length() == 0) return nullptr;

    wstring key = file + L"VS"; // PX�� �����ϱ� ���� VS ���� ���� VS �̸��� �־���

    if (shaders.count(key) > 0)
        return (VertexShader*)shaders[key];// ������ �ִ°� ��������

    // �����´ٴ� ���� ó�� �ε��Ѵٴ� ��
    shaders[key] = new VertexShader(file); // upCasting, �Ϲ� �θ� �ڽ��� �޸𸮰��� �޴� ��

    return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
    if (file.length() == 0) return nullptr;

    wstring key = file + L"PS";

    if (shaders.count(key) > 0)
        return (PixelShader*)shaders[key];// ������ �ִ°� ��������

    // �����´ٴ� ���� ó�� �ε��Ѵٴ� ��
    // upCasting, �Ϲ� �θ� �ڽ��� �޸𸮰��� �޴� ��
    shaders[key] = new PixelShader(file); 

    return (PixelShader*)shaders[key];
}

ComputeShader* Shader::AddCS(wstring file)
{
    wstring key = file + L"CS";

    if (shaders.count(key) > 0)
        return (ComputeShader*)shaders[key];// ������ �ִ°� ��������

    // �����´ٴ� ���� ó�� �ε��Ѵٴ� ��
    shaders[key] = new ComputeShader(file); // upCasting, �Ϲ� �θ� �ڽ��� �޸𸮰��� �޴� ��

    return (ComputeShader*)shaders[key];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}


	Rendering PipeLine ( GPU ����)
		IA (input Assembler, CPU�������� �� �����)
			VertexLayout: position uv ���� ����
			vertices: ��������
			vertexBuffer: vertices�� ������ �޴´�
			index: ���� �մ� ����

		VS (VertexShader, ��������)
			wvp (world, view projectoin): NDC ��ǥ, -1 ~ 1 �������� ����

		RS
			ViewPort ��ȯ (width , height) ndc(-1 ~ 1)�� 0 ~ 1280���� �ٲ�, ������ �־��ָ� �ڵ����� RS���� ���ش�.

		PS
			���� ��������

		OM
			���

	//view ��ȯ: ī�޶� �������� ��ǥ�� �������ִ� ����
*/