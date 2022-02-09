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
	  
		Min_Mag: 확대 축소
		Linear: 주변의 색과 자신색을 합쳐서 번져보이는 효과.

		확대 축소 됐을 때 픽셀 찌그러지는 거 어떻게 처리할지 정하는 것.
		픽셀이 고화질인 경우에는 역효과가 날 수 있고, 실사느낌은 리니어 쓰는 게 좋은데

		2D 이미지 같이 선이 딱딱 끊기는 것은 리니어 효과 주면 번져보일 수 있다.
		2D는 D3D11_FILTER_MIN_MAG_MIP_POINT를 쓰는 게 좋다.

		카툰, UI 같은 거 쓸 때는 POINT로 바꿔쓰고, UI랑 3D랑 합쳐서 할 때는 샘플러 2개 만들어서
		따로따로 적용해주는 것도 방법이다.

		AddressU: UV
			WRAP
				UV값은 넘칠 수도 있다. 1 이상이 될 경우에는 바둑판처럼 여러개가 나온다

			Mirror
				배경보일 때는 Mirror로 배경 늘린 적 있다.

			Border
				넘쳤을 때 특정 색으로 채우는 것.

			Clamp
				텍스처 마지막 색으로 다 채워버리는 것.
	
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;	// 텍스처끼리 비교연산, 우리는 안 쓴다.
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