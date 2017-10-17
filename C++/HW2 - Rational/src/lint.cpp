using namespace std;
#include "lint.h"
#include <string>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

size_t lint::get_size() const
{
	return size_;
}

char* lint::get_data() const
{
	return data_;
}

lint& lint::operator=(lint const& other)
{
	if (other.data_ != this->data_)
	{
		delete[] data_;
		size_ = other.size_;
      	data_ = new char[size_];
		copy(other.data_, other.data_ + other.size_, data_);  
	}

	return *this;
}


int to_int(char a)
{
	return (int)a - (int)'0';
}

lint& lint::operator+=(lint const& other)
{
	if (*this == lint(0))
	{
		*this = other;
		return *this;
	}
	if (other == lint(0))
		return *this;
	char * first_data = this->get_data();
	char * second_data = other.data_;
	int first_size = this->get_size();
	int second_size = other.size_;
	if (first_size < second_size)
	{
		swap(first_data, second_data);
		swap(first_size, second_size);
	}

	int max_length = first_size;
	char * result = new char[max_length + 2];
	result[0] = -1;
	int pass = 0;
	int i = first_size - 1;
	int j = second_size - 1;
	int z = max_length;
	while ((i >= 0 || j >=0) && z >=0)
	{
		if (j>=0)
		{	
			pass = pass + to_int(first_data[i]) + to_int(second_data[j]);
			j--;
		}
		else
		{
		pass = pass + to_int(first_data[i]);
		}
		result[z] = (to_string(pass % 10))[0] ;
		z--;
		i--;
		pass /= 10;	
	}
	if (pass != 0)
		result[0] = (to_string(pass))[0];
	delete[] data_;
	if (result[0] == -1)
	{
		data_ = new char[max_length];
		size_ = max_length;
		copy(result + 1, result + max_length + 1, data_);
	}
	else
	{
		data_ = new char[max_length + 1];
		size_ = max_length + 1;

		copy(result, result + max_length + 1, data_);
	}

	delete[] result;
	return *this;
}

lint& lint::operator-=(lint const& other)
{
	char * first_data = this->get_data();
	char * second_data = other.data_;

	int first_size = this->get_size();
	int second_size = other.size_;

	int max_length = first_size;
	char * result = new char[max_length + 1];
	for (size_t i = 0; i < (size_t)max_length; ++i )
		result[i] = -1;
	int pass = 0;
	int i = first_size - 1;
	int j = second_size - 1;
	int z = max_length - 1;
	while ((i >= 0 || j >=0) && z >=0)
	{
		if (j>=0)
		{	
			if (first_data[i]>=second_data[j] + pass)
			{
				result[z] = '0' + to_int(first_data[i]) - to_int(second_data[j]) - pass; 
				pass = 0;
			}
			else
			{
				result[z] = '0' + to_int(first_data[i]) - to_int(second_data[j]) + 10 - pass;
				pass = 1;
			}
			j--;
		}
		else
		{
			if (first_data[i] >= '0' + pass)
			{
				result[z] = first_data[i] - pass;
				pass = 0;
			}
			else
			{
				result[z] = first_data[i] - pass + 10;
				pass = 1;
			}
		}
		z--;
		i--;
	}
	z++;
	while (z < max_length - 1 && result[z] == '0')
	{
		z++;
	}
	delete[] data_;
	size_ = max_length - z;
	data_ = new char[size_];
	copy(result + z, result + max_length, data_);
	delete[] result;
	return *this;
}

lint mult(lint const& first, int second, size_t zero_count)
{
	if (second == 0)
		return lint(0);
	size_t max_length = first.get_size() + 1 + zero_count;
	char * multiple  = new char[max_length  +  1];
	char * data = first.get_data();
	int pass = 0;
	for (int i = first.get_size() - 1; i >=0 ; --i)
	{
		pass = pass + to_int(data[i]) * second;
		multiple[i + 1] = (to_string(pass % 10))[0];
		pass /= 10;
	} 
	for (size_t i = 0; i < zero_count; i++)
	{
		multiple[first.get_size() + 1 + i] = '0';
	}
	lint ret;
	if (pass == 0)
		ret = lint(multiple+1, max_length - 1);
	else
	{
		multiple[0] = (to_string(pass))[0];
		ret = lint(multiple, max_length);
	}
	delete[] multiple;
	return ret;
}


lint& lint::operator*=(lint const& other)
{
	lint first = *this;
	lint second = other;
	size_t first_size = this->get_size();
	size_t second_size = other.size_;
	if (*this == lint(0))
		return *this;
	if (other == lint(0))
	{
		this->size_ = 1;
		this->data_[0] = '0';
		return *this;
	}
	if (first_size < second_size)
	{
		swap(first, second);
		swap(first_size, second_size);
	}
	lint prev = mult(first, to_int(second.get_data()[second_size-1]), 0);
	for (size_t i = 1; i < second_size; ++i)
	{
		lint next = mult(first, to_int(second.get_data()[second_size - 1 - i]), i);		
		prev = prev + next;
	}
	delete[] this->data_;
	this->size_ = prev.size_;
	this->data_ = new char[size_];
	copy(prev.data_,prev.data_ + prev.size_, data_);
	return *this;
}



lint operator+(lint first, lint const& second)
{
	return first += second;
}

lint operator-(lint first, lint const& second)
{
	return first -= second;
}
lint operator*(lint first, lint const& second)
{
	return first *= second;
}

char div(lint & first, lint const& second)
{
	int i = 8;
	if (first < second)
		return '0';
	lint max = mult(second, 9, 0);
	if (first >= max)
		return '9';
	for (; i >= 1; --i)
	{
		lint next = mult(second, i, 0);
		if (max > first &&  next <= first)
			return (to_string(i))[0];
		max = next;
	}
	return  '9';
}

lint append(lint num, char symb)
{
	if (num == lint(0))
	{
		return lint(&symb,1);
	}	
	size_t size = num.get_size() + 1;
	char * data = new char[size];	
	char * prev_data = num.get_data();
	copy(prev_data, prev_data + size - 1, data);
	data[size - 1] = symb;
	num = lint(data, size);
	delete[] data;
	return num;
}

lint operator/(lint first, lint const& second)
{
	if (second == lint(0))
		throw std::overflow_error("Divide by zero exception");
	if (second == lint(1))
		return first;
	if (first < second)
		return lint(0);
	lint result;
	char * resid = new char[first.get_size() + 1 ];
	char * partial = new char[first.get_size() + 1];
	size_t y = second.get_size(), z = 0;
	if (lint(first.get_data(), y) < second)
		y++;
	copy(first.get_data(), first.get_data() + y, resid);
	lint part_div = lint(resid, y);		
	while(y <= first.get_size())
	{
		partial[z] = div(part_div, second);
		part_div = part_div - mult(second, to_int(partial[z]), 0);
		z++;
		if (y < first.get_size())
			part_div = append(part_div, first.get_data()[y]);
		y++;
	}
	partial[z] = div(part_div, second);

	result =  lint(partial, z);

	delete[] resid;
	delete[] partial;
	return result;	
}

lint operator%(lint first, lint const& second)
{
	if (first < second)
		return first;
	if (first == second)
		return lint(0);
	lint result = first / second;
	result *= second;
	result = first - result;
	return result;
}

bool operator==(lint first, lint second)
{
	size_t first_size = first.get_size();
	size_t second_size = second.get_size();
	if (first_size != second_size)
		return false;
	for (size_t i = 0; i < first_size; ++i)
		if (first.get_data()[i] != second.get_data()[i])
			return false;
	return true;
}

bool operator!=(lint first, lint second)
{
	return !(first == second);
}

bool operator<(lint first, lint second)
{
	size_t first_size = first.get_size();
	size_t second_size = second.get_size();
	if (first_size > second_size)
		return false;
	if (first_size < second_size)
		return true;
	for (size_t i = 0; i < first_size; ++i)
	{
		if (first.get_data()[i] < second.get_data()[i])
			return true;
		if (first.get_data()[i] > second.get_data()[i])
			return false;
	
	}
	return false;
	
}

bool operator>(lint first, lint second)
{
	return second < first;
}

bool operator<=(lint first, lint second)
{
	return (first == second) || (first < second);
}

bool operator>=(lint first, lint second)
{
	return (first == second) || (first > second);
}


ostream& operator<<(ostream& out, lint const& obj)
{
	char * data = obj.get_data();
	for (size_t i = 0; i < obj.get_size(); ++i)
		out << data[i];
	return out;
}

istream& operator>>(istream& in, lint & obj) 
{
	string inp;
	cin >> inp;
	obj = lint(inp);
	return in;
}

lint gcd(lint & first, lint & second)
{		
	lint a = first;
	lint b = second;
    lint next;

    while (a != lint(0)) 
	{
        next = a; 
		a = b % a;  
		b = next;

    }
    return b;
}


