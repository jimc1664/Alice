#ifndef GEM_MATH_RECT_H
#define GEM_MATH_RECT_H

#include "../Basic/Math.h"
#include "vec2.h"

namespace Gem {

template<class typ> class rect : public tuple<typ, typ, typ, typ> {
protected:
	typedef vec2_T<typ> v2;
public:
	rect() {}
	rect(const typ &i) : tuple(i, i, i, i) {}
	rect(const typ &i, const typ &j, const typ &k, const typ &l) : tuple(i, j, k, l) {}

	rect(const rect &other) : tuple(other) {}
	Template4 rect(const tuple<T, T2, T3, T4> &t) : tuple((typ)t.x, (typ)t.y, (typ)t.z, (typ)t.w) {}

	typ & operator[](const u32 &i) { Assume(i<4); return (&x)[i]; }

	rect operator-() const { return rect(-x, -y, -z, -w); }
	rect operator+(const rect &i) const { return rect(x+i.x, y+i.y, z+i.z, w+i.w); }
	Template1 rect operator+(const T &i) const { return rect(x+i, y+i, z+i, w+i); }
	Template1 rect& operator+= (const T &i) { *this = *this + i; return *this; }

	rect operator-(const rect &i) const { return rect(x-i.x, y-i.y, z-i.z, w-i.w); }
	Template1 rect operator-(const T &i) const { return rect(x-i, y-i, z-i, w-i); }
	Template1 rect& operator-= (const T &i) { *this = *this - i; return *this; }

	rect operator*(const rect &i) const { return rect(x*i.x, y*i.y, z*i.z, w*i.w); }
	Template1 rect operator*(const T &i) const { return rect(x*i, y*i, z*i, w*i); }
	Template1 rect& operator*= (const T &i) { *this = *this * i; return *this; }

	rect operator/(const rect &i) const { return rect(x/i.x, y/i.y, z/i.z, w/i.w); }
	Template1 rect operator/(const T &i) const { return rect(x/i, y/i, z/i, w/i); }
	Template1 rect& operator/= (const T &i) { *this = *this / i; return *this; }

	Template1 rect& set(const T &i) { *this = i; return *this; }

	bool pointCheck( const v2 &v ) const {
		
		return v.x > tl().x && v.y > tl().y && v.x < br().x && v.y < br().y;
	}
	v2& tl() { return *(v2*)&x; }
	v2& br() { return *(v2*)&z; }
	const v2& tl() const { return *(v2*)&x; }
	const v2& br() const { return *(v2*)&z; }
};

typedef rect<s32> rects;
typedef rect<u32> rectu;
typedef rect<f32> rectf;
typedef rect<f64> rectd;

typedef rect<s16> rects16;
typedef rect<u16> rectu16;

//	Template1 rect operator*(const T &i) const { return rect(x*i, y*i, z*i, w*i); }

//template<class T> rect<T> operator* ( const rect<T> &a, const T &b ) { return rect<T>(x*b, y*b, z*b, w*b); }

}



#endif //GEM_MATH_RECT_H


#if !defined(GEM_MATH_MATRIX2X3_VS_RECT_H) && defined(GEM_MATH_MATRIX2X3_H)
#include "Matrix2x3.h"
#endif 
