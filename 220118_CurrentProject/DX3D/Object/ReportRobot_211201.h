#pragma once
// Idle, Walk
class ReportRobot_211201 : public Transform
{
private:
	enum class State{
		IDLE, WALK
	}state;

	Cube* cube;
	Cube* cube2;

	Cube* head;
	
	Cube* leftArm;
	Cube* rightArm;
	Cube* leftLeg;
	Cube* rightLeg;

	float bodySize;
	float duration;
	Float3 partsSize;

	bool isFoward;
	float temp;
public:
	Cube* body;

	ReportRobot_211201(Float3 position = {0,0,0});
	~ReportRobot_211201();

	void Render();
	void Update();
	void GUIRender();

	void UpdateTerrainUp(vector<VertexUV> vertices, UINT width, UINT height);

	void SetState(State type){state = type;}
};