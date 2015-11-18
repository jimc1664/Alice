#ifndef GEM_MATH_MATRIX3_H
#define GEM_MATH_MATRIX3_H

#include "Vec3.h"

namespace Gem {

template<class typ> class mat3_T : public Convertable<mat3_T<typ>> {
	typedef mat3_T<typ> m3;
	typedef vec3_T<typ> v3;
public:
	m3() {}
	//Template1 m3( const typ &a ) : tuple( convert<m3,typ>(a) ) {}

	m3( const typ* data ) { memcpy(d,data,sizeof(typ)*9); } 
	m3( const m3 &a ) { memcpy(d,a.d,sizeof(typ)*9); } 
	//m3( const mat4<typ> &a );

	template< class T2 >  mat3_T<T2> cast() const { m3<T2> a; for( u32 i = 0; i != 9; i++ ) a.d[i] = T2(d[i]); return a; } 

	m3  getTranspose() const {
		m3 r; 
		r._00 = _00; r._11 = _11; r._22 = _22;
		r._01 = _10; r._02 = _20;
		r._10 = _01; r._12 = _21;
		r._20 = _02; r._21 = _12;
		return r;
	}

	/*
	m3 Scale( const v3 &scale );
	m3 Scale( const typ &scale );

	m3 RotateFull( const v3 &rot );
	m3 RotateGlb( const v3 &rot );

	m3 MulRotation( const m3 &a );

	m3& setRotation( const v3 &rot );
	m3& setRotation( const m3 &rot );
	m3& SetTranslation( const v3 &pos );
	v3& GetTranslation() { return row3; }

	m3& SetView( const v3 &pos, const m3 rot );

	m3& Invert()  { *this = getInverse(); return *this; }
	*/


	m3& setRotation( const v3 &rot ) { 
		const typ cx = cos( rot.x );
		const typ sx = sin( rot.x );
		const typ cy = cos( rot.y );
		const typ sy = sin( rot.y );
		const typ cz = cos( rot.z );
		const typ sz = sin( rot.z );

		const typ sxsy = sx*sy;
		const typ cxsy = cx*sy;

		_00    = ( cy*cz );
		_01    = ( cy*sz );
		_02    = ( -sy );

		_10    = ( sxsy*cz-cx*sz );
		_11    = ( sxsy*sz+cx*cz );
		_12    = ( sx*cy );

		_20    = ( cxsy*cz+sx*sz );
		_21    = ( cxsy*sz-sx*cz );
		_22   = ( cx*cy );

		return *this;
	}
	m3& setRotationX( const typ &rot ) { 
		return setRotation(v3(rot, 0, 0)); //todo...
	}
	m3& setRotationY( const typ &rot ) { 
		const typ cy = cos( rot );
		const typ sy = sin( rot );

		_00    = ( cy );
		_01    = ( 0 );
		_02    = ( -sy );

		_10    =   _12    = 0; _11    =  1;

		_20    = ( sy );
		_21    = ( 0 );
		_22   = ( cy );

		return *this;
	}


	m3& setRotationZ( const typ &rot ) { 
		const typ cz = cos( rot );
		const typ sz = sin( rot );
		_00    = ( cz );
		_01    = ( sz );
		_02    = ( 0 );

		_10    = ( -sz );
		_11    = ( cz );
		_12    = ( 0 );

		_20    =  _21    =  _22   = 0 ;
		return *this;
	}

	m3& setLookDir( const v3& dir, const v3& up ) {
		row[2] = dir;
		row[0] = up.cross(row[2]).getNormal();
		row[1] = row[2].cross(row[0]);
		return *this;
	}
	
	m3& setIdentity() {
		_00 = _11 = _22 = typ(1);
		_01 = _02 = typ(0);
		_10 = _12 = typ(0);
		_20 = _21 = typ(0);
		return *this;
	}
	//casting
	operator typ* () { return d; }	 
	operator const typ* () const { return d; }

	//matrix builders
	static m3 identity() { return m3().setIdentity(); }

	static m3 rotation( const v3 &rot ) { return m3().setRotation( rot ); }
	static m3 rotationX( const typ &rot ) { return m3().setRotationX( rot ); }
	static m3 rotationY( const typ &rot ) { return m3().setRotationY( rot ); }
	static m3 rotationZ( const typ &rot ) { return m3().setRotationZ( rot ); }
	static m3 lookDir( const v3& dir, const v3& up ) { return m3().setLookDir( dir, up ); }


	static m3 scale( const v3 &scale ) {
		m3 m;
		m._00 = scale.x; m._11 = scale.y; m._22 = scale.z;
		m._01  = m._02  = m._10  = m._12  = m._20 =  m._21  = typ(0);
		return m;
	}


	union {
		typ d[9];
		typ data[3][3];
		struct {
			typ _00, _01, _02;
			typ _10, _11, _12;
			typ _20, _21, _22;
		};
		struct {
			v3 row[3];
		};
	};

};


template<class typ> vec3_T<typ> operator* ( const vec3_T<typ> &a, const mat3_T<typ> &b ) {
	vec3_T<typ> v;
	v.x =  a.x*b._00 + a.y*b._10 + a.z*b._20;
	v.y =  a.x*b._01 + a.y*b._11 + a.z*b._21;
	v.z =  a.x*b._02 + a.y*b._12 + a.z*b._22;
	return v;
}


template<class typ> mat3_T<typ> operator* ( const mat3_T<typ> &a, const mat3_T<typ> &b ) {
	mat3_T<typ> m;
	m._00 = b._00*a._00 + b._10*a._01 + b._20*a._02;
	m._01 = b._01*a._00 + b._11*a._01 + b._21*a._02;
	m._02 = b._02*a._00 + b._12*a._01 + b._22*a._02;

	m._10 = b._00*a._10 + b._10*a._11 + b._20*a._12;
	m._11 = b._01*a._10 + b._11*a._11 + b._21*a._12;
	m._12 = b._02*a._10 + b._12*a._11 + b._22*a._12;

	m._20 = b._00*a._20 + b._10*a._21 + b._20*a._22;
	m._21 = b._01*a._20 + b._11*a._21 + b._21*a._22;
	m._22 = b._02*a._20 + b._12*a._21 + b._22*a._22;

	return m;
}

typedef mat3_T<f32> mat3f;
typedef mat3_T<f64> mat3d;

}

#ifdef GEM_MATH_QUATERNION_H
#include "Quaternion.h"
#endif //GEM_MATH_QUATERNION_H

#endif //GEM_MATH_MATRIX3_H