#include <iostream>
#include <string>
#include "Graph.h"
using namespace std;

void TestGraphMatrix()
{
	string city[] = { "����", "����", "�Ӱ�", "μ��", "����" };
	GraphMatrix<string, double> g1(city, 5, -1, true);//�Ƿ�ֵΪ-1������ͼ
	g1.AddEdge("����", "����", 80.5);
	g1.AddEdge("����", "μ��", 19.5);
	g1.AddEdge("μ��", "�Ӱ�", 50.5);
	g1.AddEdge("����", "����", 5.5);
}

void TestGraphLink()
{
	string city[] = { "����", "����", "�Ӱ�", "μ��", "����" };
	GraphLink<string, double> g2(city, 5, true);//����ͼ
	g2.AddEdge("����", "����", 70.5);
	g2.AddEdge("μ��", "�Ӱ�", 40.5);
	g2.AddEdge("����", "μ��", 19.5);
	g2.AddEdge("�Ӱ�", "����", 5.5);
	g2.AddEdge("����", "μ��", 30.5);
}
int main()
{
	//TestGraphMatrix();
	TestGraphLink();
	system("pause");
	return 0;
}