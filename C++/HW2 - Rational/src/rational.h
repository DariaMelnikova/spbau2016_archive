#pragma once
#include "lint.h"
#include <stdint.h>
#include <string>
#include <iostream>
namespace apa{
	class rational{
	public:
		rational()
		{
			numerator_ = lint(0);
			denominator_ = lint(1);
		}

		explicit rational(int numerator, int denominator = 1) 
		{
		    if (denominator == 0)
			{
		        throw std::overflow_error("Divide by zero exception");
			} else {
				numerator_ = lint(numerator);
				denominator_ = lint(denominator);
				this->normalized();
			}
		}

		explicit rational(string numerator, string denominator = "1") 
		{
		    if (denominator == "0")
			{
		        throw std::overflow_error("Divide by zero exception");
			} else {
				numerator_ = lint(numerator);
				denominator_ = lint(denominator);
				this->normalized();
			}
		}
		explicit rational(lint numerator, lint denominator = lint(1)) 
		{
		    if (denominator == lint(0))
			{
		        throw std::overflow_error("Divide by zero exception");
			} else {
				numerator_ = numerator;
				denominator_ = denominator;
				this->normalized();
			}
		}

		rational(rational const& other) :
				numerator_ (other.numerator_), denominator_(other.denominator_) {}

		~rational(){}
		rational& operator=(rational const& other);
		rational& operator-=(rational const& other);
		rational& operator+=(rational const& other);
		rational& operator*=(rational const& other);
		rational& operator/=(rational const& other);

		explicit operator bool() const
		{
			return numerator_ != lint(0);
		}

		string str() const;

		lint get_num() 
		{
			return numerator_;
		}

		lint get_denom() 
		{
			return denominator_;
		}
	
		void normalized();

	private:
		lint numerator_;
		lint denominator_;	
	
	};
	


	rational operator+(apa::rational first, apa::rational const& other);
	rational operator-(apa::rational first, apa::rational const& other);
	rational operator*(apa::rational first, apa::rational const& other);
	rational operator/(apa::rational first, apa::rational const& other);

	bool operator<(rational first, rational second);
	bool operator==(rational first, rational second);
	bool operator!=(rational first, rational second);
	bool operator>(rational first, rational second);
	bool operator<=(rational first, rational second);
	bool operator>=(rational first, rational second);

	std::ostream& operator<<(std::ostream& out, rational const& obj);
	std::istream& operator>>(std::istream& out, rational & obj);
}
