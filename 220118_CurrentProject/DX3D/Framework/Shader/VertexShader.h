#pragma once

class VertexShader : public Shader
{
private:
	friend class Shader;

	ID3D11VertexShader* shader;
	ID3D11InputLayout* inputLayout;

	ID3D11ShaderReflection* reflection;

	VertexShader(wstring file);
	~VertexShader();

public:
	virtual void Set() override;

private:
	void CreateInputLayout();
};
/*
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