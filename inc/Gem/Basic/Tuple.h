#ifndef GEM_BASIC_TUPLE_H
#define GEM_BASIC_TUPLE_H

#include "../Basic.h"

namespace Gem {

template< typename Typ1, typename Typ2, typename Typ3 = Nothing, typename Typ4 = Nothing, typename Typ5 = Nothing > struct tuple;

template< typename Typ1, typename Typ2> struct tuple<Typ1, Typ2, Nothing, Nothing, Nothing> {
	tuple() {}
	tuple(const Typ1 &A, const Typ2 &B) : x(A), y(B) {}
	tuple( const tuple &t ) : x(t.x), y(t.y) {}

//	Template2 tuple(const tuple<T, T2> &t) : x(t.x) y(t.y) {}

	operator bool() const { return x || y; }
	bool operator==(const tuple &i) const { return (x == i.x)&(y == i.y); }
	bool operator!=(const tuple &i) const { return (x != i.x) | (y != i.y); }

	tuple operator-() const { return tuple(-x, -y); }
	tuple operator+(const tuple &i) const { return tuple(x + i.x, y + i.y); }
	tuple operator-(const tuple &i) const { return tuple(x - i.x, y - i.y); }
	tuple operator*(const tuple &i) const { return tuple(x*i.x, y*i.y); }
	tuple operator/(const tuple &i) const { return tuple(x / i.x, y / i.y); }

	Template1 tuple operator+(const T &i) const { return tuple(x + i, y + i); }
	Template1 tuple operator-(const T &i) const { return tuple(x - i, y - i); }
	Template1 tuple operator*(const T &i) const { return tuple(x*i, y*i); }
	Template1 tuple operator/(const T &i) const { return tuple(x / i, y / i); }

	Template1 tuple& operator+= (const T &i) { *this = *this + i; return *this; }
	Template1 tuple& operator-= (const T &i) { *this = *this - i; return *this; }
	Template1 tuple& operator*= (const T &i) { *this = *this*i; return *this; }
	Template1 tuple& operator/= (const T &i) { *this = *this / i; return *this; }


	//Template2A tpl_2( const tpl_2<T1A, T2A> &t ) : x(t.x), y(t.y) {}
	Typ1 x;		Typ2 y;
};
Template2 tuple<T, T2> tpl(T &&a, T2 &&b) { return tuple<T, T2>(a, b); }

template< typename Typ1, typename Typ2, typename Typ3> struct tuple<Typ1, Typ2, Typ3, Nothing, Nothing> {
	tuple() {}
	tuple(const Typ1 &A, const Typ2 &B, const Typ3 &C) : x(A), y(B), z(C) {}
	tuple( const tuple &t ) : x(t.x), y(t.y), z(t.z){}
	Template1 tuple( const tuple<Typ1,Typ2,Typ3,T> &t ) : x(t.x), y(t.y), z(t.z){}
	tuple( const tuple<Typ1,Typ2> &t ) : x(t.x), y(t.y) {}
	/*tuple(const Typ1 &A, const Typ2 &B) : x(A), y(B) {}

	Template2 tuple(const tuple<T, T2> &t) : x(t.x) y(t.y) {}

	operator bool() const { return x || y; }
	bool operator==(const tuple &i) const { return (x == i.x)&(y == i.y); }
	bool operator!=(const tuple &i) const { return (x != i.x) | (y != i.y); }

	*/

	tuple operator+(const tuple &i) const { return tuple(x+i.x, y+i.y, z+i.z); }
	Template1 tuple operator+(const T &i) const { return tuple(x+i, y+i, z+i); }
	Template1 tuple& operator+= (const T &i) { *this = *this + i; return *this; }

	tuple operator-(const tuple &i) const { return tuple(x-i.x, y-i.y, z-i.z); }
	Template1 tuple operator-(const T &i) const { return tuple(x-i, y-i, z-i); }
	Template1 tuple& operator-= (const T &i) { *this = *this - i; return *this; }

	tuple operator*(const tuple &i) const { return tuple(x*i.x, y*i.y, z*i.z); }
	Template1 tuple operator*(const T &i) const { return tuple(x*i, y*i, z*i); }
	Template1 tuple& operator*= (const T &i) { *this = *this * i; return *this; }

	tuple operator/(const tuple &i) const { return tuple(x/i.x, y/i.y, z/i.z); }
	Template1 tuple operator/(const T &i) const { return tuple(x/i, y/i, z/i); }
	Template1 tuple& operator/= (const T &i) { *this = *this / i; return *this; }

	//Template2A tpl_2( const tpl_2<T1A, T2A> &t ) : x(t.x), y(t.y) {}
	Typ1 x;		Typ2 y;		Typ3 z;
};
Template3 tuple<T, T2, T3> tpl(T &&a, T2 &&b, T3 &&c) { return tuple<T, T2, T3>(a, b, c); }


template< typename Typ1, typename Typ2, typename Typ3, typename Typ4> struct tuple<Typ1, Typ2, Typ3, Typ4, Nothing> {
	tuple() {}
	tuple(const Typ1 &A, const Typ2 &B, const Typ3 &C, const Typ4 &D) : x(A), y(B), z(C), w(D) {}
	tuple( const tuple &t ) : x(t.x), y(t.y), z(t.z), w(t.w) {}

	//Template1 tuple( const tuple<Typ1,Typ2,Typ3,T> &t ) : x(t.x), y(t.y), z(t.z){}
	tuple( const tuple<Typ1,Typ2,Typ3> &t ) : x(t.x), y(t.y), z(t.z) {}
	/*

	Template2 tuple(const tuple<T, T2> &t) : x(t.x) y(t.y) {}

	operator bool() const { return x || y; }
	bool operator==(const tuple &i) const { return (x == i.x)&(y == i.y); }
	bool operator!=(const tuple &i) const { return (x != i.x) | (y != i.y); }

	*/

	tuple operator+(const tuple &i) const { return tuple(x+i.x, y+i.y, z+i.z, w+i.w); }
	Template1 tuple operator+(const T &i) const { return tuple(x+i, y+i, z+i, w+i); }
	Template1 tuple& operator+= (const T &i) { *this = *this + i; return *this; }

	tuple operator-(const tuple &i) const { return tuple(x-i.x, y-i.y, z-i.z, w-i.w); }
	Template1 tuple operator-(const T &i) const { return tuple(x-i, y-i, z-i, w-i); }
	Template1 tuple& operator-= (const T &i) { *this = *this - i; return *this; }

	tuple operator*(const tuple &i) const { return tuple(x*i.x, y*i.y, z*i.z, w*i.w); }
	Template1 tuple operator*(const T &i) const { return tuple(x*i, y*i, z*i, w*i); }
	Template1 tuple& operator*= (const T &i) { *this = *this * i; return *this; }

	tuple operator/(const tuple &i) const { return tuple(x/i.x, y/i.y, z/i.z, w/i.w); }
	Template1 tuple operator/(const T &i) const { return tuple(x/i, y/i, z/i, w/i); }
	Template1 tuple& operator/= (const T &i) { *this = *this / i; return *this; }
	//Template2A tpl_2( const tpl_2<T1A, T2A> &t ) : x(t.x), y(t.y) {}
	Typ1 x;		Typ2 y;		Typ3 z;		Typ4 w;
};
Template4 tuple<T, T2, T3, T4> tpl(T &&a, T2 &&b, T3 &&c, T4 &&d) { return tuple<T, T2, T3, T4>(a, b, c, d); }


template< int Dim, typename Typ> struct tupleD;

template< typename Typ> struct tupleD<1,Typ> {

	tupleD() {}
	tupleD(const Typ &A, const Typ &B) :Data(A) {}
	tupleD( const Typ a[1] ) : Data(a[0]) {}

	operator Typ&() { return Data; }
	operator const Typ&() const { return Data; }

	Typ Data;
};
template< typename Typ> struct tupleD<2,Typ> : public tuple<Typ,Typ> {
	tupleD() {}
	tupleD(const Typ &A, const Typ &B ) :tuple(A,B) {}
	tupleD( const tuple &t ) : tuple(t) {}
	tupleD( const Typ a[2] ) : tuple(a[0],a[1]) {}
};


}

#endif //GEM_BASIC_TUPLE_H