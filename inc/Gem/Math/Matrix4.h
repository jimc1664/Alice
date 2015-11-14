#ifndef GEM_MATH_MATRIX4_H
#define GEM_MATH_MATRIX4_H

#include "Vec3.h"
#include "Vec4.h"

namespace Gem {

Template1 class mat3x4_T;

template<class typ> class mat4_T { // : public Convertable<mat4_T<typ>> {
	typedef mat4_T<typ> m4;
	typedef vec4_T<typ> v4;
	typedef vec3_T<typ> v3;
public:
	m4() {}
	m4(Identity) { setIdentity();  }
	//Template1 m4( const T &a ) : tuple( convert<m4,T>(a) ) {}

	m4(const mat3x4_T<typ> &m);
	union {
		typ d[16];
		//typ data[3][2];

		struct {
			typ _00, _01, _02, _03;
			typ _10, _11, _12, _13;
			typ _20, _21, _22, _23;
			typ _30, _31, _32, _33;
		};
		struct {
			v4 row[4];
		};
		struct {
			v4 r0;
			v4 r1;
			v4 r2;
			v4 r3;
		};
	};

	operator typ* () { return d; }	 
	operator const typ* () const { return d; }
	typ* fPtr() const { return const_cast<typ*>(d); } //hack for shaders

	v3& translation() { return *(v3*)&_30; }

	m4 getTranspose() const {
		m4 ret; 
		ret._00 = _00; ret._11 = _11; ret._22 = _22; ret._33 = _33;
		ret._01 = _10; ret._02 = _20; ret._03 = _30;
		ret._10 = _01; ret._12 = _21; ret._13 = _31;
		ret._20 = _02; ret._21 = _12; ret._23 = _32;
		ret._30 = _03; ret._31 = _13; ret._32 = _23;
		return ret;
	}
	m4& setIdentity() { 
		_00 = _11 = _22 = _33 = typ(1);
		_01 = _02 = _03 = typ(0);
		_10 = _12 = _13 = typ(0);
		_20 = _21 = _23 = typ(0);
		_30 = _31 = _32 = typ(0);
		return *this;
	}
	//matrix builders
	static mat4_T identity() { return mat4_T(Identity()); }

	static m4 projection( const typ &fov, const typ & aspect, const typ &clipNear, const typ &clipFar ) {
			m4 m;

	/*	double D2R = M_PI / 180.0;
		double yScale = 1.0 / tan(D2R * fov / 2);
		double xScale = yScale / aspect;
		double nearmfar = near - far;
		double m[] = {
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, (far + near) / nearmfar, -1, 
			 0, 0, 2*far*near / nearmfar, 0 
	*/

			typ height = typ(1.0)/tan(fov/typ(2.0));
			m._01 = m._02 = m._03 = m._10 = m._12 = m._13 = m._20 = m._21 = m._30 = m._31 = m._33 = typ(0);
			m._00 = (height / aspect);
			m._11 = height;
			m._22 = (clipFar+clipNear) / (clipNear-clipFar);
			m._23 = typ(-1.0);
			m._32 = 2*clipNear*clipFar / (clipNear-clipFar);

	/*		typ height = typ(1.0)/tan(fov/typ(2.0));
			m._01 = m._02 = m._03 = m._10 = m._12 = m._13 = m._20 = m._21 = m._30 = m._31 = m._33 = typ(0);
			m._00 = (height / aspect);
			m._11 = height;
			m._22 = clipFar / (clipFar-clipNear);
			m._23 = typ(1.0);
			m._32 = -clipNear*clipFar / (clipFar-clipNear);*/
			return m;
	}
	static m4 camLookDir( const v3& eye, const v3& dir, const v3& up ) {
		m4 m;

		const v3& zAxis(dir);
		const v3 xAxis(up.cross(zAxis));
		const v3 yAxis(zAxis.cross(xAxis));

		m._00   = xAxis.x;
		m._01   = yAxis.x;
		m._02   = zAxis.x;
		m._03   = typ(0);

		m._10   = xAxis.y;
		m._11   = yAxis.y;
		m._12   = zAxis.y;
		m._13   = typ(0);

		m._20   = xAxis.z;
		m._21   = yAxis.z;
		m._22   = zAxis.z;
		m._23   = typ(0);

		m._30   = xAxis.dot(eye);
		m._31   = yAxis.dot(eye);
		m._32   = zAxis.dot(eye);
		m._33   = typ(1);

		return m;
	}
	static m4 camLookAt( const v3& eye, const v3& at, const v3& up  ) { return camLookDir( eye, (at-eye).getNormal(), up ); }
};

Template1 mat4_T<T> operator*( const mat4_T<T> &a, const mat4_T<T> &b ) {  //todo - intrinsics
	mat4_T<T> m;
	m._00 = b._00*a._00 + b._10*a._01 + b._20*a._02 + b._30*a._03;
	m._01 = b._01*a._00 + b._11*a._01 + b._21*a._02 + b._31*a._03;
	m._02 = b._02*a._00 + b._12*a._01 + b._22*a._02 + b._32*a._03;
	m._03 = b._03*a._00 + b._13*a._01 + b._23*a._02 + b._33*a._03;

	m._10 = b._00*a._10 + b._10*a._11 + b._20*a._12 + b._30*a._13;
	m._11 = b._01*a._10 + b._11*a._11 + b._21*a._12 + b._31*a._13;
	m._12 = b._02*a._10 + b._12*a._11 + b._22*a._12 + b._32*a._13;
	m._13 = b._03*a._10 + b._13*a._11 + b._23*a._12 + b._33*a._13;

	m._20 = b._00*a._20 + b._10*a._21 + b._20*a._22 + b._30*a._23;
	m._21 = b._01*a._20 + b._11*a._21 + b._21*a._22 + b._31*a._23;
	m._22 = b._02*a._20 + b._12*a._21 + b._22*a._22 + b._32*a._23;
	m._23 = b._03*a._20 + b._13*a._21 + b._23*a._22 + b._33*a._23;

	m._30 = b._00*a._30 + b._10*a._31 + b._20*a._32 + b._30*a._33;
	m._31 = b._01*a._30 + b._11*a._31 + b._21*a._32 + b._31*a._33;
	m._32 = b._02*a._30 + b._12*a._31 + b._22*a._32 + b._32*a._33;
	m._33 = b._03*a._30 + b._13*a._31 + b._23*a._32 + b._33*a._33;
	return m;
}

Template1 vec3_T<T> operator*( const vec3_T<T> &a, const mat4_T<T> &b ) {
    vec3_T<T> v;
    v.x =  a.x*b._00 + a.y*b._10 + a.z*b._20 + b._30;
    v.y =  a.x*b._01 + a.y*b._11 + a.z*b._21 + b._31;
    v.z =  a.x*b._02 + a.y*b._12 + a.z*b._22 + b._32;
	return v;
}
Template1 vec4_T<T> operator*( const vec4_T<T> &a, const mat4_T<T> &b ) {
    vec4_T<T> v;
    v.x =  a.x*b._00 + a.y*b._10 + a.z*b._20 + a.w*b._30;
    v.y =  a.x*b._01 + a.y*b._11 + a.z*b._21 + a.w*b._31;
    v.z =  a.x*b._02 + a.y*b._12 + a.z*b._22 + a.w*b._32;
    v.w =  a.x*b._03 + a.y*b._13 + a.z*b._23 + a.w*b._33;
	return v;
}

Template1 mat4_T<T> operator+( const mat4_T<T> &a, const vec3_T<T> &b ) { auto ret = a; ret.translation()+=b; return ret; }
Template1 mat4_T<T> operator-( const mat4_T<T> &a, const vec3_T<T> &b ) { auto ret = a; ret.translation()-=b; return ret; }

Template2 mat4_T<T>& operator*=( mat4_T<T> &a, const T2 &b ) { a = a*b; return a; }

typedef mat4_T<f32> mat4f;
typedef mat4_T<f64> mat4d;

}


#ifdef GEM_MATH_QUATERNION_H
#include "Quaternion.h"
#endif //GEM_MATH_QUATERNION_H

#endif //GEM_MATH_MATRIX4_H


#if !defined(GEM_CONV_MAT4_MAT3X4) && defined(GEM_MATH_MATRIX3X4_H)
#define GEM_CONV_MAT4_MAT3X4
namespace Gem {

Template1 mat4_T<T>::mat4_T( const mat3x4_T<T> &m ) {
	r0 = vec4_T<T>( m.r0, 0 );
	r1 = vec4_T<T>( m.r1, 0 );
	r2 = vec4_T<T>( m.r2, 0 );
	r3 = vec4_T<T>( m.r3, 1 );
}

}
#endif //GEM_CONV_MAT4_MAT3X4