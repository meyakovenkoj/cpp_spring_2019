#pragma once

using digit = signed char;

class BigInt
{
	bool negative;
	size_t size;
	size_t digits_amount = 10;
	digit* elements = nullptr;
	
	void push_back(digit new_element);
	void allocate();
	void delete_zeros();
	bool firstAbsLess( const BigInt& otherNumber) const;
	
public:
	BigInt();
	BigInt(int64_t new_element);
	BigInt(const BigInt& other);
	~BigInt();
	
	BigInt absolute() const;
	BigInt operator+(const BigInt& other) const;
	BigInt operator-() const;
	BigInt operator-(const BigInt& other) const;
	bool operator==(const BigInt& other) const;
	bool operator!=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;
	BigInt& operator=(const BigInt& other);
	friend std::ostream& operator << (std::ostream& os, const BigInt& num);
};

BigInt::BigInt(int64_t new_element) : size(0)
{
	if(new_element < 0){
		negative = true;
		new_element *= -1;
	}
	else
		negative = false;
	
	elements = new digit[digits_amount];
	
	if(!new_element)
		push_back(0);
	else
		for(size_t i = 0; new_element>0; ++i){
			push_back(new_element % 10);
			new_element /= 10;
		}
}

BigInt::BigInt(const BigInt& other) : negative(other.negative), size(other.size), digits_amount(other.digits_amount)
{
	elements = new digit[digits_amount];
	for(size_t i = 0; i<size; ++i)
		elements[i] = other.elements[i];
}

BigInt::~BigInt()
{
	delete[] elements;
}

void BigInt::push_back(digit new_element)
{
	elements[size++] = new_element;
	if(size >= digits_amount)
		allocate();
}

void BigInt::allocate()
{
	digits_amount += 5;
	digit* new_elements = new digit[digits_amount];
	for(size_t i = 0; i < size; ++i)
		new_elements[i] = elements[i];
	delete[] elements;
	elements = new_elements;
}

void BigInt::delete_zeros()
{
	for (size_t i = size - 1; i != 0; --i)
	{
		if (elements[i] != 0 )
			break;
		size--;
	}
}

BigInt::BigInt() : negative(false), size(0)
{
	elements = new digit[digits_amount];
	push_back(0);
}

BigInt BigInt::absolute() const
{
	if(negative)
		return (-*this);
	else
		return *this;
}

BigInt BigInt::operator+(const BigInt& other) const
{
	BigInt res;
	res.size = 0;
	size_t max = size > other.size ? size : other.size;
	if(negative == other.negative){
		digit buf = 0;
		for(size_t i = 0; i < max; i++){
			if (i < size)
				buf += elements[i];
			if (i < other.size)
				buf += other.elements[i];
			res.push_back(buf);
			buf = 0;
		}
		res.negative = negative;
		digit tmp = 0;
		
		
		for(size_t i = 0; i < res.size; i++){
			res.elements[i] += tmp;
			tmp = 0;
			if(res.elements[i] >= 10){
				tmp = res.elements[i] / 10;
				res.elements[i] = res.elements[i] % 10;
			}
		}
		if(tmp)
			res.push_back(tmp);
	}
	else{
		
		digit this_sign;
		digit other_sign;
		
		if(this->absolute() < other.absolute()){
			this_sign = -1;
			other_sign = 1;
			res.negative = other.negative;
		}
		else
		{
			this_sign = 1;
			other_sign = -1;
			res.negative = negative;
			if (this->absolute() == other.absolute()) {
				res.negative = false;
			}
		}
		
		digit buf = 0;
		for(size_t i = 0; i < max; i++){
			if(i < size)
				buf += this_sign * elements[i];
			if(i < other.size)
				buf += other_sign * other.elements[i];
			res.push_back(buf);
			buf = 0;
		}
		
		
		for(size_t i = 0; i < res.size - 1; i++)
		{
			if(res.elements[i] < 0){
				res.elements[i+1] -= 1;
				res.elements[i] += 10;
			}
		}
		
		if(res.elements[res.size - 1] < 0)
		{
			res.elements[res.size - 1] *= -1;
		}
		res.delete_zeros();
	}
	
	if(!res.size)
		res.size++;
	
	return res;
}

BigInt BigInt::operator-() const
{
	BigInt res(*this);
	if(size == 1 && !elements[0])
		res.negative = negative;
	else
		res.negative = !negative;
	return res;
}

BigInt BigInt::operator-(const BigInt& other) const
{
	return (*this + (-other));
}

bool BigInt::operator==(const BigInt& other) const
{
	if(size != other.size || negative != other.negative)
		return false;
	else
		for(size_t i = 0; i < size; ++i)
			if(elements[i] != other.elements[i])
				return false;
	
	return true;
}

bool BigInt::operator!=(const BigInt& other) const
{
	return !(*this == other);
}

BigInt& BigInt::operator=(const BigInt& other)
{
	negative = other.negative;
	size = other.size;
	digits_amount = other.digits_amount;
	
	delete[] elements;
	elements = new digit[digits_amount];
	for(size_t i = 0; i < size; ++i)
		elements[i] = other.elements[i];
	
	return *this;
}

std::ostream& operator << (std::ostream& os, const BigInt& num)
{
	if(num.negative)
		os << '-';
	for(int i = num.size - 1; i>-1; --i)
		os << static_cast<digit>('0' + num.elements[i]);
	return os;
}

bool BigInt::firstAbsLess(const  BigInt& otherNumber) const
{
	if (size < otherNumber.size)
		return true;
	else if (size > otherNumber.size)
		return false;
	
	for (int i = size - 1; i > -1; i-- )
		if (elements[i] != otherNumber.elements[i])
			return (elements[i] < otherNumber.elements[i]);
	return false;
}

bool BigInt::operator<(const BigInt& other) const
{
	if (!negative && other.negative)
		return false;
	else if (negative && !other.negative)
		return true;
	else if (*this == other)
		return false;
	return negative ? !firstAbsLess(other) : firstAbsLess(other);
}

bool BigInt::operator>(const BigInt& other) const
{
	if (!negative && other.negative)
		return true;
	else if (negative && !other.negative)
		return false;
	else if (*this == other)
		return false;
	return negative ? firstAbsLess(other) : !firstAbsLess(other);
}


bool BigInt::operator<=(const BigInt& other) const
{
	return !(*this > other);
}

bool BigInt::operator>=(const BigInt& other) const
{
	return !(*this < other);
}
