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

	void DFS(const V& src) //深度优先遍历-递归
	{
		size_t index = _indexMap[src];
		
		//visited标识顶点是否访问过了
		vector<bool> visited;
		visited.resize(_vertexs.size(), false);
		
		//先访问顶点集合里一个顶点
		cout << _vertexs[index] << ":" << index << "->";
		visited[index] = true; //标识访问过了

		_DFS(index, visited);//然后访问下一个临接顶点
		cout << endl;
	}

	void BFS(const V& src) //广度优先遍历-队列
	{
		queue<int> q;
		size_t index = _indexMap[src];
		q.push(index);

		vector<bool> visited;
		visited.resize(_vertexs.size(), false);

		while (!q.empty())
		{
			int front = q.front(); 
			if (visited[front] == false) //如果没有访问过
			{
				cout << _vertexs[front] << ":" << front << "->";
				visited[front] = true;

				Edge* cur = _linkTables[front];
				while (cur) //把和当前顶点邻接点，没有访问过的都入队列
				{
					int dst = cur->_dst;
					if (visited[dst] == false)
					{
						q.push(dst);
					}

					cur = cur->_next;
				}
			}
			q.pop(); //该顶点访问过了
		}

		cout << endl;
	}
	
	bool Kruskal(GraphLink<V, W>& minTree)
	{
		//如果是有向图直接return 
		if (_isDirected == true)
			return false;

		//minTree是默认构造函数生成的
		minTree._isDirected = _isDirected;
		minTree._vertexs = _vertexs;
		minTree._linkTables.resize(_vertexs.size(), NULL);

		struct Less
		{
			bool operator()(Edge* left, Edge* right)
			{
				return left->_w < right->_w;
			}
		};

		//小堆，把权值小的边在堆顶
		priority_queue<Edge*, vector<Edge*>, Less> pq; 
		//遍历邻接表
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			Edge* cur = _linkTables[i];
			while (cur) //邻接表的链没有到尾
			{
				if (cur->_src < cur->_dst)
				{
					pq.push(cur);
					//如果_src小于_dst就把这条边放入优先级队列
					//_src < _dst 保证一条边只添加一次
				}
				cur = cur->_next;
			}
		}

		//count统计最小生成树添加的边数
		size_t count = 0;
		//用并查集保存一个集合中的顶点
		UnionFind _set(_vertexs.size());

		// 构建最小生成树
		while (!pq.empty())
		{
			Edge* edge = pq.top();//取一条边
			pq.pop();
			int src = edge->_src;
			int dst = edge->_dst;
			//如果这条边的两个顶点不在一个集合
			if (false == _set.Find(src, dst))
			{
				//给最小生成树添加这条边
				minTree._AddEdge(src, dst, edge->_w);
				minTree._AddEdge(dst, src, edge->_w);
				_set.Union(src, dst); //把这两个顶点放入一个集合
				++count;

				if (count == _vertexs.size() - 1)
				{
					return true;
				}
			}
		}
		//如果优先级队列里空了，说明所有边都用了
		return false;
	}


	bool Prim(GraphLink& minTree)
	{
		if (_isDirected == true)
			return false;

		// 1.初始化最小生成树
		minTree._vertexs = _vertexs;
		minTree._linkTables.resize(_vertexs.size());
		minTree._isDirected = _isDirected;

		vector<bool> visited;
		visited.resize(_vertexs.size(), false);

		struct Less
		{
			bool operator()(Edge* left, Edge* right)
			{
				return left->_w < right->_w;
			}
		};
		priority_queue<Edge*, vector<Edge*>, Less> pq;

		//从0开始
		int src = 0;
		visited[src] = true;
		
		int count = 0;
		while (count < _vertexs.size());
		{
			// 2.取出一个顶点所有未访问过的临接边放到一个最小堆里面
			Edge* cur = _linkTables[src];
			while (cur)
			{
				//如果当前顶点的目标顶点没有访问
				if (visited[cur->_dst] == false)
				{
					pq.push(cur); //把当前边放入优先级队列中
				}
				cur = cur->_next;
			}

			// 2.选出堆中最小的边加入生成树
			while (!pq.empty() && count < _vertexs.size())
			{
				cur = pq.top(); //取出当前顶点临接的最小边
				pq.pop();

				//如果当前顶点的目标顶点没有访问
				//如果访问了，会继续从优先级队列里取次小的边
				if (visited[cur->_dst] == false)
				{
					//最小生成树添加边
					minTree._AddEdge(cur->_src, cur->_dst, cur->_w);
					visited[cur->_dst] = true; //目标顶点访问过了
					src = cur->_dst; //接着选出的目标顶点继续
					++count;

					break;
				}
			}
		}

		return true;
	}
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