#ifndef GEM_BASIC_MATH_H
#define GEM_BASIC_MATH_H

#include "..\Basic.h"

#include <math.h>
#undef min
#undef max

namespace Gem {

#define PI (3.14159265358979323846)
#define PIf ((f32)PI)
#define DEG_TO_RAD ( PIf / 180.0f)
#define RAD_TO_DEF ( 180.0f / PIf )
#define ZERO_EP 1e-06f 

#define HIGHf (999999999999999999999.f)  //todo
#define LOWf (-HIGHf)

Template1 T ceil0(const T &a)		{ return (a>0) ? ceil(a) : floor(a); }
Template1 T pow2(const T &a) { return a*a; }
Template1 T pow3(const T &a) { return a*a*a; }

//http://ilab.usc.edu/wiki/index.php/Fast_Square_Root
inline f32 invSqrt2(const f32 &x) { // ie 1/sqrt(x)
	u32 tmp = (unsigned(0xBE800000) - *(unsigned*)&x) >> 1;
	f32 y = *(f32*)&tmp;
	return y * (1.47f - 0.47f * x * y * y);
};
inline f32 invSqrt(f32 x) {
//	return 1.f / (sqrt(x));
	f32 xhalf = 0.5f*x;
	s32 i = *(s32*)&x; // get bits for f32ing value
	i = 0x5f3759df - (i >> 1); // gives initial guess y0
	x = *(f32*)&i; // convert bits back to f32
	x = x*(1.5f - xhalf*x*x); // Newton step, repeating increases accuracy
	return x;
}
inline f64 invSqrt(f64 x) {
	return 1.0 / sqrt(x);
}

inline f32 invSqrt3(f32 x) { //http://paste.lisp.org/display/111559
	union {
		f32 f;
		s32 i;
	} tmp;
	tmp.f = x;
	tmp.i = 0x5f3759df - (tmp.i >> 1);
	f32 y = tmp.f;
	return y * (1.5f - 0.5f * x * y * y);
}

Template1 inline const T& max(const T &i, const T &j) { return (i > j) ? i : j; }
Template1 inline const T& min(const T &i, const T &j) { return (i < j) ? i : j; }

Template1 inline void swap(T &i, T &j) { T x = i; i = j; j = x; }

}

#endif //GEM_BASIC_MATH_H