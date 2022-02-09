#include "Framework.h"

SamplerState::SamplerState()
	: desc{}, state(nullptr)
{
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	Changed();
}

SamplerState::~SamplerState()
{
	state->Release();
}

void SamplerState::SetState(UINT slot)
{
	DC->PSSetSamplers(slot, 1, &state);
}

void SamplerState::Filter(D3D11_FILTER value)
{
	desc.Filter = value;

	Changed();
}

void SamplerState::Address(D3D11_TEXTURE_ADDRESS_MODE value)
{
	desc.AddressU = value;
	desc.AddressV = value;
	desc.AddressW = value;

	Changed();
}

void SamplerState::Changed()
{
	if (state != nullptr)
		state->Release();

	DEVICE->CreateSamplerState(&desc, &state);
}


/* #include "framework.h"

SamplerState::SamplerState()
	: desc{}, state(nullptr)
{
	  
		Min_Mag: Ȯ�� ���
		Linear: �ֺ��� ���� �ڽŻ��� ���ļ� �������̴� ȿ��.

		Ȯ�� ��� ���� �� �ȼ� ��׷����� �� ��� ó������ ���ϴ� ��.
		�ȼ��� ��ȭ���� ��쿡�� ��ȿ���� �� �� �ְ�, �ǻ������ ���Ͼ� ���� �� ������

		2D �̹��� ���� ���� ���� ����� ���� ���Ͼ� ȿ�� �ָ� �������� �� �ִ�.
		2D�� D3D11_FILTER_MIN_MAG_MIP_POINT�� ���� �� ����.

		ī��, UI ���� �� �� ���� POINT�� �ٲ㾲��, UI�� 3D�� ���ļ� �� ���� ���÷� 2�� ����
		���ε��� �������ִ� �͵� ����̴�.

		AddressU: UV
			WRAP
				UV���� ��ĥ ���� �ִ�. 1 �̻��� �� ��쿡�� �ٵ���ó�� �������� ���´�

			Mirror
				��溸�� ���� Mirror�� ��� �ø� �� �ִ�.

			Border
				������ �� Ư�� ������ ä��� ��.

			Clamp
				�ؽ�ó ������ ������ �� ä�������� ��.
	
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;	// �ؽ�ó���� �񱳿���, �츮�� �� ����.
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	Changed();
}

SamplerState::~SamplerState()
{
	state->Release();
}

void SamplerState::SetState(UINT slot)
{
	DC->PSSetSamplers(slot, 1, &state);
}

void SamplerState::Filter(D3D11_FILTER value)
{
	desc.Filter = value;

	Changed();
}

void SamplerState::Address(D3D11_TEXTURE_ADDRESS_MODE value)
{
	desc.AddressU	=	value;
	desc.AddressV	=	value;
	desc.AddressW	=	value;

	Changed();
}

void SamplerState::Changed()
{
	if(state != nullptr)
		state->Release();

	DEVICE->CreateSamplerState(&desc, &state);
}
*/