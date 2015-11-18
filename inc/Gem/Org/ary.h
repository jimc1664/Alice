#ifndef GEM_ORG_ARY_H
#define GEM_ORG_ARY_H

#include "../Basic.h"

namespace Gem {
	
namespace Ctor {
	struct Standard { template<class Obj> struct Sub { 
					void construct( Obj &at )									{ new (&at) Obj(); }
		Template1	void construct( Obj &at, T &&o )							{ new (&at) Obj(o); }
					void mulConstruct( Obj* const &at, u32 c )					{ Assume(c!= 0); for(;;) { --c; new (&at[c]) Obj();		if(!c) break; } }
//		Template1	void mulConstruct( Obj* const &at, u32 c, T &&o )			{ Assume(c!= 0); for(;;) { --c; new (&at[c]) Obj(o);	if(!c) break; } }
		Template1	void mulConstruct( Obj* const &at, u32 c, T *const &o )		{ Assume(c!= 0); for(;;) { --c; new (&at[c]) Obj(o[c]);	if(!c) break; } }
					void destruct( Obj &at )									{ at.~Obj(); }
					void mulDestruct( Obj* const &at, u32 c )					{ Assume(c!= 0); for(;;) { --c; at[c].~Obj();			if(!c) break; } }

		Template2	void mulConstruct( Obj* const &at, u32 c, T *const &o, const T2 &cpySpcf )		
		{ Assume(c!= 0); for(;;) { --c; new (&at[c]) Obj(cpySpcf.copy(o[c]));	if(!c) break; } }
	};  Template1 static Sub<T> getCnType();  };

	struct Simple { template<class Obj> struct Sub {
					void construct( Obj &at )									{}
		Template1	void construct( Obj &at, T &&o )							{ new (&at) Obj(o); }
					void mulConstruct( Obj* const &at, u32 c )					{ }
//		Template1	void mulConstruct( Obj* const &at, u32 c, T &&o )			{ Assume(c!= 0); for(;;) { --c; new (&at[c]) Obj(o);	if(!c) break; } }
		Template1	void mulConstruct( Obj* const &at, u32 c, T *const &o )		{ Assume(c!= 0); for(;;) { --c; new (&at[c]) Obj(o[c]);	if(!c) break; } }
					void destruct( Obj &at )									{}
					void mulDestruct( Obj* const &at, u32 c )					{}

		Template2	void mulConstruct( Obj* const &at, u32 c, T *const &o, const T2 &cpySpcf )		
		{ Assume(c!= 0); for(;;) { --c; new (&at[c]) Obj(cpySpcf.copy(o[c]));	if(!c) break; } }
	}; Template1 static Sub<T> getCnType();  };

	struct SimpleZeroed { template<class Obj> struct Sub  : public Simple::Sub<Obj> {
					void construct( Obj &at )									{ memset( &at,0,sizeof(at) ); }
		Template1	void construct( Obj &at, T &&o )							{ new (&at) Obj(o); }
					void mulConstruct( Obj* const &at, u32 c )					{ memset( at,0,sizeof(at)*c );}
	}; Template1 static Sub<T> getCnType();  };

	/*struct StandardNoDefault { template<class Obj> struct Sub  : public Standard::Sub<Obj> {
					void construct( Obj &at );
		Template1	void construct( Obj &at, T &&o )							{ new (&at) Obj(o); }
					void mulConstruct( Obj* const &at, u32 c );
	}; Template1 static Sub<T> getCnType();  };*/
}

template<class Obj, class CnType = Ctor::Standard > class ary  {
#define DEFAULT_EXTENDBY 32
	typedef decltype( CnType::getCnType<Obj>() ) CnTyp; 
public:
	
	CnTyp Cn;
//	typedef typename CnType::Sub<Obj> Cn;  //todo --  why dafuq this ain't working no more
	
	ary() : Allocated(0), Count(0), Data(0)			{}
	ary( const ary<Obj> &s )						: Allocated( s.Count ), Count(s.Count), Data( alloc(s.Count) ) { Cn.mulConstruct(Data,Count,s.Data); }
	ary( const u32 &allocated )								: Allocated(allocated), Count(0), Data( alloc(allocated) )	{}
	ary( const u32 &allocated, const u32 &count )				: Allocated(allocated), Count(count), Data( alloc(allocated) )	{ Cn.mulConstruct(Data,count); }
	ary( const u32 &allocated, const u32 &count, const Obj &o )	: Allocated(allocated), Count(count), Data( alloc(allocated) )	{ Cn.mulConstruct(Data,count,o); }

	void reserve( const u32 &amnt ) { Assume(Data == 0); Data = alloc(Allocated=amnt); }
	Template1 ary( const Steal_Struct<T> &s )		: Allocated( s->Allocated ), Count(s->Count), Data( s->Data ) {  s->Allocated = s->Count = 0; s->Data = 0; }
	Template1 ary( const Clone_Struct<T> &s )		: Allocated( s->Count ), Count(s->Count), Data( alloc(s->Count) ) { Cn.mulConstruct(Data,Count,s->Data,s); }
	
	~ary()											{ if(Allocated){ if(Count) Cn.mulDestruct(Data,Count);  free(Data);} }

//			operator Obj* &				()				{ return Data; }
	operator const Obj* const &	()			  const { return Data; }
	Obj&		operator[](const u32 &ind)			{ Assume(ind<Count); return Data[ind]; }
	const Obj&	operator[](const u32 &ind)	  const	{ Assume(ind<Count); return Data[ind]; }
	Obj&		operator[](const s32 &ind)			{ return (*this)[(u32)ind]; }
	const Obj&	operator[](const s32 &ind)	  const	{ return (*this)[(u32)ind]; }
	const u32&	count()						  const { return Count; }
	u32			sizeOf()					  const { return Count*sizeof(Obj); }

	Template1 Obj&	add( T &&allocated, const u32 &extendBy = DEFAULT_EXTENDBY ) {
		Assume( !Data || (((sizet)&allocated < (sizet)Data) || ((sizet)&allocated > (sizet)&Data[Allocated-1]))); //copying stored data could be errorous as allocated following realloc could therefore invalidate 'allocated'
		extendIfNeeded( Count+1, extendBy ); 
		Cn.construct( Data[Count], allocated );
		return Data[Count++];
	}
	Obj&	add() {
		int nc = Count + 1;
		extendIfNeeded( nc ); 
		Cn.construct( Data[Count] );
		return Data[Count=nc];
	}
	void	push( const Obj &allocated ) { add(allocated); }
	u32		pushI( const Obj &allocated ) { u32 r = Count; add(allocated); return r;  }
	u32		pushI() { u32 r = Count; add(); return r;  }

	Obj pop() {
		Assume( Count );
		Obj ret = Data[Count-1];
		decCount();
		return ret;
	}
	void addMul( const u32 &amnt, const Obj allocated[], const u32 &extendBy ) { 
		Assume( amnt <= extendBy );
		extendIfNeeded( Count+amnt, extendBy ); 
		Cn.mulConstruct( &Data[Count], amnt, allocated );
		Count += amnt;
	}
	void addMul( const u32 &amnt, const Obj allocated[] ) { 
		extendIfNeeded( Count+amnt, amnt ); 
		Cn.mulConstruct( &Data[Count], amnt, allocated );
		Count += amnt;
	}
	void addMul( const u32 &amnt )	{ 
		extendIfNeeded( Count+amnt, amnt ); 
		Cn.mulConstruct( &Data[Count], amnt );
		Count += amnt;
	}
	void enSpace( const u32 &amnt ) { extendIfNeeded( Count+amnt, amnt ); }

	void decCount() { Assume( Count != 0 ); Cn.destruct( Data[--Count]); }
	void decCountTo( const u32 &c ) {
		Assume( c < Count );
		Cn.mulDestruct( &Data[c], Count-c);
		Count = c;
	}	

	void sfDecCountTo( const u32 &c ) { if( c < Count ) decCountTo(c); }  //safe version
	void clear() { sfDecCountTo(0); }


	void erase( const u32 &i ) {
		Assume( i < Count );
		Count--;
		if( i != Count ) {
			memcpy( &Data[i], &Data[Count], sizeof( Obj ) );
		}
	}
	class iter {
	public:
		iter( ary &allocated, const u32 &i ) : Ind(i), Array(allocated) {}
		void operator++(int)				{ Ind++; }
		void operator++()				{ ++Ind; }
		operator bool()					{ return Ind < Array.count(); }
		operator Obj&()					{ return Array[Ind]; }
		Obj* operator->()				{ return &Array[Ind]; }
		Obj& operator*()					{ return Array[Ind]; }
		const u32& ind()			const	{ return Ind; }

		bool operator == ( const iter &o ) const { return o.Ind == Ind; }
		bool operator != ( const iter &o ) const { return o.Ind != Ind; }
	protected:
		u32 Ind;
		ary &Array;
	};

	/*class rvsIter : public iter {
	public:
		rvsIter( ary &allocated, const u32 &i ) : iter(allocated, i ) {}
		bool operator--(int)				{ return Ind-- != 0; }
	}; */

	iter start()		{ return iter(*this, 0 ); }

	iter begin()		{ return start(); }
	iter end()			{ return iter(*this, Count ); }

	//void Iterate( void (Obj::*fnc)(void) )						{ for( iter it(first()); it; it++ ) ((*it).*fnc)(); }
//	Template void Iterate( void (Obj::*fnc)(T&), T &p1 )			{ for( iter it(first()); it; it++ ) ((*it).*fnc)(p1); }


	bool contains( const Obj o ) const {
		for( int i = Count; i--; )
			if (Data[i] == o)
				return true;
		return false;
	}

protected:
	static	Obj*	alloc( const u32 &allocated ) { return static_cast<Obj*>(malloc(allocated*sizeof(Obj))); }
	void	extendIfNeeded( const u32 &nCount, const u32 &extendBy = DEFAULT_EXTENDBY ) { 		
		if( nCount > Allocated ) {
			Data = static_cast<Obj*>(realloc(Data,(Allocated+=(Allocated+extendBy))*sizeof(Obj)));
		}
	}
	u32 Allocated;
	u32 Count;
	Obj* Data;
	//setUncopyable(ary);
};


Template1 struct StrideIter {
	StrideIter() : Cur(0), Remaining(0), Stride(0) {}
	StrideIter( 	T* start, const u32 & amnt, const u32 &stride ) : Cur(start), Remaining(amnt), Stride(stride) {}
	operator bool() { return Remaining != 0; }
	operator T&() { return *Cur; }
	T& operator++(int) { T& ret = *Cur; Remaining--; Cur = (T*)(((u8*)Cur)+Stride); return ret; }
	const u32& remain() const { return Remaining; }
private:	
	T* Cur;
	u32 Remaining;
	u32 Stride;
};
#define GetStrideIter( Array, Member ) (StrideIter< decltype( Array[0].Member ) > ( &Array[0].Member, Array.count(), sizeof( decltype( Array[0] ) ) ))

}

#endif //GEM_ORG_ARY_H