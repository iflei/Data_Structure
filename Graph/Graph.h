#pragma once
#include <vector>
#include <unordered_map>
using namespace std;

//�ڽӾ���
template <typename V, typename W>
class GraphMatrix
{
public:
	GraphMatrix(V* vertexs, size_t n, const W& invalid = W(), bool isDirected = false)
		:_vertexs(vertexs, vertexs + n)
		, _isDirected(isDirected)
	{
		//��������
		_matrixs = new W*[_vertexs.size()];
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			_matrixs[i] = new W[_vertexs.size()];
			//��unordered_map��¼������vector�е��±�
			_indexMap[vertexs[i]] = i;
			//��ʼ��Ϊ�Ƿ�ֵ���Ƿ�ֵ��ʾ����֮�䲻���ڱ�
			for (size_t j = 0; j < _vertexs.size(); ++j)
				_matrixs[i][j] = invalid;
		}
	}

	void AddEdge(const V& v1, const V& v2, const W& w)
	{
		size_t src = _indexMap[v1];
		size_t dst = _indexMap[v2];

		_matrixs[src][dst] = w;
		//���������ͼ
		if (_isDirected == false)
			_matrixs[dst][src] = w;
	}

private:
	vector<V> _vertexs; //���㼯��
	W** _matrixs; //�ڽӾ���
	bool _isDirected; //�Ƿ�����
	unordered_map<V, size_t> _indexMap;
};



template <typename W>
struct LinkEdge
{
	size_t _src;//��������
	size_t _dst;
	W _w; //Ȩ��
	LinkEdge<W>* _next;

	LinkEdge(size_t src = 0, size_t dst = 0, const W& w = W())
		: _src(src)
		, _dst(dst)
		, _w(w)
		, _next(NULL)
	{}
};

//�ڽӱ�
template <typename V, typename W>
class GraphLink
{
	typedef LinkEdge<W> Edge;
public:
	GraphLink(V* vertexs, size_t n, bool isDirected = false)
		:_vertexs(vertexs, vertexs + n)
		, _isDirected(isDirected)
	{
		//�ڽӱ���Ҫ���÷Ƿ�ֵ����ʼ��ָ��Ϊ��
		_linkTables.resize(n, NULL);

		//��¼�����±�
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			_indexMap[vertexs[i]] = i;
		}
	}

	void AddEdge(const V& v1, const V& v2, const W& w)
	{
		size_t src = _indexMap[v1];
		size_t dst = _indexMap[v2];

		_AddEdge(src, dst, w);
		//���������ͼ
		if (_isDirected == false)
			_AddEdge(dst, src, w);
	}

	void DFS(const V& src); //������ȱ���-�ݹ�

	void BFS(const V& src); //������ȱ���-����
private:
	//����LinkEdge�ڵ�
	void _AddEdge(size_t src, size_t dst, const W& w)
	{
		// ͷ��
		Edge* edge = new Edge(src, dst, w);
		edge->_next = _linkTables[src];
		_linkTables[src] = edge;
	}
private:
	vector<V> _vertexs; //���㼯��
	vector<Edge*> _linkTables; //�ڽӱ�
	bool _isDirected; //�Ƿ�����
	unordered_map<V, size_t> _indexMap;
};