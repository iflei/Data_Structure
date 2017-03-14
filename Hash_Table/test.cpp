#include <iostream>
#include "HashTable.h"
using namespace std;

int main()
{
	HashTable<int, int> ht1(10);
	ht1.Insert(3, 3);
	ht1.Insert(12, 12);
	ht1.Insert(29, 20);
	ht1.Insert(13, 13);
	ht1.Insert(21, 21); 
	ht1.Insert(22, 22);
	ht1.Insert(6, 16);
	ht1.Insert(34,34);
	ht1.Insert(35, 35);
	cout << "find 12? " << ht1.Find(12) << endl;
	cout << "find 29? " << ht1.Find(29) << endl;
	ht1.Erase(12);
	cout << "find 12? " << ht1.Find(12) << endl;
	HashTable<string, string> ht2(10);
	ht2.Insert("apple", "Æ»¹û");
	ht2.Insert("orange", "½Û×Ó");
	ht2.Insert("banana", "Ïã½¶");

	system("pause");
	return 0;
}
