#ifndef GEM_OS_TIME_H
#define GEM_OS_TIME_H

#include "../Basic.h"

namespace Gem { 
	
class Randomizer;

namespace OS {

//a note on the difference between Time and Time::Gap ... Time represents the time since some arbituary event, so it makes no sense for it to be used in anything other than [ Time1 - Time2 ], since
// -- the arbitary value constant is is canceled out, giving a perfectly useable Gap representing the difference between Time1 and Time2
//another note||todo there is an inaccuracy where this is used on a cpu with multiple cores, fixable (i think) via SetProcessorAffinity

class TimeInitator;

class Time {
public: 
	Time( NoConstruct ) {}
	Time() : Val(Current()) {}

	Time& update() { Val = Current(); return *this; }
	s64& raw() { return Val; }

	class Gap {
	public:
		Gap() {}
		f64 asF() const {  return (f64)Val/HPCFreqF; }
		s64 asI() const {  return Val*1000/HPCFreq; }

		operator f32() const { return (f32)asF(); }
		operator f64() const { return asF(); }
		operator s32() const { return (s32)asI(); }
		operator s64() const { return asI(); }
		operator u32() const { return (u32)asI(); }

		bool operator <  ( const Gap &a ) { return Val <  a.Val; }
		bool operator >  ( const Gap &a ) { return Val >  a.Val; }
		bool operator >= ( const Gap &a ) { return Val >= a.Val; }
		bool operator <= ( const Gap &a ) { return Val <= a.Val; }
		bool operator == ( const Gap &a ) { return Val == a.Val; }
		bool operator != ( const Gap &a ) { return Val != a.Val; }

		Gap& operator *= ( const s64 &a ) { Val*=a; return *this; }
		Gap& operator /= ( const s64 &a ) { Val/=a; return *this; }
	
		Gap& operator += ( const Gap &a ) { Val+=a.Val; return *this; }
		Gap& operator -= ( const Gap &a ) { Val-=a.Val; return *this; }
		Gap& operator *= ( const Gap &a ) { Val*=a.Val; return *this; }
		Gap& operator /= ( const Gap &a ) { Val/=a.Val; return *this; }	
	
		Template1 Gap operator + ( const T &a ) const { Gap t(*this); return t+=a; }
		Template1 Gap operator - ( const T &a ) const { Gap t(*this); return t-=a; }
		Template1 Gap operator * ( const T &a ) const { Gap t(*this); return t*=a; }
		Template1 Gap operator / ( const T &a ) const { Gap t(*this); return t/=a; }

		void set( const f64 &val ) { Val = (s64)(val * HPCFreqF); }
	private: friend class Time;
		Gap( const s64 &a, const s64 &b ) : Val( b -a ) { }
		s64 Val;
	};

	Gap operator-( const Time &a ) const { return Gap( a.Val, Val ); }
	Gap	elap() { return Gap( Val, Current() ); }
	Gap	updateElap() { s64 tVal = Val; update(); return Gap( tVal, Val ); }

	/*
	Time& operator*= ( const s64 &a ) { Val*=a; return *this; }
	Time& operator/= ( const s64 &a ) { Val/=a; return *this; }
	
	Time& operator+= ( const Time &a ) { Val+=a.Val; return *this; }
	Time& operator-= ( const Time &a ) { Val-=a.Val; return *this; }
	Time& operator*= ( const Time &a ) { Val*=a.Val; return *this; }
	Time& operator/= ( const Time &a ) { Val/=a.Val; return *this; }	
	
	Template1 Time operator+( const T &a ) const { Time t(*this); return t+=a; }
	Template1 Time operator-( const T &a ) const { Time t(*this); return t-=a; }
	Template1 Time operator*( const T &a ) const { Time t(*this); return t*=a; }
	Template1 Time operator/( const T &a ) const { Time t(*this); return t/=a; }
	*/
private:
	friend class TimeInitator;
	friend class Randomizer;

	static s64 Current();

	static f64 HPCFreqF;
	static s64 HPCFreq;
	s64 Val;
};


}	}

#endif //GEM_OS_TIME_H