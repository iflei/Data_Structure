#include <iostream>
#include "UnionFind.h"
using namespace std;

int main()
{
	// 0 1 2 3 4 
	int r[3][2] = { { 0, 1 }, { 1, 2 }, { 3, 4 } };
	UnionFind set(5); //�������4�����Կ�5��
	for (int i = 0; i < 3; i++)
	{
		set.Union(r[i][0], r[i][1]);
	}

	cout << "0��2��ͬһ������? " << set.Find(0, 2) << endl;
	cout << "1��4��ͬһ������? " << set.Find(1, 4) << endl;
	cout << "�м�������? " << set.CountRoot() << endl;

	system("pause");
	return 0;
}