#include <iostream>
#include <string>
#include "RBTree.h"
using namespace std;

int main()
{
	RBTree<string, int> countTree;
	countTree["apple"]++;
	countTree["banana"]++;
	countTree["apple"]++;
	countTree["banana"]++;
	countTree["orange"]++;
	countTree["apple"]++;
	countTree["orange"]++;
	cout << "IsBalance? " << countTree.IsBalace() << endl;
	countTree.InOrder();
	cout << countTree["apple"] << endl;
	system("pause");
	return 0;
}