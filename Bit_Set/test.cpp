#include <iostream>
#include "BitMap.h"
#include "BloomFilter.h"
using namespace std;

int main()
{
	BitMap bt(1024);
	bt.Set(121);
	bt.Set(348);
	bt.Set(870);
	bt.Set(911);
	bt.Set(1000);
	cout << "348? " << bt.Test(348) << endl;
	bt.ReSet(348);
	cout << "348? " << bt.Test(348) << endl;

	BloomFilter<string> bl;
	char* url1 = "http://www.iflei.com/linux-basis.html";
	char* url2 = "http://www.iflei.com/heap-and-priority-queue.html";
	char* url3 = "http://www.iflei.com/the-compressed-storage-and-fast-transpose-of-matrix.html";
	char* url4 = "http://www.iflei.com/analog-implement-the-vector-of-stl.html";
	char* url5 = "http://www.iflei.com/the-optimal-solution-of-the-maze-problem.html";
	char* url6 = "http://www.iflei.com/commonly-used-simple-configuration-of-vim.html";
	char* url7 = "http://www.iflei.com/reversed-polish-notation.html";
	char* url8 = "http://www.iflei.com/git-tool.html";
	char* url9 = "http://www.iflei.com/memory-alignment.html";
	char* url10 = "http://www.iflei.com/the-difference-between-the-c-and-cpp.html";
	char* url11 = "http://www.iflei.com/taught-you-how-to-install-win10-system.html";
	char* url12 = "http://www.iflei.com/process-communication-of-message-queue.html";
	char* url13 = "http://www.iflei.com/installation-script-to-lnmp.html";
	char* url14 = "http://www.iflei.com/the-progress-bar.html";
	char* url15 = "http://www.iflei.com/threaded-binary-tree.html";
	char* url16 = "http://www.iflei.com/complex-list-of-replication.html";
	char* url17 = "http://www.iflei.com/cpp-implement-queue.html";
	char* url18 = "http://www.iflei.com/fibonacci-array.html";
	char* url19 = "http://www.iflei.com/shared_ptr.html";
	char* url20 = "http://www.iflei.com/cpp-double-linked-list.html";

	bl.Set(url1);
	bl.Set(url2);
	bl.Set(url3);
	bl.Set(url4);
	bl.Set(url5);
	bl.Set(url6);
	bl.Set(url7);
	bl.Set(url8);
	bl.Set(url9);
	bl.Set(url10);
	bl.Set(url11);
	bl.Set(url12);
	bl.Set(url13);
	bl.Set(url14);
	bl.Set(url15);
	bl.Set(url16);
	bl.Set(url17);
	bl.Set(url18);
	bl.Set(url19);
	bl.Set(url20);

	cout << "url1? " << bl.Test(url1) << endl;
	cout << "url6? " << bl.Test(url6) << endl;
	cout << "!url3? " << bl.Test("http://www.iflei.com/the-compressed-storage-and-fast-transpose-of-matrix") << endl;
	cout << "!url16? " << bl.Test("http://iflei.com/complex-list-of-replication.html") << endl;
	cout << "!url11? " << bl.Test("www.iflei.com/taught-you-how-to-install-win10-system.html") << endl;
	system("pause");
	return 0;
}