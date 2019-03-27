#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
class Matrix
{
private:
	class Row
	{
	private:
		size_t columns;
		std::vector<int> row;
	public:
		Row(size_t columns_num) : columns(columns_num), row(std::vector<int>(columns_num))
		{
		}
		Row(const Row& another) : columns(another.getColumns()), row(std::vector<int>(another.row))
		{
		}
		/*Row(Row&& another) : columns(another.columns), row(std::vector<int>(another.row))
		{
			another.columns = 0;
			another.row.clear();
		}*/
		size_t getColumns() const
		{
			return columns;
		}
		const int& operator[](size_t i) const
		{
			if(i >= columns)
				throw std::out_of_range("Out of size");
			else
				return row[i];
		}
		int& operator[](size_t i)
		{
			if(i >= columns)
				throw std::out_of_range("Out of size");
			else
				return row[i];
		}
		Row& operator=(const Row& another)
		{
			columns = another.getColumns();
			row = std::vector<int>(another.row);
			return *this;
		}
		/*Row& operator=(Row&& another)
		{
			std::swap(columns, another.columns);
			row.swap(another.row);
			another.columns = 0;
			another.row.clear();
			return *this;
		}*/
		Row& operator*=(int i)
		{
			for(int k = 0; k < getColumns(); k++)
				row[k] *= i;
			return *this;
		}
		bool operator==(const Row& another) const
		{
			if(columns != another.getColumns())
				return false;
			for(int k = 0; k < getColumns(); k++)
				if(row[k] != another[k])
					return false;
			return true;
		}
		bool operator!=(const Row& another) const
		{
			return !(*this == another);
		}
		~Row()
		{
		}
	};
	size_t rows;
	size_t columns;
	std::vector<Row> data;
public:
	Matrix(size_t row_num, size_t columns_num) : rows(row_num), columns(columns_num), data(std::vector<Row>(rows,columns))
	{
	}
	Matrix(const Matrix& another) : rows(another.getRows()), columns(another.getColumns()), data(std::vector<Row>(another.data))
	{
	}
	/*Matrix(Matrix&& another) : rows(another.getRows()), columns(another.getColumns()), data(std::vector<Row>(another.data))
	{
		another.rows = 0;
		another.columns = 0;
		another.data.clear();
	}*/
	size_t getRows() const
	{
		return rows;
	}
	size_t getColumns() const
	{
		return columns;
	}
	const Row& operator[](size_t i) const
	{
		if (i >= rows)
			throw std::out_of_range("Out of size");
		return data[i];
	}
	Row& operator[](size_t i)
	{
		if (i >= rows)
			throw std::out_of_range("Out of size");
		return data[i];
	}
	Matrix& operator=(const Matrix& another)
	{
		rows = another.getRows();
		columns = another.getColumns();
		data = std::vector<Row>(another.data);
		return *this;
	}
	/*Matrix& operator=(Matrix&& another)
	{
		std::swap(rows, another.rows);
		std::swap(columns, another.columns);
		data.swap(another.data);
		another.rows = 0;
		another.columns = 0;
		another.data.clear();
		return *this;
	}*/
	Matrix& operator*=(int i)
	{
		for(int k = 0; k < getRows(); k++)
			data[k] *= i;
		return *this;
	}
	bool operator==(const Matrix& another) const
	{
		if(columns != another.getColumns() || rows != another.getRows())
			return false;
		for(int k = 0; k < getRows(); k++)
			if(data[k] != another[k])
				return false;
		return true;
	}
	bool operator!=(const Matrix& another) const
	{
		return !(*this == another);
	}
	~Matrix()
	{
	}
};
