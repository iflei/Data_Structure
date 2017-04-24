#include "BinaryTree.h"
#include <iostream>
using namespace std;

int main()
{
	int a1[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	int a2[15] = { 1, 2, '#', 3, '#', '#', 4, 5, '#', 6, '#', 7, '#', '#', 8 };
	BinaryTree<int> t1(a1, 10, '#');
	BinaryTree<int> t2(a2, 15, '#');
	t1.PostOrder();
	t2.PrevOrder();

	system("pause");
	return 0;
}