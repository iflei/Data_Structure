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
	cout << "测试堆" << endl;
	testHeap();
	cout << "测试优先级队列" << endl;
	testPriorityQueue();
	
	system("pause");
	return 0;
}
