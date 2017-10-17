#include "rational.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

apa::rational& apa::rational::operator=(apa::rational const& other)
{
	this->numerator_ = lint(other.numerator_);
	this->denominator_ = lint(other.denominator_);
	return *this;
}


apa::rational& apa::rational::operator+=(apa::rational const& other)
{
	if (this->denominator_ == other.denominator_)
	{

		this->numerator_ = this->numerator_ + other.numerator_;
		this->normalized();
		return *this;
	}
	this->numerator_ = this->numerator_ * other.denominator_ + this->denominator_ * other.numerator_;
		
	this->denominator_ = other.denominator_ * this->denominator_;
	this->normalized();
	return *this;
}

apa::rational& apa::rational::operator-=(apa::rational const& other)
{
	if (this->denominator_ == other.denominator_)
	{
		this->numerator_ = this->numerator_ - other.numerator_;
		this->normalized();
		return *this;
	}
	if (*this < other)
	{
		*this = rational(0);
		return *this;
	}
	this->numerator_ = this->numerator_ * other.denominator_ - this->denominator_ * other.numerator_;
	this->denominator_ = other.denominator_ * this->denominator_;
	this->normalized();
	return *this;
}

apa::rational& apa::rational::operator*=(apa::rational const& other)
{

	this->numerator_ = this->numerator_ * other.numerator_;
	this->denominator_ = other.denominator_ * this->denominator_;
	this->normalized();
	return *this;
}

apa::rational& apa::rational::operator/=(apa::rational const& other)
{
	if (this == &other)
	{
		*this = rational(1);
		return *this;
	}
	this->numerator_ = this->numerator_ * other.denominator_;
	this->denominator_ = other.numerator_ * this->denominator_;
	this->normalized();
	return *this;
}

string apa::rational::str() const
{
	stringstream  ss;
	lint num = this->numerator_;
	lint denum = this->denominator_;
	ss << num << " / " << denum;
	return ss.str();
}

void apa::rational::normalized()
{
	lint divide = gcd(this->numerator_, this->denominator_);
	this->numerator_ = this->numerator_ / divide;
	this->denominator_ = this->denominator_ / divide;
}

namespace apa{

	apa::rational operator+(apa::rational first, apa::rational const& other)
	{
		return first += other;
	}

	apa::rational operator-(apa::rational first, apa::rational const& other)
	{
		return first -= other;
	}

	apa::rational operator*(apa::rational first, apa::rational const& second)
	{
		return first *= second;
	}

	apa::rational operator/(apa::rational first, apa::rational const& second)
	{
		return first /= second;
	}

	bool operator<(apa::rational first, apa::rational second) 
	{
		return (first.get_num() * second.get_denom()) < (first.get_denom() * second.get_num());
	}

	bool operator==(apa::rational first, apa::rational second)
	{
		return (first.get_num() * second.get_denom()) == (first.get_denom() * second.get_num());
	}

	bool operator!=(apa::rational first, apa::rational second)
	{
		return !(first == second);
	}

	bool operator>(apa::rational first, apa::rational second)
	{
		return second < first;	} 

	bool operator<=(apa::rational first, apa::rational second)
	{
		return (first < second) || (first == second);
	}

	bool operator>=(apa::rational first, apa::rational second)
	{
		return (first > second) || (first == second);	}

	ostream& operator<<(ostream& out, apa::rational const& obj) 
	{
		string show = obj.str();
	    out << show; 
	    return out;
	}

	istream& operator>>(istream& in, apa::rational& obj) 
	{
		lint numerator, denominator;
		string input;
		getline(in, input);
		size_t x = 0;
		char * n_data = new char [input.size()];
		while (input[x] <= '9' && input[x] >= '0' && x < input.size())
		{
			n_data[x] = input[x];
			x++;
		}	
		if (x == 0)
		{
			delete[] n_data;
			throw apa::wrong_format();
		}
		size_t y = x;
		if (input[y] != ' ')
		{
			delete[] n_data;
			throw apa::wrong_format();
		}
		y++;
		if (input[y] != '/')
		{
			delete[] n_data;
			throw apa::wrong_format();
		}
		y++;
		if (input[y] != ' ')
		{
			delete[] n_data;
			throw apa::wrong_format();
		}
		char * d_data = new char[input.size()];
		y++;
		while (input[y] <= '9' && input[y] >= '0' && y < input.size())
		{
			d_data[y - x - 3] = input[y];
			y++;
		}	
		if (y - x - 3 <= 0)
		{
			delete[] n_data;
			delete[] d_data;
			throw apa::wrong_format();
		}
		numerator = lint(n_data, x);
		denominator = lint(d_data, y-x-3);
		obj = rational(numerator, denominator);
		obj.normalized();
		delete[] n_data;
		delete[] d_data;
		return in;
	} 
}


