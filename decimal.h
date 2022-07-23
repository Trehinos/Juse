#pragma once

#include "types.h"
#include <math.h>

namespace Juse
{

	class decimal /* = base * 10^scale */
	{
		i64 base;
		i8 exponent;

	public:
		decimal(i64 = 0, i8 = 0);
		decimal(double);
		inline operator double() { return double(base) * pow(10, exponent); }
		inline decimal operator+(decimal b) { return decimal(double(*this) + double(b)); }
		inline decimal operator+(int b) { return decimal(double(*this) + double(b)); }
		inline decimal operator+(double b) { return decimal(double(*this) + b); }
		decimal operator-(decimal);
		decimal operator-(int);
		decimal operator-(double);
		decimal operator*(decimal);
		decimal operator*(int);
		decimal operator*(double);
		decimal operator/(decimal);
		decimal operator/(int);
		decimal operator/(double);
	};

	class Machine
	{



	};
}
