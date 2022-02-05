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