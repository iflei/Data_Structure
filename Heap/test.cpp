#include <iostream>
#include "Heap.h"
#include "PriorityQueue.h"
using namespace std;

void testHeap()
{
	int a[] = { 10, 11, 13, 12, 16, 18, 15, 17, 14, 19 };
	Heap<int> hp1(a, sizeof(a) / sizeof(a[0]));
	hp1.Pop();
	hp1.Push(20);
	cout << hp1.Top() << endl;
}
void testPriorityQueue()
{
	int a[] = { 10, 11, 13, 12, 16, 18, 15, 17, 14, 19 };
	PriorityQueue<int, Less<int> > pq(a, sizeof(a) / sizeof(a[0]));
	pq.Pop();
	pq.Push(20);
	cout << pq.Top() << endl;
}

int main()
{
	cout << "���Զ�" << endl;
	testHeap();
	cout << "�������ȼ�����" << endl;
	testPriorityQueue();
	
	system("pause");
	return 0;
}
