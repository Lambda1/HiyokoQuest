#ifndef __MY_VECTOR_HPP__
#define __MY_VECTOR_HPP__

namespace my_math
{
	template <class TYPE>
		class Vec
		{
			public:
				TYPE x,y,z;
			public:
				Vec() : x(0), y(0), z(0) {}
				Vec(TYPE x,TYPE y,TYPE z) : x(x), y(y), z(z) {}
				~Vec(){}
				inline void Set(TYPE x,TYPE y){ this->x = x, this->y = y; }
				inline void Set(TYPE x,TYPE y,TYPE z){ this->x = x, this->y = y, this->z = z; }
		};
}

#endif
