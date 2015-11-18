#ifndef GEM_MATH_MATRIX3X4_H
#define GEM_MATH_MATRIX3X4_H

#include "Vec3.h"
#include "matrix3.h"

namespace Gem {

Template1 class mat4_T;

template<class typ> class mat3x4_T : public Convertable<mat3_T<typ>> {
	typedef mat3x4_T<typ> mat;
	typedef mat3_T<typ> m3;
	typedef vec3_T<typ> v3;
public:
	mat() {}


	/*mat(const v3 &p, const typ &r, const v3 &s) { setTransformation(p, r, s); }


	mat& setRotation( const typ &r ) {
		typ sr = sin(r), cr = cos(r);
		r0 = v3(cr, sr);
		r1 = v3(-sr,  cr);
		r2 = v3(0.0f, 0.0f);
		return *this;
	}	
	mat& setScale( const v3 &s ) {
		r0 = v3(s.x, 0.0f);
		r1 = v3(0.0f, s.y);
		r2 = v3(0.0f, 0.0f);
		return *this;
	}
	*/
	//Template1 mat( const T &a ) : tuple( convert<mat,T>(a) ) {}

	union {
		typ d[12];
		struct {
			m3 Rot;
			v3 Pos;
		};
		struct {
			typ _00, _01, _02;
			typ _10, _11, _12;
			typ _20, _21, _22;
			typ _30, _31, _32;
		};
		struct {
			v3 row[4];
		};
		struct {
			v3 r0;
			v3 r1;
			v3 r2;
			v3 r3;
		};
	};


	mat operator*(const mat &b ) const {  //todo - intrinsics
		mat m;
		m._00 = b._00*_00 + b._10*_01 + b._20*_02;
		m._01 = b._01*_00 + b._11*_01 + b._21*_02;
		m._02 = b._02*_00 + b._12*_01 + b._22*_02;

		m._10 = b._00*_10 + b._10*_11 + b._20*_12;
		m._11 = b._01*_10 + b._11*_11 + b._21*_12;
		m._12 = b._02*_10 + b._12*_11 + b._22*_12;

		m._20 = b._00*_20 + b._10*_21 + b._20*_22;
		m._21 = b._01*_20 + b._11*_21 + b._21*_22;
		m._22 = b._02*_20 + b._12*_21 + b._22*_22;

		m._30 = b._00*_30 + b._10*_31 + b._20*_32 + b._30;
		m._31 = b._01*_30 + b._11*_31 + b._21*_32 + b._31;
		m._32 = b._02*_30 + b._12*_31 + b._22*_32 + b._32;
		return m;
	}
	mat& operator*=(const mat &b )  { *this = *this *b; return *this; }

	mat& setTransformation( const v3 &p, const m3 &r, const v3 &s ) {
		 
		Rot = r *m3::scale(s);
		Pos = p;
		return *this;
	}

	mat& setIdentity() {
		_00 = _11 = _22 = typ(1);
		_01 = _02 = typ(0);
		_10 = _12 = typ(0);
		_20 = _21 = typ(0);
		_30 = _31 = _32 = typ(0);
		return *this;
	}
	//casting
	operator typ* () { return d; }	 
	operator const typ* () const { return d; }
	Template1A T1A as() const { return convert<T1A,T>( *(this) ); }
	template<> mat4_T<typ> as() const { return mat4_T<typ>( *this ); } 
	


	//matrix builders
	static mat identity() { return mat().setIdentity(); }
	static mat view( const v3& eye, const m3& rot ) {
		mat m;

		m._30   = -rot.row[0] .dot(eye);
		m._31   = -rot.row[1].dot(eye);
		m._32   = -rot.row[2].dot(eye);
		m.Rot = rot.getTranspose();
	//	m.Pos = -eye*rot;  ??

		return m;
	}	
	static mat camLookDir(  const v3& eye, const v3& dir, const v3& up ) {
		mat m;

		const v3& zAxis(dir);
		const v3 xAxis(up.cross(zAxis));
		const v3 yAxis(zAxis.cross(xAxis));

		m._00   = xAxis.x;
		m._01   = yAxis.x;
		m._02   = zAxis.x;

		m._10   = xAxis.y;
		m._11   = yAxis.y;
		m._12   = zAxis.y;

		m._20   = xAxis.z;
		m._21   = yAxis.z;
		m._22   = zAxis.z;

		m._30   = -xAxis.dot(eye);
		m._31   = -yAxis.dot(eye);
		m._32   = -zAxis.dot(eye);

		return m;
	}
	static mat transform( const v3 &p, const m3 &r, const v3 &s ) { mat m; m.setTransformation(p,r,s); return m; }

};

/*
template<class T> vec3_T<T> operator* ( const vec3_T<T> &a, const mat3x4_T<T> &b ) {
	vec3_T<T> v;
	v.x =  a.x*b._00 + a.y*b._10 + b._20;
	v.y =  a.x*b._01 + a.y*b._11 + b._21;
	return v;
}

template<class T> mat3x4_T<T> operator* ( const mat3x4_T<T> &a, const mat3x4_T<T> &b ) {
	mat3x4_T<T> m;
	m._00 = b._00*a._00 + b._10*a._01;
	m._01 = b._01*a._00 + b._11*a._01;

	m._10 = b._00*a._10 + b._10*a._11;
	m._11 = b._01*a._10 + b._11*a._11;

	m._20 = b._00*a._20 + b._10*a._21 + b._20;
	m._21 = b._01*a._20 + b._11*a._21 + b._21;

	return m;
}*/



typedef mat3x4_T<f32> mat3x4f;
typedef mat3x4_T<f64> mat3x4d;

}


#ifdef GEM_MATH_MATRIX4_H
#include "Matrix4.h"
#endif //GEM_MATH_MATRIX4_H

#endif //GEM_MATH_MATRIX3X4_H

/*
#if !defined(GEM_MATH_MATRIX2X3_VS_RECT_H) && defined(GEM_MATH_RECT_H)
#define GEM_MATH_MATRIX2X3_VS_RECT_H

namespace Gem {
template<class T> rect<T> operator* ( const rect<T> &a, const mat3x4_T<T> &b ) {
	rect<T> r = a;
	r.tl() *= b;
	r.br() *= b;
	return r;
}
}

#endif */
