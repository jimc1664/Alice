#ifndef GEM_MATH_MATRIX2X3_H
#define GEM_MATH_MATRIX2X3_H

#include "Vec2.h"

namespace Gem {

template<class typ> class mat2x3_T {//, public Convertable<mat3_T<typ>> {
	typedef mat2x3_T<typ> mat;
	typedef vec2_T<typ> v2;
public:
	mat() {}
	mat(const v2 &p, const typ &r, const v2 &s) { setTransformation(p, r, s); }


	mat& setRotation( const typ &r ) {
		typ sr = sin(r), cr = cos(r);
		r0 = v2(cr, sr);
		r1 = v2(-sr,  cr);
		r2 = v2(0.0f, 0.0f);
		return *this;
	}	
	mat& setScale( const v2 &s ) {
		r0 = v2(s.x, 0.0f);
		r1 = v2(0.0f, s.y);
		r2 = v2(0.0f, 0.0f);
		return *this;
	}
	mat& setTransformation( const v2 &p, const typ &r, const v2 &s ) {
		 
		typ sr = sin(r), cr = cos(r);

		r0 = v2(cr, sr)*s.x;
		r1 = v2(-sr,  cr)*s.y;
		r2 = p;

		return *this;
	}
	//Template1 mat( const T &a ) : tuple( convert<mat,T>(a) ) {}

	union {
		typ d[6];
		//typ data[3][2];

		struct {
			typ _00, _01;
			typ _10, _11;
			typ _20, _21;
		};
		struct {
			vec2_T<typ> row[3];
		};
		struct {
			vec2_T<typ> r0;
			vec2_T<typ> r1;
			vec2_T<typ> r2;
		};
	};
};

template<class T> vec2_T<T> operator* ( const vec2_T<T> &a, const mat2x3_T<T> &b ) {
	vec2_T<T> v;
	v.x =  a.x*b._00 + a.y*b._10 + b._20;
	v.y =  a.x*b._01 + a.y*b._11 + b._21;
	return v;
}

template<class T> mat2x3_T<T> operator* ( const mat2x3_T<T> &a, const mat2x3_T<T> &b ) {
	mat2x3_T<T> m;
	m._00 = b._00*a._00 + b._10*a._01;
	m._01 = b._01*a._00 + b._11*a._01;

	m._10 = b._00*a._10 + b._10*a._11;
	m._11 = b._01*a._10 + b._11*a._11;

	m._20 = b._00*a._20 + b._10*a._21 + b._20;
	m._21 = b._01*a._20 + b._11*a._21 + b._21;

	return m;
}

typedef mat2x3_T<f32> mat2x3f;
typedef mat2x3_T<f64> mat2x3d;

}

#endif //GEM_MATH_MATRIX2X3_H

#if !defined(GEM_MATH_MATRIX2X3_VS_RECT_H) && defined(GEM_MATH_RECT_H)
#define GEM_MATH_MATRIX2X3_VS_RECT_H

namespace Gem {
template<class T> rect<T> operator* ( const rect<T> &a, const mat2x3_T<T> &b ) {
	rect<T> r = a;
	r.tl() *= b;
	r.br() *= b;
	return r;
}
}

#endif 
