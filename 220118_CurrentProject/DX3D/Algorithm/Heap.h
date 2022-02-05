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
	�迭�� ���� �� �ǰ� Ʈ�����·� ���� �Ѵ�.
	1, 2, 3, 4

	�Ӹ���:
		        1(0)
			2(1)	 3(2)
		  4(3)


	bool Empty() { return heap.empty();} // ����ִ��� �� ����ִ��� üũ�ϴ� ��
*/