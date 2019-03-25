#pragma once
#include <vector>
class Matrix
{
private:
	class Row
	{
	private:
		size_t columns;
		std::vector<int> row;
	public:
		Row(size_t columns_num = 0) : columns(columns_num) {
			row = std::vector<int>(columns_num);
		}
		Row(const Row& obj) {
			columns = obj.getColumns();
			row = std::vector<int>(obj.row);
		}
		Row(Row&& obj){
			columns = obj.getColumns();
			row = std::vector<int>(obj.row);
		}
		size_t getColumns() const{
			return columns;
		}
		const int& operator[](size_t i) const{
			if(i >= columns)
				throw std::out_of_range("Out of size");
			else
				return row[i];
		}
		int& operator[](size_t i){
			if(i >= columns)
				throw std::out_of_range("Out of size");
			else
				return row[i];
		}
		Row& operator=(const Row& smatr){
			columns = smatr.getColumns();
			row = std::vector<int>(smatr.row);
			return *this;
		}
		Row& operator=(Row&& smatr){
			columns = smatr.getColumns();
			row = std::vector<int>(smatr.row);
			return *this;
		}
		Row& operator*=(const int i){
			for(int k = 0; k < getColumns(); k++)
				row[k] *= i;
			return *this;
		}
		const bool operator==(const Row& smatr) const{
			if(columns != smatr.getColumns())
				return false;
			for(int k = 0; k < getColumns(); k++)
				if(row[k] != smatr[k])
					return false;
			return true;
		}
		const bool operator!=(const Row& smatr) const{
			if(columns != smatr.getColumns())
				return true;
			for(int k = 0; k < getColumns(); k++)
				if(row[k] != smatr[k])
					return true;
			return false;
		}
		~Row(){}
	};
	size_t rows;
	size_t columns;
	std::vector<Row> data;
public:
	Matrix(size_t row_num, size_t columns_num) : rows(row_num), columns(columns_num){
		data = std::vector<Row>(rows);
		for (size_t i = 0; i < rows; i++)
			data[i] = Row(columns);
	}
	Matrix(const Matrix& obj) {
		columns = obj.getColumns();
		rows = obj.getRows();
		data = std::vector<Row>(obj.data);
	}
	Matrix(Matrix&& obj){
		columns = obj.getColumns();
		rows = obj.getRows();
		data = std::vector<Row>(obj.data);
	}
	size_t getRows() const{
		return this->rows;
	}
	size_t getColumns() const{
		return this->columns;
	}
	const Row& operator[](size_t i) const{
		if (i >= columns)
			throw std::out_of_range("Out of size");
		return data[i];
	}
	Row& operator[](size_t i){
		if (i >= columns)
			throw std::out_of_range("Out of size");
		return data[i];
	}
	Matrix& operator=(const Matrix& smatr){
		columns = smatr.getColumns();
		rows = smatr.getRows();
		data = std::vector<Row>(smatr.data);
		return *this;
	}
	Matrix& operator=(Matrix&& smatr){
		columns = smatr.getColumns();
		rows = smatr.getRows();
		data = std::vector<Row>(smatr.data);
		return *this;
	}
	Matrix& operator*=(const int i){
		for(int k = 0; k < getColumns(); k++)
			data[k] *= i;
		return *this;
	}
	const bool operator==(const Matrix& smatr) const{
		if(columns != smatr.getColumns())
			return false;
		if(rows != smatr.getRows())
			return false;
		for(int k = 0; k < getColumns(); k++)
			if(data[k] != smatr[k])
				return false;
		return true;
	}
	const bool operator!=(const Matrix& smatr) const{
		if(columns != smatr.getColumns())
			return true;
		if(rows != smatr.getRows())
			return true;
		for(int k = 0; k < getColumns(); k++)
			if(data[k] != smatr[k])
				return true;
		return false;
	}
	~Matrix(){}
};
