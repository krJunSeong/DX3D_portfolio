#pragma once

class Heap
{
private:
    vector<Node*> heap;

public:
    Heap();
    ~Heap();

    void Insert(Node* node);
    void UpdateUpper();

    Node* DeleteRoot();
    void UpdateLower();

    void Swap(int n1, int n2);

    void Clear();

    bool Empty() { return heap.empty(); }
};

/* Heap.h
	배열로 보면 안 되고 트리형태로 봐야 한다.
	1, 2, 3, 4

	머릿속:
		        1(0)
			2(1)	 3(2)
		  4(3)


	bool Empty() { return heap.empty();} // 비어있는지 안 비어있는지 체크하는 거
*/