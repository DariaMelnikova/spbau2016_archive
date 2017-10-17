#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

namespace apa
{
	class wrong_format : exception{};
}


class lint
{
public:
	lint() : size_(1), data_(new char [1]) {};

	lint(int init)
	{
		size_ = (init == 0) ? 1 : floor(log10(init)) + 1;
		data_ = new char[size_ + 1];
		sprintf(data_, "%d", init);
	}

	lint(const char * init, size_t size)
	{
		size_ = size;
		data_ = new char[size_];
		copy(init, init + size_, data_);
	}	

	lint(string init)
	{
		size_ = init.size() ;
		data_ = new char[size_];
		for (size_t i = 0; i<size_; ++i)
		{
			if (init[i]<'0' || init[i]>'9')
			{
				delete[] data_;
				throw apa::wrong_format();
			}
			data_[i] = init[i];
		}
	}

	lint(lint const& other)
	{
		size_ = other.size_;
		data_ = new char[size_ + 1];
		copy(other.data_, other.data_ + other.size_, data_);
	}


	~lint()
	{
		delete[] data_;
	}
	
	size_t get_size() const;

	char* get_data() const;

	lint& operator=(lint const& other);
	lint& operator+=(lint const& other);
	lint& operator-=(lint const& other);
	lint& operator*=(lint const& other);
	lint& operator/=(lint const& other);

private:
	size_t size_;
	char * data_;

};

lint operator+(lint first, lint const& second);
lint operator-(lint first, lint const& second);
lint operator*(lint first, lint const& second);
lint operator/(lint first, lint const& second);
lint operator%(lint first, lint const& second);

bool operator==(lint first, lint second);
bool operator!=(lint first, lint second);
bool operator<(lint first, lint second);
bool operator>(lint first, lint second);
bool operator<=(lint first, lint second);
bool operator>=(lint first, lint second);

ostream& operator<<(ostream& out, lint const&  obj);
istream& operator>>(istream& in, lint & obj);

lint gcd(lint & first, lint & second);

lint mult(lint const& first, int second, size_t zero_count);
