#ifndef __MY_MATH_HPP__
#define __MY_MATH_HPP__

#include <cmath>
#include <random>

namespace my_math
{
	class Math
	{
		public:
			constexpr static double PI = 3.14159;

			Math() = delete;
			~Math();

			template <class T>
			inline static T Distance(const T x1,const T y1,const T x2,const T y2){ return std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)); }
			template <class T>
			inline static double Angle(const T x1,const T y1,const T x2,const T y2){ return std::atan2(y2-y1,x2-x1); }
	};
};

#endif
