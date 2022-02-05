#include "Framework.h"

Bar::Bar(wstring frontImage, wstring backImage, float maxValue)
	: maxValue(maxValue), curValue(maxValue)
{
	valueBuffer = new ValueBuffer();
	valueBuffer->data.values[0] = maxValue;
	valueBuffer->data.values[1] = curValue;
	valueBuffer->data.values[2] = curValue;

	Texture* texture = Texture::Add(frontImage);

	frontBar = new Quad(Float2(texture->Width(), texture->Height()));
	frontBar->GetMaterial()->SetShader(L"Bar.hlsl");
	frontBar->GetMaterial()->SetDiffuseMap(frontImage);

	texture = Texture::Add(backImage);

	backBar = new Quad(Float2(texture->Width(), texture->Height()));
	backBar->GetMaterial()->SetShader(L"Texture.hlsl");
	backBar->GetMaterial()->SetDiffuseMap(backImage);

	//frontBar->SetPivot(frontBar->Size().x * -0.5f, 0);

	frontBar->SetParent(this);
	backBar->SetParent(this);
}

Bar::~Bar()
{
	delete valueBuffer;

	delete frontBar;
	delete backBar;
}

void Bar::Update()
{	
	valueBuffer->data.values[0] = maxValue;
	valueBuffer->data.values[1] = curValue;
	valueBuffer->data.values[2] = lerpValue;

	frontBar->UpdateWorld();
	backBar->UpdateWorld();

	UpdateWorld();
}

void Bar::Render()
{	
	backBar->Render();
	valueBuffer->SetPSBuffer(10);
	frontBar->Render();
}
