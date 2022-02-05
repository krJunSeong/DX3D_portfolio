#include "Framework.h"

Heap::Heap()
{
}

Heap::~Heap()
{
}

void Heap::Insert(Node* node)
{
    heap.push_back(node);
    UpdateUpper();
}

void Heap::UpdateUpper()
{
    int curIndex = heap.size() - 1;
    int parent = (curIndex - 1) / 2;

    while (curIndex != 0)
    {
        if (heap[parent]->GetCost() <= heap[curIndex]->GetCost())
            break;

        Swap(curIndex, parent);
        curIndex = parent;
        parent = (curIndex - 1) / 2;
    }
}

Node* Heap::DeleteRoot()
{
    Node* root = heap.front();
    Swap(0, heap.size() - 1);
    heap.pop_back();
    UpdateLower();

    return root;
}

void Heap::UpdateLower()
{
    int curIndex = 0;
    int lChild = 1;
    int rChild = 2;
    int minNode = curIndex;

    while (true)
    {
        if (lChild < heap.size() && heap[lChild]->GetCost() < heap[minNode]->GetCost())
            minNode = lChild;

        if (rChild < heap.size() && heap[rChild]->GetCost() < heap[minNode]->GetCost())
            minNode = rChild;

        if (minNode == curIndex)
            break;

        Swap(curIndex, minNode);
        curIndex = minNode;
        lChild = curIndex * 2 + 1;
        rChild = lChild + 1;
    }
}

void Heap::Swap(int n1, int n2)
{
    swap(heap[n1], heap[n2]);
}

void Heap::Clear()
{
    heap.clear();
}




/* Heap.cpp

    UpdateUpper
        부모랑 자식이랑 값을 비교해서 효율 넘버가 낮은 얘(효율좋은얘) 가 맨 위로 올라간다.

    int parent = (curIndex - 1) / 2;
        부모인덱스 잡는 것

Node* Heap::DeleteRoot()
        제일 마지막 노드와 스왑을 해준다.
        그다음 제일 마지막을 지운다
        하향식으로 만든다.
        자식 둘 중에 작은 거와 부모랑 비교한다.
        더 작은 거랑 부모랑 바꾼다.

        자식이랑 비교한다.
        아니라면 스탑.

void Heap::UpdateLower()
        // int lChild = curIndex * 2 + 1; 공식


Heap
Astar의 원리
    경우의 수와 효율
        시작노드 기준으로 갈 수 있는 경우의 수를 계산한다.
        그리고 그 경우의 수들을 오픈 노드라고 해서 확장,. 확장된 노드로 들어간다.
        확장이 끝난 노드는 클로즈드로 닫는다.

    이 노드들 중에서 제일 좋은 거를 찾고, 얘 기준으로 또 확장해서 찾는다.
*/