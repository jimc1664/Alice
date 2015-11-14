#ifndef GEM_MATH_VEC4_H
#define GEM_MATH_VEC4_H

#include "../Basic/Math.h"
#include "../Basic/Tuple.h"

namespace Gem {

template<class typ> class vec4_T : public tuple<typ, typ, typ, typ>, public Convertable<vec4_T<typ>> {
	typedef vec4_T<typ> v4;
public:
	v4() {}
	Template4 v4( T &&a, T2 &&b, T3 &&c, T4 &&d) : tuple(a,b,c, d) {}



	Template1 v4(const T &a  OnlyIF( Is_BasicType<T>) )			: tuple(a,a,a,a) {  }
	Template1 v4(const T &a	 OnlyIF2( Is_ValidConversion,T,v4) )	: tuple(convert<v4, T>(a)) {  }
	v4( const tuple<typ,typ,typ> &t, const typ &d = typ(0.0) ) : tuple(t.x, t.y, t.z, d) {}

	/*v4(const typ &i) : tuple(i, i) {}				CtorStraint(T,Is_BasicType)
	v4(const typ &i, const typ &j) : tuple(i, j) {}

	v4(const v4 &other) : tuple(other) {}
	Template2 v4(const tuple<T, T2> &t) : tuple((typ)t.a, (typ)t.b) {}

	typ & operator[](const u32 &i) { Assume(i<2); return (&a)[i]; }
	*/

	v4 operator-() const { return v4(-x, -y, -z, -w); }
	/*
	v4 operator+(const v4 &i) const { return v4(x+i.x, y+i.y, z+i.z); }
	v4 operator+(const typ &i) const { return v4(x+i, y+i, z+i); }
	Template1 vec4_T& operator+= (const T &i) { *this = *this + i; return *this; }

	v4 operator-(const v4 &i) const { return v4(x-i.x, y-i.y, z-i.z); }
	v4 operator-(const typ &i) const { return v4(x-i, y-i, z-i); }
	Template1 vec4_T& operator-= (const T &i) { *this = *this - i; return *this; }

	v4 operator*(const v4 &i) const { return v4(x*i.x, y*i.y, z*i.z); }
	v4 operator*(const typ &i) const { return v4(x*i, y*i, z*i); }
	Template1 vec4_T& operator*= (const T &i) { *this = *this * i; return *this; }

	v4 operator/(const v4 &i) const { return v4(x/i.x, y/i.y, z/i.z); }
	v4 operator/(typ i) const { i = (typ)1.0 / i;  return v4(x*i, y*i, z*i); }
	Template1 vec4_T& operator/= (const T &i) { *this = *this / i; return *this; }

	/*


	Template1 vec4_T& set(const T &i) { *this = i; return *this; }

	const typ& minimum() const { return min(x, y); }
	const typ& maximum() const { return max(x, y); }
* /
	const typ sqrLeng() const { return x*x + y*y +z*z; }
	typ leng() const { return sqrt(sqrLeng()); }

	vec4_T& normalise() { *this *= invSqrt(sqrLeng()); return *this; }
	v4 getNormal() const { v4 v(*this); return v.normalise(); }

	typ dot(const v4 &j) const { return x*j.x + y*j.y + z*j.z; } */
};

/*
Template1 vec4_T<T>	floor(const vec4_T<T> &i)											{ return vec4_T<T>(floor(i.x), floor(i.y)); }
Template1 vec4_T<T>	ceil(const vec4_T<T> &i)											{ return vec4_T<T>(ceil(i.x), floor(i.y)); }

Template1 vec4_T<T>	abs(const vec4_T<T> &i)												{ return vec4_T<T>(abs(i.x), abs(i.y)); }
Template1 vec4_T<T>	max(const vec4_T<T> &i, const vec4_T<T> &j)							{ return vec4_T<T>(max(i.x, j.x), max(i.y, j.y)); }
Template1 vec4_T<T>	min(const vec4_T<T> &i, const vec4_T<T> &j)							{ return vec4_T<T>(min(i.x, j.x), min(i.y, j.y)); }
Template1 vec4_T<T>	clamp(const vec4_T<T> &i, const vec4_T<T> &j, const vec4_T<T>& c)		{ return vec4_T<T>(clamp(i.x, j.x, c.x), clamp(i.y, j.y, c.y)); }
Template1 bool		ifMin(vec4_T<T> &i, const vec4_T<T> &j)								{ return ifMin(i.x, j.x) | ifMin(i.y, j.y); }
Template1 bool		ifMax(vec4_T<T> &i, const vec4_T<T> &j)								{ return ifMax(i.x, j.x) | ifMax(i.y, j.y); }

Template1 bool		isEqual(const vec4_T<T>& i, const vec4_T<T>& j, const T& epsilon)	{ return (i - j).sqrLeng() <= epsilon; }

Template1 void minMax(vec4_T<T> &min, vec4_T<T> &max)										{ minMax(min.x, max.x); minMax(min.y, max.y); }
Template1 void minMax(vec4_T<T> &min, vec4_T<T> &max, const T &c)							{ minMax(min.a, max.a, c.a); minMax(min.b, max.b, c.b); }
Template1 void minMax(vec4_T<T> &min, vec4_T<T> &max, const T &c, const T &d)				{ minMax(min.a, max.a, c.a, d.a); minMax(min.b, max.b, c.b, d.b); }
Template1 void minMax(vec4_T<T> &min, vec4_T<T> &max, const T &c, const T &d, const T &e)	{ minMax(min.a, max.a, c.a, d.a, e.a); minMax(min.b, max.b, c.b, d.b, e.b); }

*/

//Template1 T dot(const vec4_T<T> &i, const vec4_T<T> &j) { return i.dot(j); }


typedef vec4_T<s32> vec4s;
typedef vec4_T<u32> vec4u;
typedef vec4_T<f32> vec4f;
typedef vec4_T<f64> vec4d;

typedef vec4_T<s16> vec4s16;
typedef vec4_T<u16> vec4u16;

}

#endif //GEM_MATH_VEC4_H