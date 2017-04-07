//队列(链表实现)
template <typename T>
struct QueueNode
{
	T _data;
	QueueNode<T>* _next;

	QueueNode(const T& e)
		:_data(e)
		, _next(NULL)
	{}
};

template <typename T>
class Queue
{
	typedef QueueNode<T> Node;
public:
	Queue()
		:_head(NULL)
		, _tail(NULL)
		, _size(0)
	{}

	~Queue()
	{
		while (_head)
		{
			Node* del = _head;
			_head = _head->_next;
			delete del;
		}
		_head = NULL;
		_tail = NULL;
		_size = 0;
	}

	void Push(const T& e)
	{
		//没有结点
		if (_head == NULL)
		{
			_head = new Node(e);
			_tail = _head;
		}
		//有结点
		else
		{
			_tail->_next = new Node(e);
			_tail = _tail->_next;
		}
		++_size;
	}

	void Pop()
	{
		//只有一个结点/无结点
		if (_head == _tail)
		{
			delete _head;
			_head = NULL;
			_tail = NULL;
		}
		//多个结点
		else
		{
			Node* del = _head;
			_head = _head->_next;
			delete del;
		}
		--_size;
	}

	T& Front()
	{
		assert(_head);
		return _head->_data;
	}

	T& Back()
	{
		assert(_tail);
		return _tail->_data;
	}

	bool Empty()
	{
		return _head == NULL;
	}

	size_t Size()
	{
		return _size;
	}

private:
	Node* _head;
	Node* _tail;
	size_t _size;
};


void testQueue()
{
	Queue<int> q;
	q.Push(1);
	q.Push(2);
	q.Push(3);
	q.Push(4);
	q.Push(5);
	size_t size = q.Size();
	for (size_t i = 0; i < size; ++i)
	{
		cout << q.Front() << endl;
		q.Pop();
	}
	
}