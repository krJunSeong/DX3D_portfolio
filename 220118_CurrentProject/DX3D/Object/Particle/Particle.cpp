#include "framework.h"

Particle::Particle()
{
	isActive = false;		// 필요할 때 터트리기 위해 일단 flase

	material = new Material();

	// ---------------------------------------------------------------
	// 이거 안 쓰면 알파값 버그로잘림
	// 뎁스 무시하고 전체적으로 알파 다 적용시키면 렌더순서로 해서 깊이연산이 안 됨.
	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

}

Particle::~Particle()
{
	delete material;
	delete vertexBuffer;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
}

void Particle::Render()
{
	if(!isActive) return;

	blendState[1]->SetState();
	depthState[1]->SetState();

	vertexBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // geometry 사용 시에는 포인트리스트로 넘겨야 함

	material->Set();
	geometryShader->Set();

	DC->Draw(particleCount, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void Particle::Play(Vector3 position)
{
	isActive = true;
	this->position = position;
}

void Particle::Stop()
{
	isActive = false;
}
