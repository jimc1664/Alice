#ifndef GEM_MATH_VEC3_H
#define GEM_MATH_VEC3_H

#include "../Basic/Math.h"
#include "../Basic/Tuple.h"

namespace Gem {

template<class typ> class vec3_T : public tuple<typ, typ, typ>, public Convertable<vec3_T<typ>> {
	typedef vec3_T<typ> v3;
public:
	v3() {}
	//Template3 v3( T &&a, T2 &&b, T3 &&c) : tuple(a,b,c) {}

	v3( const typ &a, const typ &b, const typ &c) : tuple(a,b,c) {}

	Template1 v3(const T &a  OnlyIF( Is_BasicType<T>) )			: tuple(a,a,a) {  }
	Template1 v3(const T &a	 OnlyIF2( Is_ValidConversion,T,v3) )	: tuple(convert<v3, T>(a)) {  }

	v3( const tuple<typ,typ> &t, const typ &c = typ(0.0) ) : tuple(t.x, t.y, c) {}
	v3( const tuple<typ,typ,typ,typ> &t ) : tuple(t.x, t.y,t.z) {}


	/*v3(const typ &i) : tuple(i, i) {}				CtorStraint(T,Is_BasicType)
	v3(const typ &i, const typ &j) : tuple(i, j) {}

	v3(const v3 &other) : tuple(other) {}
	Template2 v3(const tuple<T, T2> &t) : tuple((typ)t.a, (typ)t.b) {}

	typ & operator[](const u32 &i) { Assume(i<2); return (&a)[i]; }
	*/

	v3 operator-() const { return v3(-x, -y, -z); }

	v3 operator+(const v3 &i) const { return v3(x+i.x, y+i.y, z+i.z); }
	v3 operator+(const typ &i) const { return v3(x+i, y+i, z+i); }
	Template1 vec3_T& operator+= (const T &i) { *this = *this + i; return *this; }

	v3 operator-(const v3 &i) const { return v3(x-i.x, y-i.y, z-i.z); }
	v3 operator-(const typ &i) const { return v3(x-i, y-i, z-i); }
	Template1 vec3_T& operator-= (const T &i) { *this = *this - i; return *this; }

	v3 operator*(const v3 &i) const { return v3(x*i.x, y*i.y, z*i.z); }
	v3 operator*(const typ &i) const { return v3(x*i, y*i, z*i); }
	Template1 vec3_T& operator*= (const T &i) { *this = *this * i; return *this; }

	v3 operator/(const v3 &i) const { return v3(x/i.x, y/i.y, z/i.z); }
	v3 operator/(typ i) const { i = (typ)1.0 / i;  return v3(x*i, y*i, z*i); }
	Template1 vec3_T& operator/= (const T &i) { *this = *this / i; return *this; }

	/*


	Template1 vec3_T& set(const T &i) { *this = i; return *this; }

	const typ& minimum() const { return min(x, y); }
	const typ& maximum() const { return max(x, y); }
*/
	const typ sqrLeng() const { return x*x + y*y +z*z; }
	typ leng() const { return sqrt(sqrLeng()); }

	vec3_T& normalise() { *this *= invSqrt(sqrLeng()); return *this; }
	v3 getNormal() const { v3 v(*this); return v.normalise(); }

	typ dot(const v3 &j) const { return x*j.x + y*j.y + z*j.z; }
	v3 cross( const v3 &a ) const { return v3( y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x ); }
};

/*
Template1 vec3_T<T>	floor(const vec3_T<T> &i)											{ return vec3_T<T>(floor(i.x), floor(i.y)); }
Template1 vec3_T<T>	ceil(const vec3_T<T> &i)											{ return vec3_T<T>(ceil(i.x), floor(i.y)); }

Template1 vec3_T<T>	abs(const vec3_T<T> &i)												{ return vec3_T<T>(abs(i.x), abs(i.y)); }
Template1 vec3_T<T>	max(const vec3_T<T> &i, const vec3_T<T> &j)							{ return vec3_T<T>(max(i.x, j.x), max(i.y, j.y)); }
Template1 vec3_T<T>	min(const vec3_T<T> &i, const vec3_T<T> &j)							{ return vec3_T<T>(min(i.x, j.x), min(i.y, j.y)); }
Template1 vec3_T<T>	clamp(const vec3_T<T> &i, const vec3_T<T> &j, const vec3_T<T>& c)		{ return vec3_T<T>(clamp(i.x, j.x, c.x), clamp(i.y, j.y, c.y)); }
Template1 bool		ifMin(vec3_T<T> &i, const vec3_T<T> &j)								{ return ifMin(i.x, j.x) | ifMin(i.y, j.y); }
Template1 bool		ifMax(vec3_T<T> &i, const vec3_T<T> &j)								{ return ifMax(i.x, j.x) | ifMax(i.y, j.y); }

Template1 bool		isEqual(const vec3_T<T>& i, const vec3_T<T>& j, const T& epsilon)	{ return (i - j).sqrLeng() <= epsilon; }

Template1 void minMax(vec3_T<T> &min, vec3_T<T> &max)										{ minMax(min.x, max.x); minMax(min.y, max.y); }
Template1 void minMax(vec3_T<T> &min, vec3_T<T> &max, const T &c)							{ minMax(min.a, max.a, c.a); minMax(min.b, max.b, c.b); }
Template1 void minMax(vec3_T<T> &min, vec3_T<T> &max, const T &c, const T &d)				{ minMax(min.a, max.a, c.a, d.a); minMax(min.b, max.b, c.b, d.b); }
Template1 void minMax(vec3_T<T> &min, vec3_T<T> &max, const T &c, const T &d, const T &e)	{ minMax(min.a, max.a, c.a, d.a, e.a); minMax(min.b, max.b, c.b, d.b, e.b); }

*/

Template1 T dot(const vec3_T<T> &i, const vec3_T<T> &j) { return i.dot(j); }
Template1 vec3_T<T> cross(const vec3_T<T> &i, const vec3_T<T> &j) { return i.cross(j); }


typedef vec3_T<s32> vec3s;
typedef vec3_T<u32> vec3u;
typedef vec3_T<f32> vec3f;
typedef vec3_T<f64> vec3d;

typedef vec3_T<s16> vec3s16;
typedef vec3_T<u16> vec3u16;

}

#endif //GEM_MATH_VEC3_H