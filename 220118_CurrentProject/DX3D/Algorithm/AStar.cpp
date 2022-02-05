#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
    heap = new Heap();
}

AStar::~AStar()
{
    for (Node* node : nodes)
        delete node;

    delete heap;
}

void AStar::Update()
{
    if (MOUSE_CLICK(1))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);

        for (Node* node : nodes)
        {
            if (node->RayCollision(ray))
            {
                node->SetState(Node::OBSTACLE);
                break;
            }
        }
    }
}

void AStar::Render()
{
    for (Node* node : nodes)
        node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
    Float2 size = terrain->GetSize();

    interval.x = size.x / width;
    interval.y = size.y / height;

    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = terrain->GetHeight(pos);

            nodes.push_back(new Node(pos, nodes.size()));
        }
    }

    SetEdge();
}

void AStar::SetNode(Quad* quad)
{
    Float2 size = quad->GetSizeScale();

    interval.x = size.x / width;
    interval.y = size.y / height;

    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = quad->position.y;

            nodes.push_back(new Node(pos, nodes.size()));
        }
    }

    SetEdge();
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
    Reset();
    path.clear();

    //1. StartNode Init
    float G = 0.0f;
    float H = GetDistance(start, end);

    nodes[start]->g = G;
    nodes[start]->h = H;
    nodes[start]->f = G + H;
    nodes[start]->via = start;
    nodes[start]->state = Node::OPEN;

    heap->Insert(nodes[start]);

    while (nodes[end]->state != Node::CLOSED)
    {
        if (heap->Empty())
            return;

        int curIndex = GetMinNode();//Find MinNode In OpenNodes
        Extend(curIndex, end);//Add OpenNode
        nodes[curIndex]->state = Node::CLOSED;
    }

    //Back Tracking
    int curIndex = end;
    while (curIndex != start)
    {
        nodes[curIndex]->state = Node::USING;
        path.push_back(nodes[curIndex]->position);
        curIndex = nodes[curIndex]->via;
    }

    path.push_back(nodes[start]->position);
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
    int cutNodeNum = 0;
    Ray ray;
    ray.position = start;

    for (UINT i = 0; i < path.size(); i++)
    {
        ray.direction = path[i] - ray.position;
        float distance = ray.direction.Length();

        if (!CollisionObstacle(ray, distance))
        {
            cutNodeNum = path.size() - i - 1;
            break;
        }
    }

    for (int i = 0; i < cutNodeNum; i++)
        path.pop_back();

    cutNodeNum = 0;
    ray.position = end;

    for (UINT i = 0; i < path.size(); i++)
    {
        ray.direction = path[path.size() - i - 1] - ray.position;
        float distance = ray.direction.Length();

        if (!CollisionObstacle(ray, distance))
        {
            cutNodeNum = path.size() - i - 1;
            break;
        }
    }

    for (int i = 0; i < cutNodeNum; i++)
        path.erase(path.begin());
}

int AStar::FindCloseNode(Vector3 pos)
{
    float minDist = FLT_MAX;
    int index = -1;

    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->state == Node::OBSTACLE)
            continue;

        float dist = Distance(pos, nodes[i]->position);
        if (minDist > dist)
        {
            minDist = dist;
            index = i;
        }
    }

    return index;
}

bool AStar::CollisionObstacle(Ray ray, float distance)
{
    for (Collider* obstalce : obstacles)
    {
        Contact contact;

        if (obstalce->RayCollision(ray, &contact))
        {
            if (contact.distance < distance)
                return true;
        }
    }

    return false;
}

void AStar::SetObstalceNode()
{
    for (Collider* obstacle : obstacles)
    {
        for (Node* node : nodes)
        {
            if (obstacle->Collision(node))
            {
                node->state = Node::OBSTACLE;
            }
        }
    }
}

void AStar::Reset()
{
    for (Node* node : nodes)
    {
        if (node->state != Node::OBSTACLE)
            node->state = Node::NONE;
    }

    heap->Clear();
}

float AStar::GetDistance(int start, int end)
{
    Vector3 startPos = nodes[start]->position;
    Vector3 endPos = nodes[end]->position;

    Vector3 temp = endPos - startPos;

    float x = abs(temp.x);
    float y = abs(temp.y);

    float minSize = min(x, y);
    float maxSize = max(x, y);

    return sqrt(minSize * minSize * 2) + (maxSize - minSize);
}

int AStar::GetMinNode()
{
    return heap->DeleteRoot()->index;
}

void AStar::Extend(int center, int end)
{
    for (Node::Edge* edge : nodes[center]->edges)
    {
        int index = edge->index;

        if (nodes[index]->state == Node::CLOSED)
            continue;
        if (nodes[index]->state == Node::OBSTACLE)
            continue;

        float G = nodes[center]->g + edge->cost;
        float H = GetDistance(index, end);
        float F = G + H;

        if (nodes[index]->state == Node::OPEN)
        {
            if (F < nodes[index]->f)
            {
                nodes[index]->g = G;
                nodes[index]->f = F;
                nodes[index]->via = center;
            }
        }
        else if (nodes[index]->state == Node::NONE)
        {
            nodes[index]->g = G;
            nodes[index]->h = H;
            nodes[index]->f = F;
            nodes[index]->via = center;
            nodes[index]->state = Node::OPEN;

            heap->Insert(nodes[index]);
        }
    }
}

void AStar::SetEdge()
{
    for (UINT i = 0; i < nodes.size(); i++)
    {
        if (i % (width + 1) != width)
        {
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - width - 1)
        {
            nodes[i]->AddEdge(nodes[i + width + 1]);
            nodes[i + width + 1]->AddEdge(nodes[i]);
        }

        if (i % (width + 1) != width && i < nodes.size() - width - 1)
        {
            nodes[i]->AddEdge(nodes[i + width + 2]);
            nodes[i + width + 2]->AddEdge(nodes[i]);
        }

        if (i % (width + 1) != 0 && i < nodes.size() - width - 1)
        {
            nodes[i]->AddEdge(nodes[i + width]);
            nodes[i + width]->AddEdge(nodes[i]);
        }
    }
}
