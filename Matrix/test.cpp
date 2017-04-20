#include "SymmetricMatrix.h"
#include "SparseMatrix.h"

int main()
{
	cout << "²âÊÔ¶Ô³Æ¾ØÕó" << endl;
	int a[5][5] =
	{
		{ 0, 1, 2, 3, 4 },
		{ 1, 0, 1, 2, 3 },
		{ 2, 1, 0, 1, 2 },
		{ 3, 2, 1, 0, 1 },
		{ 4, 3, 2, 1, 0 },
	};
	SymmetricMatrix<int> sm((int*)a, 5);
	sm.Display();
	cout << endl;
	cout << "²âÊÔÏ¡Êè¾ØÕó" << endl;
	int arr[6][5] =
	{
		{ 1, 0, 3, 0, 5 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 2, 0, 4, 0, 6 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 }
	};
	SparseMatrix<int> sm2((int*)arr, 6, 5, 0);
	sm2.Display();
	cout << endl;
	cout << "²âÊÔÆÕÍ¨×ªÖÃ" << endl;
	SparseMatrix<int> sm3 = sm2.Transport();
	sm3.Display();
	cout << endl;
	cout << "²âÊÔ¿ìËÙ×ªÖÃ" << endl;
	SparseMatrix<int> sm4 = sm2.FastTransport();
	sm4.Display();
	cout << endl;
	system("pause");
	return 0;
}