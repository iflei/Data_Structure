#include <iostream>
#include <string>
#include "Graph.h"
using namespace std;

void TestGraphMatrix()
{
	string city[] = { "西安", "安康", "延安", "渭南", "咸阳" };
	GraphMatrix<string, double> g1(city, 5, -1, true);//非法值为-1，有向图
	g1.AddEdge("西安", "安康", 80.5);
	g1.AddEdge("西安", "渭南", 19.5);
	g1.AddEdge("渭南", "延安", 50.5);
	g1.AddEdge("西安", "咸阳", 5.5);
}

void TestGraphLink()
{
	string city[] = { "西安", "安康", "延安", "渭南", "咸阳" };
	GraphLink<string, double> g2(city, 5, true);//有向图
	g2.AddEdge("西安", "安康", 70.5);
	g2.AddEdge("渭南", "延安", 40.5);
	g2.AddEdge("西安", "渭南", 19.5);
	g2.AddEdge("延安", "咸阳", 5.5);
	g2.AddEdge("咸阳", "渭南", 30.5);
}
int main()
{
	//TestGraphMatrix();
	TestGraphLink();
	system("pause");
	return 0;
}