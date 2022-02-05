#pragma once

class Scene
{
public:
	virtual ~Scene() = default; // 업캐스팅, 캐릭터, 자식: 워리어 부모것만 호출하고 자식거 호출안함. 그래서 버츄얼 넣으줌

	virtual void Update() = 0;
	
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;	// ui
	virtual void GUIRender() = 0;	// img

	virtual void Start() {}
	virtual void End() {}
};
