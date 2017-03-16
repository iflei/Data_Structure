#include <iostream>
#include <string>
#include "HashTable.h"
using namespace std;

int main()
{
	HashTable<int, int> ht1(10);
	ht1.Insert(make_pair(51, 0));
	ht1.Insert(make_pair(105, 0));
	ht1.Insert(make_pair(52, 0));
	ht1.Insert(make_pair(3, 0));
	ht1.Insert(make_pair(55, 0));
	ht1.Insert(make_pair(2, 0));
	ht1.Insert(make_pair(106, 0));
	ht1.Insert(make_pair(53, 0));
	ht1.Insert(make_pair(0, 0));
	HashTable<int, int>::Iterator it = ht1.Begin();
	while (it != ht1.End())
	{
		cout << it->first << ":" << it->second << endl;
		++it;
	}

	HashTable<string, int> ht2(10);
	ht2.Insert(make_pair("apple", 0));
	ht2.Insert(make_pair("orange", 1));
	ht2.Insert(make_pair("banana", 2));

	ht2["apple"]++;
	ht2["orange"]++;
	ht2["banana"]++;
	HashTable<string, int>::Iterator it2 = ht2.Begin();
	while (it2 != ht2.End())
	{
		cout << it2->first << ":" << it2->second << endl;
		++it2;
	}
	system("pause");
	return 0;
}