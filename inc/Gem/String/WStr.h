#ifndef GEM_STRING_WSTR_H
#define GEM_STRING_WSTR_H

#include "CStr.h"
#include "StrStream.h"

namespace Gem {

Template1 class StrStream;

template<u32 Size, class CharT> class WStrTmplt : public StrStream<WStrTmplt<Size,CharT>>, public StringBase<CharT,WStrTmplt<Size,CharT>> {
public:
	WStrTmplt() : Len(0) { Str[0] = 0; }
//	WStrTmplt( const CStrTemplate<CharT> &i ) : Len(i.len()) { memcpy(Str,&(i[0]),Len*sizeof(CharT)); Str[Len] = 0; }

//	template<class Obj1, class Obj2>
	//WStrTmplt( const CondenseHlpr<CharT,Obj1,Obj2> &a ) : Len(a.len()) { Assume(Size>Len); a.copyTo(&Str[0]); Str[Len] = 0; }
	template<class Obj1> WStrTmplt( const StringBase_Sub<CharT,Obj1> &a ) : Len(a.len()) { Assume(Size>Len); a.copyTo(&Str[0]); Str[Len] = 0; }
	WStrTmplt( const StringUkn &a ) : Len(a.len()) { Assume(Size>Len); a.copyTo(&Str[0]); Str[Len] = 0; }
	
	WStrTmplt( const char *const &c )		: Len(0) { while( Str[Len] = (CharT)c[Len] ) { Len++;  Assume(Len < Size ); } }
	WStrTmplt( const wchar_t *const &w )	: Len(0) { while( Str[Len] = (CharT)w[Len] ) { Len++;  Assume(Len < Size ); } }
	
	const u32& reserved()const { return Size;	}
	u32 len()		const { return Len;		}
	//void copyTo( char* const &to ) const override;

	//operator CStrTemplate<CharT> ()				{ return CStrTemplate<CharT>( Str, Len ) ;}
	operator CharT* ()				{ return Str;		}
	//operator const CharT* ()	const	{ return Str;		}
	CharT* str()					{ return Str;		}
	const CharT* str()		const	{ return Str;		}


	WStrTmplt& operator<<( const CStrTemplate<CharT> &r )	{ 
		Assume( (Len + r.len() ) < Size );
		memcpy( end(), &(r[0]), r.len()*sizeof(CharT) );
		Len += r.len();
		*end() = 0;
		return *this; 
	} 

	template<class T2> WStrTmplt& operator += ( const T2 &r ) { return *this<<r; }

	template<class Ret_t, class InLen_t> 
	WStrTmplt& appendFnc( Ret_t(__stdcall*func)( CharT*, InLen_t) ) {
		func( end(),remaining() );
		fixLen();
		return *this; //todo - ensure whole thing copied
	}
	template<class Ret_t, class In1_t, class InLen_t> 
	WStrTmplt& appendFnc( Ret_t(__stdcall*func)(In1_t, CharT*, InLen_t), const In1_t &in1 ) {
		func(in1,end(),remaining());
		fixLen();
		return *this; //todo - ensure whole thing copied
	}

	WStrTmplt& fileToDir() { 
		Assume(Len != 0);
		for( int i = Len; --i; ) {
			if( isSlash(Str[i]) ) {
				Len = i+1;
				Str[Len] = 0;
				return *this;
			}
		}
		Error("WStrTmplt::fileToDir() Failed!  Could not find \\ ");
		return *this;
	}
	void fixLen() {
		for( ; Len != Size; Len++ )  if( !Str[Len] ) return;
		Error("WStrTmplt::fixLen() Failed!  Could not find end of string ");
	}
	void fixLen( const u32 &len ) {
		Assume( len < Size ); Assume( len > Len ); Assume( !Str[len] ); Assume( Str[len-1] );
		Len = len;
	}
	void clear() { Len = 0; Str[0] = 0; }

	CharT* end()						{ return &Str[Len];	}


	template<class Read> void read( Read &in ) {
		u32 l = in.read<u16>();
		Len = min( l, Size-2 );
		in.readRawV( Str, Len ); Str[Len]=0;
		if( Len != l ) in.skip(l - Len );
	}
protected:
	const CharT* end()		const	{ return &Str[Len];	}
	u32 remaining()	const { return Size - Len;	}
	u32 Len;
	CharT Str[Size];
};

#define DeclareWStr( cName, type, ex ) \
template<u32 Size> class cName : public WStrTmplt<Size,type>, public Serialisable< cName<Size> > { \
public: \
	cName() {}\
	Template1 cName( const T &t) : WStrTmplt(t) {}\
	ex \
};

DeclareWStr( WStr, char,  
	void copyTo( char* const &to ) const override {  memcpy( to, Str, Len); }  
	void copyTo( wchar_t* const &to ) const override { for( u32 i = Len; i--; ) to[i] = (wchar_t)Str[i];	 }  
);
DeclareWStr( WStrW, wchar_t,  
	void copyTo( char* const &to ) const override {  for( u32 i = Len; i--; ) to[i] = (char)Str[i]; }  
	void copyTo( wchar_t* const &to ) const override {  memcpy( to, Str, Len); }  
);

}

/*
#ifndef WSTR_H
#define WSTR_H

#include "CStr.h"
#include "BasicAssert.h"
#include "StrStream.h"

template<u32 Size, class CharT> class WStrTmplt : public StrStream<WStrTmplt<Size,CharT>>, public StringBase<CharT,WStrTmplt<Size,CharT>> {
public:
	WStrTmplt() : Len(0) { Str[0] = 0; }
//	WStrTmplt( const CStrTemplate<CharT> &i ) : Len(i.len()) { memcpy(Str,&(i[0]),Len*sizeof(CharT)); Str[Len] = 0; }

//	template<class Obj1, class Obj2>
	//WStrTmplt( const CondenseHlpr<CharT,Obj1,Obj2> &a ) : Len(a.len()) { Assume(Size>Len); a.copyTo(&Str[0]); Str[Len] = 0; }
	template<class Obj1> WStrTmplt( const StringBase2<CharT,Obj1> &a ) : Len(a.len()) { Assume(Size>Len); a.copyTo(&Str[0]); Str[Len] = 0; }
	
	WStrTmplt( const char *const &c )		: Len(0) { while( Str[Len] = (CharT)c[Len] ) { Len++;  Assume(Len < Size ); } }
	WStrTmplt( const wchar_t *const &w )	: Len(0) { while( Str[Len] = (CharT)w[Len] ) { Len++;  Assume(Len < Size ); } }
	
	const u32& reserved()const { return Size;	}
	u32 len()		const { return Len;		}
	//void copyTo( char* const &to ) const override;

	//operator CStrTemplate<CharT> ()				{ return CStrTemplate<CharT>( Str, Len ) ;}
	operator CharT* ()				{ return Str;		}
	//operator const CharT* ()	const	{ return Str;		}
	CharT* str()					{ return Str;		}
	const CharT* str()		const	{ return Str;		}


	WStrTmplt& operator<<( const CStrTemplate<CharT> &r )	{ 
		Assume( (Len + r.len() ) < Size );
		memcpy( end(), &(r[0]), r.len()*sizeof(CharT) );
		Len += r.len();
		*end() = 0;
		return *this; 
	} 

	template<class T2> WStrTmplt& operator += ( const T2 &r ) { return *this<<r; }

	template<class Ret_t, class InLen_t> 
	WStrTmplt& appendFnc( Ret_t(__stdcall*func)( CharT*, InLen_t) ) {
		func( end(),remaining() );
		fixLen();
		return *this; //todo - ensure whole thing copied
	}
	template<class Ret_t, class In1_t, class InLen_t> 
	WStrTmplt& appendFnc( Ret_t(__stdcall*func)(In1_t, CharT*, InLen_t), const In1_t &in1 ) {
		func(in1,end(),remaining());
		fixLen();
		return *this; //todo - ensure whole thing copied
	}

	WStrTmplt& fileToDir() { 
		Assume(Len != 0);
		for( int i = Len; --i; ) {
			if( isSlash(Str[i]) ) {
				Len = i+1;
				Str[Len] = 0;
				return *this;
			}
		}
		ThrowFail("WStrTmplt::fileToDir() Failed!  Could not find \\ ");
		return *this;
	}
	void fixLen() {
		for( ; Len != Size; Len++ )  if( !Str[Len] ) return;
		ThrowFail("WStrTmplt::fixLen() Failed!  Could not find end of string ");
	}
	void fixLen( const u32 &len ) {
		Assume( len < Size ); Assume( len > Len ); Assume( !Str[len] ); Assume( Str[len-1] );
		Len = len;
	}
	void clear() { Len = 0; Str[0] = 0; }

	CharT* end()						{ return &Str[Len];	}


	template<class Read> void read( Read &in ) {
		u32 l = in.read<u16>();
		Len = min( l, Size-2 );
		in.readRawV( Str, Len ); Str[Len]=0;
		if( Len != l ) in.skip(l - Len );
	}
protected:
	const CharT* end()		const	{ return &Str[Len];	}
	u32 remaining()	const { return Size - Len;	}
	u32 Len;
	CharT Str[Size];
};

#define DeclareWStr( cName, type, ex ) \
template<u32 Size> class cName : public WStrTmplt<Size,type>, public Serialisable< cName<Size> > { \
public: \
	cName() {}\
	Template1 cName( const CharT &t) : WStrTmplt(t) {}\
	ex \
};

DeclareWStr( WStr, char,  
	void copyTo( char* const &to ) const override {  memcpy( to, Str, Len); }  
	void copyTo( wchar_t* const &to ) const override { for( u32 i = Len; i--; ) to[i] = (wchar_t)Str[i];	 }  
);
DeclareWStr( WStrW, wchar_t,  
	void copyTo( char* const &to ) const override {  for( u32 i = Len; i--; ) to[i] = (char)Str[i]; }  
	void copyTo( wchar_t* const &to ) const override {  memcpy( to, Str, Len); }  
);

#endif //WSTR_H*/

#endif //GEM_STRING_WSTR_H