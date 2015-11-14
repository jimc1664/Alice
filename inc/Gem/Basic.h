#ifndef GEM_BASIC_H
#define GEM_BASIC_H

#include <stdexcept>

#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#ifdef DEBUG
#else 
#ifndef NDEBUG
#define NDEBUG
#endif
#endif

#include <crtdefs.h>
#include <crtdbg.h>
#include <new>

namespace Gem {

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned int			u32;
typedef unsigned __int64		u64;

typedef char					s8;
typedef short					s16;
typedef int						s32;
typedef __int64					s64;

typedef float 					f32;
typedef double 					f64;
typedef void*					ptr;
typedef const void*				cptr;

#ifdef  _WIN64
typedef unsigned __int64	sizet;
#else
typedef unsigned int		sizet;
#endif

typedef void(*func) ();
typedef void(*callBack) (void*);
typedef ptr(*ptrFunc) ();


#define null (0)

#define Template1 template<typename T>
#define Template2 template<typename T, typename T2>
#define Template3 template<typename T, typename T2, typename T3>
#define Template4 template<typename T, typename T2, typename T3, typename T4>
#define Template5 template<typename T, typename T2, typename T3, typename T4, typename T5>

#define Template1A template<typename T1A>
#define Template2A template<typename T1A, typename T2A>

Template2 static T convert(const T2 &b) { err }
Template1 struct Convertable { Template1A T1A as() const { return convert<T1A,T>( *static_cast<const T*>(this) ); } };

//deviation on http://www.stroustrup.com/bs_faq2.html#constraints  - for helping compiler pick correct overload 
Template1 struct Is_BasicType { enum { Check = 0, }; };
#define DEF_BasicType( a )		template<> struct Is_BasicType<a> { static void validFoo( Is_BasicType& a ) {}  typedef void(*Valid) (Is_BasicType&); enum { Check = 1, }; }
DEF_BasicType(s8);
DEF_BasicType(s16);
DEF_BasicType(s32);
DEF_BasicType(s64);
DEF_BasicType(u8);
DEF_BasicType(u16);
DEF_BasicType(u32);
DEF_BasicType(u64);
DEF_BasicType(f32);
DEF_BasicType(f64);
#undef DEF_BasicType


Template2 struct Is_ValidConversion { };


#define Constrain_BasicType( c ) typename Is_BasicType<c>::Type

#define OnlyIF( c ) ,typename c::Valid=c::validFoo
#define OnlyIF2( c,t1,t2 ) ,typename c<t1,t2>::Valid=0

#define Constraint( c ) { typename c::Valid _tmp =0; }0

#define AutoCtor( Class, Member, cd )						\
template<class T, int i> struct AutoCtor_##Member{};		\
template<class T> struct AutoCtor_##Member<T,1> {			\
	AutoCtor_##Member() { Value = cd; }						\
	operator T& () { return Value; }						\
	T Value;												\
};															\
template<class T> struct AutoCtor_##Member<T,0> : public T {\
	AutoCtor_##Member() : T cd { }							\
};															\
AutoCtor_##Member<Class, Is_BasicType<Class>::Check > Member


struct Zeroed		{ Zeroed(const u32 &a = 0) : Alloc(a) {} const u32 Alloc; };
struct Set			{ Set(const u32& v, const  u32 &a = 0) : Alloc(a), Val(v) {} const u32 Alloc, Val; };

Template2	 struct CpySpcf {
	CpySpcf(T& a) : A(a) {}
	T* operator->() const { return &A; }  T& A;
	operator T& () const { return A; }
	template<typename T3> auto sub() -> decltype(T2::copy(*((T3*)0))) { return T2::copy<T3>(A); }
	template<typename T3> static auto copy(T3 &a) -> decltype(T2::copy(a)) { return T2::copy<T3>(a); }
};
Template1	 struct Steal_Struct : public CpySpcf<T, Steal_Struct<T>>	{ Steal_Struct(T& a) : CpySpcf(a) {} template<typename T2>	Steal_Struct<T2> static copy(T2& a) { return Steal_Struct<T2>(a); } };
Template1	Steal_Struct<T> Steal(T& a) { return Steal_Struct<T>(a); }
Template1	 struct Clone_Struct : public CpySpcf<T, Clone_Struct<T>>	{ Clone_Struct(T& a) : CpySpcf(a) {} template<typename T2> Clone_Struct<T2> static copy(T2& a) { return Clone_Struct<T2>(a); } };
Template1	Clone_Struct<T> Clone(T& a) { return Clone_Struct<T>(a); }

struct NoConstruct {}; struct Identity {};  struct Default { };
struct Nothing { Nothing(){} Template1 Nothing(T){} };

class Uncopyable {
protected:
	Uncopyable() {}
	~Uncopyable() {}
private:
	template<typename uncopyable_doodad> Uncopyable(const Uncopyable&) { this_is_uncopyable }
	template<typename uncopyable_doodad> const Uncopyable& operator=(const Uncopyable&) { this_is_uncopyable }
};

#define SetUncopyable( a ) \
template<typename uncopyable_doodad> a(const a&) { this_is_uncopyable } \
template<typename uncopyable_doodad> const a& operator=(const a&) { this_is_uncopyable } \


Template1 struct Serialisable {
	T& base() { return *static_cast<T*>(this); }
	const T& base() const { return *static_cast<const T*>(this); }
};
Template1 struct RawData {

};


#define _STRINGIFY( a ) #a 
#define STRINGIFY( a ) _STRINGIFY(a)
#define FINDFAILED 0xffffffff
#define INVALIDINDEX 0xffffffff

#define S_RELEASE(a) if(a) { a->Release(); a = 0; }0

#define CCheck(a) {s8 t[u32(a)];t[0]=1; }0

/*
#define FLi const char* file, const u32 line,
#define FLi_ const char* file, const u32 line 

#define FLs File(file), Line(line),
#define FLs_ : File(file), Line(line)

#define FL __FILE__, __LINE__,
#define FL_ __FILE__, __LINE__

#define FLp file, line,
#define FLp_ file, line
#define FLp2 File, Line,
#define FLp2_ File, Line
*/

#define Inline __forceinline

#ifdef DEBUG
#define IF_DEBUG(a) a
#define IF_RELEASE(a) 0
#else
#define IF_DEBUG(a) 0
#define IF_RELEASE(a) a
#endif

#define ConstructorWrap( Obj, ObjBase ) \
				Obj() {}\
	Template1	Obj(T &&a) : ObjBase(a)			{} \
	Template2	Obj(T &&a, T2 &&b) : ObjBase(a, b)			{} \
	Template3	Obj(T &&a, T2 &&b, T3 &&c) : ObjBase(a, b, c)		{} \
	Template4	Obj(T &&a, T2 &&b, T3 &&c, T4 &&d) : ObjBase(a, b, c, d)		{} \
	Template5	Obj(T &&a, T2 &&b, T3 &&c, T4 &&d, T5 &&e) : ObjBase(a, b, c, d, e)	{}

#define ConstructorWrapP2( Obj, ObjBase ) \
	Template2	Obj(T &&a, T2 &&b) : ObjBase(a, b)			{}

//			Obj()																						{} \

#define StructWrap( Obj, ObjBase ) struct Obj : public ObjBase { ConstructorWrap( Obj, ObjBase );  }

//the compiler will very kindly automagicly make copy constuctors for you, use this so you don't accidently use them
#define setUncopyable(obj)									\
	template<typename uncopyable_doodad> void operator=(const obj &a)	{ uncopyable } \
	template<typename uncopyable_doodad> obj(const obj &a)				{ uncopyable }

#define DefGlobal( className, funcName ) u8 _Glob##className[sizeof(className)]; 
#define FwdDecGlobal( className, funcName ) class className; className& funcName();
#define DecGlobal( className, funcName ) extern DefGlobal( className, funcName ) inline className& funcName() { return *(className*)(_Glob##className); }
#define InitGlobal(className, funcName ) new (&funcName()) className
#define DeinitGlobal(className, funcName ) (funcName()).~className()
#define AutoGlobalDef(className, funcName ) struct AutoInit##className {  AutoInit##className() { InitGlobal( className, funcName ); } ~ AutoInit##className##() { DeinitGlobal( className, funcName ); } } AutoInit##funcName;
#define AutoGlobal(className, funcName )																					\
struct AutoInit##className{ \
	Template1   AutoInit##className(T &&p1)				{ InitGlobal(className, funcName)(p1); }			\
	Template2  AutoInit##className(T &&p1, T2 &&p2)	{ InitGlobal(className, funcName)(p1, p2); }		\
	~AutoInit##className##() { DeinitGlobal(className, funcName); }												\
} AutoInit##funcName;

//#define S(a) (stringC()<<a)
/*
#define PRINT(x) std::cout<<x
#define PRINTL(x) std::cout<<x<<"\n"
*/

#define CallMember( obj, fPtr ) (obj.*fPtr)
Template1 ptr offsetPtr( const ptr &p, const T &add ) { return (u8*)p+add; }

#define ArySize(a)   (sizeof(a)/sizeof(a[0]))

Template2 struct Cmb2 { const T I1; const T2 I2; };
Template2 Cmb2<T, T2> Cmb(const T &p1, const T2 &p2) { Cmb2<T, T2> c = { p1, p2 }; return c; }
Template3 struct Cmb3 { const T I1; const T2 I2; const T3 I3; };
Template3 Cmb3<T, T2, T3> Cmb(const T &p1, const T2 &p2, const T3 &p3) { Cmb3<T, T2, T3> c = { p1, p2, p3 }; return c; }


#define DEFGlobalFuncV( act, parentClass, This, code )					\
	func act; {																\
struct act##Struct : public parentClass{ \
private:	void tFunc()	{ code }							\
public:		static void Func()	{ ((act##Struct*)This)->tFunc(); }	\
}; act = act##Struct::Func; }0

#define DEFFuncV( act, code )						\
	func act; {											\
struct act##Struct{ static void Func() { code } }; \
	act = act##Struct::Func; }0

#define DEFFunc( ret, act, param, code )			\
	ret(*act) param; {									\
struct act##Struct{ static ret Func##param{ code } };	\
	act = act##Struct::Func; }0

#define DEFClassFunc( ret, act, classt, func )			\
	ret(*act) (void *t); {									\
struct act##Struct{ static ret Func(void *t) { return ((classt*)t)->func(); } };	\
	act = act##Struct::Func; }0

#define DEFClassFunc_P( ret, act, paramsT, params, classt, func )			\
	ret(*act) (void *t, paramsT); {									\
struct act##Struct{ static ret Func(void *t, paramsT) { return ((classt*)t)->func(params); } };	\
	act = act##Struct::Func; }0

#define DEFClassCB( ret, act, classt, code )			\
	ret(*act) (void *t); {									\
struct act##Struct : public classt{ \
	ret int_Func() { code } \
	static ret external_Func(void *t) { return ((act##Struct*)t)->int_Func(); } };	\
	act = act##Struct::external_Func; }0

#define DEFClassCB_P( ret, act, paramsT, params, classt, code )			\
	ret(*act) (void *, paramsT); {									\
struct act##Struct : public classt{ \
	ret int_Func(paramsT) { code } \
	static ret external_Func(void *classPtr, paramsT) { return ((act##Struct*)classPtr)->int_Func(params); } };	\
	act = act##Struct::external_Func; }0


#define Alloc2( p1, T1, prm1, p2, T2, prm2 ) {\
	u8 *p = new u8[sizeof(T1)+sizeof(T2)]; \
	p1 = (T1*)p;		p2 = (T2*)&p[sizeof(*p1)]; \
	new (p2)T2 prm2; new (p1)T1 prm1;	\
}0





/*
inline void error( char* a ) {
	PRINTL(a);
	while(1) {}
}

struct BasicErr {
	BasicErr( const char *const &str ) : Str(str) { }
	const char *Str;
	const BasicErr& print() const { PRINTL("  /Str:: "<<Str); return *this; }
};

#define PThrow(a) throw (*static_cast<const decltype(a)*>(&a.print() ))

#define DefineERR(errType) \
struct errType : public BasicErr { \
	errType( const char *const &file,  const u32 &line, const char *const &str ) : File(file), Line(line), BasicErr(str) {} \
	const char *File; \
	const u32 Line; \
	const errType& print()  const {	 PRINTL("  /Str:: "<<Str<<"  /File:: "<<File<<"  /Line:: "<<Line ); return *this; } \
};

//DefineERR(CriticalErr )
DefineERR(FailedErr)

struct CriticalErr : public FailedErr {
	CriticalErr( const char *const &file,  const u32 &line, const char *const &str ) : FailedErr(file,line,str) {
		int a = 0;
	} 
};

#define ThrowCrit(a) PThrow( CriticalErr( __FILE__, __LINE__, a ) )
#define ThrowFail(a) PThrow ( FailedErr( __FILE__, __LINE__, a ) )
#undef DefineERR

struct FindFailedErr : public BasicErr { FindFailedErr( const char *const &str ) : BasicErr("Failed to find object") {} };


#undef  Assert
#undef  AssertOnFail
#undef  AssertOnFAILED */

#ifdef  NDEBUG

#define Error(info) ((void)0)
#define Assert(Expression,Info)     ((void)0)
#define AssertOnFail(Expression,Info)    Expression
#define AssertOnFAILED(Expression,Info)    Expression

#define AssertB(Expression,Info)     ((void)0)
#define AssertBOnFail(Expression,Info)    Expression
#define AssertBOnFAILED(Expression,Info)    Expression

#define Assume(Expression)     ((void)0)
#define AssumeB(Expression)     ((void)0)

#else
/*
#ifdef  __cplusplus
extern "C" {
#endif

_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);
inline void __cdecl _wassert2(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line ) {
	_wassert(_Message, _File, _Line);
}
_CRTIMP void __cdecl _assert(_In_z_ const char * _Message, _In_z_ const char *_File, _In_ unsigned _Line);

#ifdef  __cplusplus
}
#endif */
void debugBreak();

//#define ThrowAssert(info)  _wassert2(_CRT_WIDE(#info), _CRT_WIDE(__FILE__), __LINE__)
#define ThrowAssert(info) { debugBreak(); throw std::logic_error( info ## "  --at line: "STRINGIFY(__LINE__)" in: " __FILE__  ); }0
#define Error(info)  ThrowAssert(info)

//fyi the trailing 0 is to force correct usage ie [ Assert(sumFing); ] - not [ Assert(sumFing) ], 
#define Assert(expression,info) { if(!(expression)) ThrowAssert(info); } 0  

//#define Assert(expression,info)				{ if(!(expression)) ThrowCrit(info###expression); } 0  
#define AssertOnFail(expression,info)		Assert(expression,info)
#define AssertOnFAILED(expression,info)		Assert(SUCCEEDED(expression),info)

//#define AssertB(expression,info)			{ if(!(expression)) _assert(info###expression,__FILE__, __LINE__); } 0
//#define AssertBOnFail(expression,info)		Assert(expression,info)
//#define AssertBOnFAILED(expression,info)	Assert(SUCCEEDED(expression),info)

#define Assume(expression)					Assert(expression,"Incorrect assumption := "###expression )
//#define AssumeB(expression)					AssertB(expression,"Incorrect assumption := ")


//#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

#endif  /* NDEBUG */

#pragma warning(disable:4355) //this used in initialiser list
#pragma warning(disable:4345) //"behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized" ie: nothing



class DebugFlag  {
public:
#ifdef DEBUG
	DebugFlag() : Flg(0) {}
	void check( const u32 &mask ) { Assume( (Flg & mask) == mask ); }
	void set( const u32 &mask ) { Flg |= mask; }
	void clear( const u32 &mask ) { Flg &= ~mask; }
private:
	u32 Flg;
#else
	void check( const u32 &mask ) {}
	void set( const u32 &mask ) { }
	void clear( const u32 &mask ) { }
#endif
};

}

#ifdef _M_IX86
#define LibName_OX( sub ) sub##"_x86"
#elif _M_X64
#define LibName_OX( sub ) sub##"_x64"
#else
#error "unhandled processor architecture"
#endif

#ifdef DEBUG
#ifdef _GEM_TEST_
#define LibName( sub ) "GemFW_"##LibName_OX(sub)##"_t_d"
#else
#define LibName( sub ) "GemFW_"##LibName_OX(sub)"_d"
#endif
#else
#ifdef _GEM_TEST_
#define LibName( sub ) "GemFW_"##LibName_OX(sub)##"_t"
#else
#define LibName( sub ) "GemFW_"##LibName_OX(sub)
#endif
#endif

#ifdef DEBUG
#define ExtLibName( sub ) sub"_d"
#else
#define ExtLibName( sub ) sub
#endif


#ifdef _WINDOWS_
#define OS_Dat(b, a, c) \
	union {	\
		a _##c;	\
		b c;	\
	}
#else
#define OS_Dat(b, a, c) \
	union {	\
		a _##c;	\
	}
#endif //_WINDOWS_

#ifdef _WINDOWS_
#define OS_DatA(b, a, c) \
	void z_##c##_SizeCheck() { static_assert((a) == sizeof(b), "Err: OS_Dat Size Check Mismatch" #c); } \
	union {	\
		u8 _##c[a];	\
		b c;	\
	}
#else
#define OS_DatA(b, a, c) \
	union {	\
		u8 _##c[a];	\
	}
#endif //_WINDOWS_

#ifdef DIS_INTERNAL
#define Dis_Dat(b, a, c) \
	void z_##c##_SizeCheck() { static_assert(sizeof(a) == sizeof(b), "Err: Dis_Dat Size Check Mismatch" #c); } \
	union {	\
		b c;	\
	}
#else
#define Dis_Dat(b, a, c) \
	union {	\
		a c;	\
	}
#endif //DIS_INTERNAL


#ifdef DIS_INTERNAL
#define Dis_DatA(b, a, c) \
	void z_##c##_SizeCheck() { static_assert((a) == sizeof(b), "Err: Dis_Dat Size Check Mismatch" #c); } \
	union {	\
		u8 _##c[a];	\
		b c;	\
	}
#else
#define Dis_DatA(b, a, c) \
	union {	\
		u8 _##c[a];	\
	}
#endif //DIS_INTERNAL


#endif //GEM_BASIC_H