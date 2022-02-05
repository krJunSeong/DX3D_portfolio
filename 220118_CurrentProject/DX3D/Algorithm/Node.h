#pragma once

class Node : public BoxCollider
{
public:
    enum State
    {
        NONE, OPEN, CLOSED, USING, OBSTACLE
    };

private:
    friend class AStar;

    struct Edge
    {
        int index;
        float cost;
    };

    int index;
    int via;

    float f, g, h;

    State state;

    vector<Edge*> edges;

public:
    Node(Vector3 pos, int index);
    ~Node();

    void Render();

    void AddEdge(Node* node);

    void SetState(State state) { this->state = state; }

    float GetCost() { return f; }
};
/*
	OPNE: 갈 수 있는 경우의 수
	CLOSED: 갈 수 있는 경우의 수의 확장이 끝난 애들
	via: 경로
	f: g + h, 최종효율
	Astar 효율:

	시작 노드부터 현재 계산하는 노드까지의 효율
	H: 휴리스틱, 추정가정치
				노드부터 목적지까지의 거리, 효율
*/