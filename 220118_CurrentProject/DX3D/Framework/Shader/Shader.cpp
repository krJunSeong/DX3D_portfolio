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

// 스태틱은 외부에서 초기화 꼭 해야함
map<wstring, Shader*> Shader::shaders;

VertexShader* Shader::AddVS(wstring file)
{
    if (file.length() == 0) return nullptr;

    wstring key = file + L"VS"; // PX랑 구분하기 위해 VS 넣을 때는 VS 이름을 넣어줌

    if (shaders.count(key) > 0)
        return (VertexShader*)shaders[key];// 있으면 있는거 내보낸다

    // 내려온다는 것은 처음 로드한다는 뜻
    shaders[key] = new VertexShader(file); // upCasting, 일반 부모가 자식의 메모리값을 받는 것

    return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
    if (file.length() == 0) return nullptr;

    wstring key = file + L"PS";

    if (shaders.count(key) > 0)
        return (PixelShader*)shaders[key];// 있으면 있는거 내보낸다

    // 내려온다는 것은 처음 로드한다는 뜻
    // upCasting, 일반 부모가 자식의 메모리값을 받는 것
    shaders[key] = new PixelShader(file); 

    return (PixelShader*)shaders[key];
}

ComputeShader* Shader::AddCS(wstring file)
{
    wstring key = file + L"CS";

    if (shaders.count(key) > 0)
        return (ComputeShader*)shaders[key];// 있으면 있는거 내보낸다

    // 내려온다는 것은 처음 로드한다는 뜻
    shaders[key] = new ComputeShader(file); // upCasting, 일반 부모가 자식의 메모리값을 받는 것

    return (ComputeShader*)shaders[key];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}


	Rendering PipeLine ( GPU 영역)
		IA (input Assembler, CPU정보들을 다 갖고옴)
			VertexLayout: position uv 같은 유형
			vertices: 정점정보
			vertexBuffer: vertices의 정보를 받는다
			index: 정점 잇는 순서

		VS (VertexShader, 정점연산)
			wvp (world, view projectoin): NDC 좌표, -1 ~ 1 공간에서 비율

		RS
			ViewPort 변환 (width , height) ndc(-1 ~ 1)을 0 ~ 1280으로 바꿈, 설정만 넣어주면 자동으로 RS에서 해준다.

		PS
			색을 결정지음

		OM
			출력

	//view 변환: 카메라 기준으로 좌표계 정렬해주는 연산
*/