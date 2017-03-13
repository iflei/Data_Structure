#pragma once
#include <vector>
using namespace std;

class BitMap
{
public:
	BitMap(size_t num = 1024)
	{
		//��Ҫ����λ�Ϳ��Ա�ʾ���ٸ�����״̬
		_array.resize((num >> 5) + 1); //bit / 32 + 1
	}

	//����ĳ��������
	void Set(size_t value)
	{//0 - 31  32 - 63
		size_t index = value >> 5; //�����ӳ�䵽�������Ǹ�Ԫ����
		size_t num = value % 32; //ӳ��ĳ��Ԫ�ص���λ��

		_array[index] |= (1 << num);
	}
	//����ĳ����������
	void ReSet(size_t value)
	{
		size_t index = value >> 5;
		size_t num = value % 32;

		_array[index] &= ~(1 << num);
	}
	//���ĳ�����Ƿ����
	bool Test(size_t value)
	{
		size_t index = value >> 5;
		size_t num = value % 32;

		return _array[index] & (1 << num);
	}

protected:
	vector<size_t> _array;
};