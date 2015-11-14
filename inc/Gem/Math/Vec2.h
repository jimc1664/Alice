#ifndef GEM_MATH_VEC2_H
#define GEM_MATH_VEC2_H

#include "../Basic/Math.h"
#include "../Basic/Tuple.h"

namespace Gem {

	template<class typ> class vec2_T : public tuple<typ, typ> {// , Convertable<vec2_T<typ>>, Convertable<tuple<typ, typ>>, Convertable<tupleD<2, typ>> {
	typedef vec2_T<typ> v2;
public:
	v2() {}
//	v2(const typ &i) : tuple(i, i) {}
	v2(const typ &i, const typ &j) : tuple(i, j) {}

	//v2(const v2 &other) : tuple(other) {}
	//v2(const tupleD<2,typ> &other) : tuple(other) {} 


	Template2 v2(const tuple<T, T2> &t) : tuple((typ)t.x, (typ)t.y) {}
	Template1 v2(const T &a OnlyIF(Is_BasicType<T>) )			: tuple((typ)a, (typ)a) { }
	//Template1 v2(const T &a	 OnlyIF2( Is_ValidConversion,T,v2) )	: tuple(convert<v2, T>(a)) {  }

	const typ* ptr() const { return &x; }
	typ & operator[](const u32 &i) { Assume(i<2); return (&x)[i]; }
	//operator const typ* () const { return &x; }

	v2 operator-() const { return v2(-x, -y); }
	v2 operator+(const tuple &i) const { return v2(x + i.x, y + i.y); }
	v2 operator-(const tuple &i) const { return v2(x - i.x, y - i.y); }
	v2 operator*(const tuple &i) const { return v2(x*i.x, y*i.y); }
	v2 operator/(const tuple &i) const { return v2(x / i.x, y / i.y); }

	v2 operator+(const typ &i ) const { return v2(x + i, y + i); }
	v2 operator-(const typ &i) const { return v2(x - i, y - i); }
	v2 operator*(const typ &i) const { return v2(x*i, y*i); }
	v2 operator/(typ i) const { return v2(x / i, y / i); }

	Template1 vec2_T& operator+= (const T &i) { *this = *this + i; return *this; }
	Template1 vec2_T& operator-= (const T &i) { *this = *this - i; return *this; }
	Template1 vec2_T& operator*= (const T &i) { *this = *this*i; return *this; }
	Template1 vec2_T& operator/= (const T &i) { *this = *this / i; return *this; }

	//for f32 v2 operator/( typ i ) const { i = typ(1)/i; return *this * i; }

	Template1 vec2_T& set(const T &i) { *this = i; return *this; }

	const typ sqrLeng() const { return x*x + y*y; }
	typ leng() const { return sqrt(sqrLeng()); }
	v2 perp() const { return v2(y, -x); }

	vec2_T& normalise() { *this *= invSqrt(sqrLeng()); return *this; }
	v2 getNormal() const { v2 v(*this); return v.normalise(); }

	typ dot(const v2 &j) const { return x*j.x + y * j.y; }

	const typ& minimum() const { return min(x, y); }
	const typ& maximum() const { return max(x, y); }

	typ area() const { return x*y; }

};

Template1 vec2_T<T>	floor(const vec2_T<T> &i)											{ return vec2_T<T>(floor(i.x), floor(i.y)); }
Template1 vec2_T<T>	ceil(const vec2_T<T> &i)											{ return vec2_T<T>(ceil(i.x), floor(i.y)); }

Template1 vec2_T<T>	abs(const vec2_T<T> &i)												{ return vec2_T<T>(abs(i.x), abs(i.y)); }
Template1 vec2_T<T>	max(const vec2_T<T> &i, const vec2_T<T> &j)							{ return vec2_T<T>(max(i.x, j.x), max(i.y, j.y)); }
Template1 vec2_T<T>	min(const vec2_T<T> &i, const vec2_T<T> &j)							{ return vec2_T<T>(min(i.x, j.x), min(i.y, j.y)); }
Template1 vec2_T<T>	clamp(const vec2_T<T> &i, const vec2_T<T> &j, const vec2_T<T>& c)		{ return vec2_T<T>(clamp(i.x, j.x, c.x), clamp(i.y, j.y, c.y)); }
Template1 bool		ifMin(vec2_T<T> &i, const vec2_T<T> &j)								{ return ifMin(i.x, j.x) | ifMin(i.y, j.y); }
Template1 bool		ifMax(vec2_T<T> &i, const vec2_T<T> &j)								{ return ifMax(i.x, j.x) | ifMax(i.y, j.y); }

Template1 bool		isEqual(const vec2_T<T>& i, const vec2_T<T>& j, const T& epsilon)	{ return (i - j).sqrLeng() <= epsilon; }

Template1 void minMax(vec2_T<T> &min, vec2_T<T> &max)										{ minMax(min.x, max.x); minMax(min.y, max.y); }
Template1 void minMax(vec2_T<T> &min, vec2_T<T> &max, const T &c)							{ minMax(min.x, max.x, c.x); minMax(min.y, max.y, c.y); }
Template1 void minMax(vec2_T<T> &min, vec2_T<T> &max, const T &c, const T &d)				{ minMax(min.x, max.x, c.x, d.x); minMax(min.y, max.y, c.y, d.y); }
Template1 void minMax(vec2_T<T> &min, vec2_T<T> &max, const T &c, const T &d, const T &e)	{ minMax(min.x, max.x, c.x, d.x, e.x); minMax(min.y, max.y, c.y, d.y, e.y); }

Template1 T wind(const vec2_T<T> &i, const vec2_T<T> &j)							{ return  j.x*i.y - j.y*i.x; }

Template1 T dot(const vec2_T<T> &i, const vec2_T<T> &j) { return i.dot(j); }

typedef vec2_T<s32> vec2s;
typedef vec2_T<u32> vec2u;
typedef vec2_T<f32> vec2f;
typedef vec2_T<f64> vec2d;

typedef vec2_T<s16> vec2s16;
typedef vec2_T<u16> vec2u16;

}

#endif //GEM_MATH_VEC2_H