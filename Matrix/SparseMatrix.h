#pragma once
#include <vector>

//��Ԫ��
template <typename T>
struct Triple
{
	T _value;
	size_t _row;
	size_t _col;

	Triple()
	{}

	Triple(size_t row, size_t col, const T& value)
		:_row(row)
		, _col(col)
		, _value(value)
	{}
};

//ϡ�����ѹ���洢
template <typename T>
class SparseMatrix
{
public:
	SparseMatrix()
	{}

	SparseMatrix(T* a, size_t rowSize, size_t colSize, const T& invalid = T() )
		:_rowSize(rowSize)
		, _colSize(colSize)
		, _invalid(invalid)
	{
		for (size_t i = 0; i < rowSize; ++i)
		{
			for (size_t j = 0; j < colSize; ++j)
			{
				if (a[i*colSize + j] != invalid)
				{
					_a.push_back(Triple<T>(i, j, a[i*colSize + j]));
				}
			}
		}
	}

	void Display()
	{
		size_t index = 0;
		for (size_t i = 0; i < _rowSize; ++i)
		{
			for (size_t j = 0; j < _colSize; ++j)
			{
				//����Ҫ��ֹԽ��index < _a.size()
				if (index < _a.size() && i == _a[index]._row && j == _a[index]._col)
				{
					cout << _a[index]._value <<" ";
					++index;
				}
				else
				{
					cout << _invalid << " ";
				}
			}
			cout << endl;
		}
	}

	//��ͨת��
	SparseMatrix<T> Transport()
	{
		SparseMatrix<T> tmp;
		//������
		tmp._rowSize = _colSize;
		tmp._colSize = _rowSize;
		tmp._invalid = _invalid;
		//��reserve����
		tmp._a.reserve(_a.size());
		//������
		for (size_t j = 0; j < _colSize; ++j)
		{
			//Ѱ��ÿ�е���ЧԪ�أ���Ҫ����һ��vector
			size_t index = 0;
			while (index < _a.size())
			{
				//��Ԫ����е���j
				if (_a[index]._col == j)
				{
					//�������У�Ȼ��push_back
					tmp._a.push_back(Triple<T>(_a[index]._col, _a[index]._row, _a[index]._value));
				}
				++index;
			}
		}
		//����Ĭ�Ͽ������캯��
		return tmp;
	}

	//����ת��
	SparseMatrix<T> FastTransport()
	{
		SparseMatrix<T> tmp;
		tmp._rowSize = _colSize;
		tmp._colSize = _rowSize;
		tmp._invalid = _invalid;
		//�������resize()����tmp._a���㹻�ռ䣬��Ϊ������Ҫ���±����vector
		//����Triple����Ĭ�Ϲ��캯��
		tmp._a.resize(_a.size(), Triple<T>());
		//ͳ��ת�ú����ÿһ�е����ݸ���
		int* rowCounts = new int[tmp._rowSize];
		memset(rowCounts, 0, sizeof(int) * tmp._rowSize);
		
		size_t index = 0;
		while (index < _a.size())
		{
			//_a[index]._col ���±꣬��Ӧת�ú���к�
			rowCounts[_a[index]._col]++;
			++index;
		}
		//ͳ��ת�ú�ľ���ÿ����ѹ�������еĴ洢�Ŀ�ʼλ��
		//rowStart���±���ת�ú���к�
		int* rowStart = new int[tmp._rowSize];
		rowStart[0] = 0;
		for (size_t i = 1; i < tmp._rowSize; ++i)
		{
			rowStart[i] = rowStart[i - 1] + rowCounts[i - 1];
		}

		//��ʼ����ת��
		//����ԭvectorԪ�أ���λ��λ��
		index = 0;
		while (index < _a.size())
		{
			//ԭ����Ԫ�������ת�ú���У�Ҳ��rowStart���±�
			size_t row = _a[index]._col;
			//ֱ�Ӷ�λԭѹ��ϡ������Ԫ�ص�ת�ú�ѹ������λ��
			tmp._a[rowStart[row]] = Triple<T>(_a[index]._col, _a[index]._row, _a[index]._value);
			//��row���Ѿ�������Ԫ�أ�����һ�е�rowStart++
			rowStart[row]++;
			++index;
		}
		delete[] rowCounts;
		delete[] rowStart;
		return tmp;
	}

protected:
	//vector�洢ѹ������
	vector< Triple<T> > _a;
	size_t _rowSize; //����
	size_t _colSize; //����
	T _invalid;      //�Ƿ�ֵ
};