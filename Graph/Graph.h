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

	void DFS(const V& src) //������ȱ���-�ݹ�
	{
		size_t index = _indexMap[src];
		
		//visited��ʶ�����Ƿ���ʹ���
		vector<bool> visited;
		visited.resize(_vertexs.size(), false);
		
		//�ȷ��ʶ��㼯����һ������
		cout << _vertexs[index] << ":" << index << "->";
		visited[index] = true; //��ʶ���ʹ���

		_DFS(index, visited);//Ȼ�������һ���ٽӶ���
		cout << endl;
	}

	void BFS(const V& src) //������ȱ���-����
	{
		queue<int> q;
		size_t index = _indexMap[src];
		q.push(index);

		vector<bool> visited;
		visited.resize(_vertexs.size(), false);

		while (!q.empty())
		{
			int front = q.front(); 
			if (visited[front] == false) //���û�з��ʹ�
			{
				cout << _vertexs[front] << ":" << front << "->";
				visited[front] = true;

				Edge* cur = _linkTables[front];
				while (cur) //�Ѻ͵�ǰ�����ڽӵ㣬û�з��ʹ��Ķ������
				{
					int dst = cur->_dst;
					if (visited[dst] == false)
					{
						q.push(dst);
					}

					cur = cur->_next;
				}
			}
			q.pop(); //�ö�����ʹ���
		}

		cout << endl;
	}
	
	bool Kruskal(GraphLink<V, W>& minTree)
	{
		//���������ͼֱ��return 
		if (_isDirected == true)
			return false;

		//minTree��Ĭ�Ϲ��캯�����ɵ�
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

		//С�ѣ���ȨֵС�ı��ڶѶ�
		priority_queue<Edge*, vector<Edge*>, Less> pq; 
		//�����ڽӱ�
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			Edge* cur = _linkTables[i];
			while (cur) //�ڽӱ����û�е�β
			{
				if (cur->_src < cur->_dst)
				{
					pq.push(cur);
					//���_srcС��_dst�Ͱ������߷������ȼ�����
					//_src < _dst ��֤һ����ֻ���һ��
				}
				cur = cur->_next;
			}
		}

		//countͳ����С��������ӵı���
		size_t count = 0;
		//�ò��鼯����һ�������еĶ���
		UnionFind _set(_vertexs.size());

		// ������С������
		while (!pq.empty())
		{
			Edge* edge = pq.top();//ȡһ����
			pq.pop();
			int src = edge->_src;
			int dst = edge->_dst;
			//��������ߵ��������㲻��һ������
			if (false == _set.Find(src, dst))
			{
				//����С���������������
				minTree._AddEdge(src, dst, edge->_w);
				minTree._AddEdge(dst, src, edge->_w);
				_set.Union(src, dst); //���������������һ������
				++count;

				if (count == _vertexs.size() - 1)
				{
					return true;
				}
			}
		}
		//������ȼ���������ˣ�˵�����б߶�����
		return false;
	}


	bool Prim(GraphLink& minTree)
	{
		if (_isDirected == true)
			return false;

		// 1.��ʼ����С������
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

		//��0��ʼ
		int src = 0;
		visited[src] = true;
		
		int count = 0;
		while (count < _vertexs.size());
		{
			// 2.ȡ��һ����������δ���ʹ����ٽӱ߷ŵ�һ����С������
			Edge* cur = _linkTables[src];
			while (cur)
			{
				//�����ǰ�����Ŀ�궥��û�з���
				if (visited[cur->_dst] == false)
				{
					pq.push(cur); //�ѵ�ǰ�߷������ȼ�������
				}
				cur = cur->_next;
			}

			// 2.ѡ��������С�ı߼���������
			while (!pq.empty() && count < _vertexs.size())
			{
				cur = pq.top(); //ȡ����ǰ�����ٽӵ���С��
				pq.pop();

				//�����ǰ�����Ŀ�궥��û�з���
				//��������ˣ�����������ȼ�������ȡ��С�ı�
				if (visited[cur->_dst] == false)
				{
					//��С��������ӱ�
					minTree._AddEdge(cur->_src, cur->_dst, cur->_w);
					visited[cur->_dst] = true; //Ŀ�궥����ʹ���
					src = cur->_dst; //����ѡ����Ŀ�궥�����
					++count;

					break;
				}
			}
		}

		return true;
	}
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