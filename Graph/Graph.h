#pragma once
#include <vector>
#include <unordered_map>
using namespace std;

//邻接矩阵
template <typename V, typename W>
class GraphMatrix
{
public:
	GraphMatrix(V* vertexs, size_t n, const W& invalid = W(), bool isDirected = false)
		:_vertexs(vertexs, vertexs + n)
		, _isDirected(isDirected)
	{
		//创建矩阵
		_matrixs = new W*[_vertexs.size()];
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			_matrixs[i] = new W[_vertexs.size()];
			//用unordered_map记录顶点在vector中的下标
			_indexMap[vertexs[i]] = i;
			//初始化为非法值，非法值表示顶点之间不存在边
			for (size_t j = 0; j < _vertexs.size(); ++j)
				_matrixs[i][j] = invalid;
		}
	}

	void AddEdge(const V& v1, const V& v2, const W& w)
	{
		size_t src = _indexMap[v1];
		size_t dst = _indexMap[v2];

		_matrixs[src][dst] = w;
		//如果是无向图
		if (_isDirected == false)
			_matrixs[dst][src] = w;
	}

private:
	vector<V> _vertexs; //定点集合
	W** _matrixs; //邻接矩阵
	bool _isDirected; //是否有向
	unordered_map<V, size_t> _indexMap;
};



template <typename W>
struct LinkEdge
{
	size_t _src;//两个顶点
	size_t _dst;
	W _w; //权重
	LinkEdge<W>* _next;

	LinkEdge(size_t src = 0, size_t dst = 0, const W& w = W())
		: _src(src)
		, _dst(dst)
		, _w(w)
		, _next(NULL)
	{}
};

//邻接表
template <typename V, typename W>
class GraphLink
{
	typedef LinkEdge<W> Edge;
public:
	GraphLink(V* vertexs, size_t n, bool isDirected = false)
		:_vertexs(vertexs, vertexs + n)
		, _isDirected(isDirected)
	{
		//邻接表不需要设置非法值，初始化指针为空
		_linkTables.resize(n, NULL);

		//记录顶点下标
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
		//如果是无向图
		if (_isDirected == false)
			_AddEdge(dst, src, w);
	}

	void DFS(const V& src); //深度优先遍历-递归

	void BFS(const V& src); //广度优先遍历-队列
private:
	//插入LinkEdge节点
	void _AddEdge(size_t src, size_t dst, const W& w)
	{
		// 头插
		Edge* edge = new Edge(src, dst, w);
		edge->_next = _linkTables[src];
		_linkTables[src] = edge;
	}
private:
	vector<V> _vertexs; //顶点集合
	vector<Edge*> _linkTables; //邻接表
	bool _isDirected; //是否有向
	unordered_map<V, size_t> _indexMap;
};