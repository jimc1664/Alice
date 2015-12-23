#ifndef GEM_STRING_CSTR_H
#define GEM_STRING_CSTR_H

#include "../Basic.h"
//#include <malloc.h>

namespace Gem {

//extern int stringTestX;

Template1 class CStrTemplate;
Template3 struct CondenseHlpr;

struct StringHlpr {
	Template1 static bool isSlash( const T&c ) { return (c == (T)'\\') | (c == (T)'/'); }
};

struct StringUknC {
	virtual u32	len() const = 0;
	virtual void copyTo( char* const &to ) const = 0;
};
struct StringUknW {
	virtual u32	len() const = 0;
	virtual void copyTo( wchar_t* const &to ) const = 0;
};

struct StringUkn : StringUknC, StringUknW  {
	virtual u32	len() const = 0;

	template<class T> void copyTo( T* const & to ) const ; 
	template<> void copyTo(char* const & to) const { ((StringUknC*)this)->copyTo(to); }
	template<> void copyTo(wchar_t* const & to) const { ((StringUknW*)this)->copyTo(to); }
};

template<class CharT, class Upper> struct StringBase_Sub : public StringUkn, public StringHlpr {
#define This (*static_cast<const Upper*>(this))

	u32	len()							const { return This.len(); }

	void copyTo( CharT* const &to ) const { This.copyTo( to ); }  //does not null terminate!

	template<class CharT, class Obj1, class Obj2>
	struct CondenseHlpr : public StringBase_Sub<CharT,CondenseHlpr<CharT,Obj1,Obj2> > {  
		typedef CondenseHlpr<CharT,Obj1,Obj2> ThisT;
		CondenseHlpr( const Obj1 &a, const Obj2 &b ) : A(a), B(b), Len(a.len() + b.len()) {}
	//	template<class Obj3> CondenseHlpr<CharT,ThisT, Obj3> operator+( const Obj3 &b ) const { return CondenseHlpr<CharT,ThisT, Obj3>(*this,b); }

		void copyTo( char* const &to ) const override { A.copyTo( to ); B.copyTo( &to[A.len()] ); } 
		void copyTo( wchar_t* const &to ) const override { A.copyTo( to ); B.copyTo( &to[A.len()] ); } 
		u32 len() const { return Len; }

		const Obj1 &A;
		const Obj2 &B;
		const u32 Len;
	};
	template<class Upper2> CondenseHlpr<CharT,Upper,Upper2> operator+( const StringBase_Sub<CharT,Upper2> &r ) const	{
		return CondenseHlpr<CharT,Upper,Upper2>(upper(),r.upper()); 
	}
	const Upper& upper() const { return This; }
#undef This
}; 

template<class CharT, class Upper> class StringBase : public StringBase_Sub<CharT,Upper> {
public:	
#define This (*static_cast<const Upper*>(this))
	
	operator	const CharT* ()							const { return This.str(); }	
	operator		bool()					const { return This.len() != 0; }	
	const CharT*		str()					const { return This.str(); }

	const CharT& operator[]( const u32 &i ) { return (This.str())[i]; }

	bool isEqual( const CharT *const &r, bool caseSensitive = false ) const {
		auto l = len();
		for( u32 i = 0;; i++ ) {
			if( i == l ) return (r[i] == 0);  //if r[l.len] != 0 then r is longer than l
			if( (!r[i]) || !charAreEqual( (*this)[i], r[i], caseSensitive )  ) return false;
		}
	}
	Template1A bool isEqual( const StringBase<CharT,T1A> &r, bool caseSensitive = false ) const {
		auto l = len();
		if( l != r.len() ) return false;
		for( u32 i = l; i--; ) {
			if( !charAreEqual( (*this)[i], r[i], caseSensitive )  ) return false;
		}
		return true;
	}
	Template1A bool operator == ( const T1A &r ) const { return isEqual(r,true); }
	Template1A bool operator < ( const T1A &r ) const { 
		if( r.len() == len() ) {
			for( int i = len(); i--; ) {
				if((*this)[i] == r[i]) continue;
				else return (*this)[i] < r[i];
			}
			return false;
		} else return len() < r.len();	
	}

	operator const CStrTemplate<CharT>()	const; 


	bool search(const CharT& c, CStrTemplate<CharT> &res) const;
	CStrTemplate<CharT> nextWord() const;
	CStrTemplate<CharT> getPath() const;
	CStrTemplate<CharT> trimmedDeadSpace() const;
	CStrTemplate<CharT> getParam(const CharT &c ) const;

	CStrTemplate<CharT> trimmedStartTo(const CharT* c ) const;
	CStrTemplate<CharT> trimmedTo(const CharT* c ) const;

	const CharT* start() const {  return This.str(); }
	const CharT* end() const {  return This.str() + This.len(); }
private:

	static bool skipWhiteSpace( CharT *&dat, CharT * const end ) {
		for( ; ; dat++ ) {
			if((sizet)dat >= (sizet)end) return false;
			switch( *dat ) {
				case 10 : case 13 : case 9 : case ' ' :
					break; //ignore whitespace
				case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
				case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
				case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
				case'"':case'_':case'#':case'(':case')':case'{':case'}':case'[':case']':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case';':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
					return true;				
			}
		}
		return false;
	}
	static bool findWhiteSpace( CharT *&dat, CharT *const end ) {
		for( ; ; dat++ ) {
			if((sizet)dat >= (sizet)end) return false;
			switch( *dat ) {
				case 10 : case 13 : case 9 : case ' ' :
					return true; //ignore whitespace
				case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
				case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
				case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
				case'"':case'_':case'#':case'(':case')':case'{':case'}':case'[':case']':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case';':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
					break;
			}
		}
		return false;
	}
	static bool find( CharT *&dat, CharT *const end , const CharT &c ) {
		for( ; ; dat++ ) {
			if((sizet)dat >= (sizet)end) return false;
			if(*dat == c) return true;
		}
		return false;
	}
	static bool rvsFind( CharT * const dat, CharT *& end, const CharT &c1, const CharT &c2 ) {
		for( ; ; end-- ) {
			if((sizet)dat >= (sizet)end) return false;
			if(*(end-1) == c1 || *(end-1) == c2 ) return true;
		}
		return false;
	}
	static bool trimEndDeadSpace( CharT * const dat, CharT *& end ) {
		for( ; ; end-- ) {
			if((sizet)dat >= (sizet)end) return false;
			switch( *(end-1) ) {
				case 10 : case 13 : case 9 : case 0: case ' ' :
					break; //ignore whitespace
				case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
				case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
				case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
				case'"':case'_':case'#':case'(':case')':case'{':case'}':case'[':case']':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case';':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
					return true;				
			}
		}
	}

#undef This
};

Template3  bool isEqual( const StringBase<T,T2> &l, const StringBase<T,T3> &r, bool caseSensitive = false ) { return l.isEqual(r,caseSensitive); }
Template2  bool isEqual( const StringBase<T,T2> &l, const T* r, bool caseSensitive = false ) { return l.isEqual(r,caseSensitive); }
Template2  bool isEqual( const T* l, const StringBase<T,T2> &r, bool caseSensitive = false ) { return r.isEqual(l,caseSensitive); }

template<class CharT> class CStrTemplate : public StringBase<CharT,CStrTemplate<CharT>>, public Serialisable<CStrTemplate<CharT>> { //const string
public:
	CStrTemplate() : Str(0), Len(0) {}
	CStrTemplate( const CStrTemplate &a ) : Len(a.len()), Str(a) {}
	CStrTemplate( const CharT *const &c, const u32 &l ) : Len(l), Str(c) {}
	//CStrTemplate( const StringUkn &l ) : Len(l), Str(c) {}

//	CStrTemplate& operator=( const CStrTemplate &a ) { Str = a; Len = a.len(); return *this; }
	bool operator == ( const CStrTemplate& r ) { 
		if( Len != r.len() ) return false;; 
		for( u32 i = Len; i--; ) 
			if( Str[i] != r[i] ) return false;
		return true;
	}

	void copyTo( char* const &to ) const override; // { memcpy( to, Str, Len); } //does not null terminate!
	void copyTo( wchar_t* const &to ) const override;

	u32 len() const override { return Len; }
	const CharT * str() const { return Str; }

	/*template<class Obj2> CondenseHlpr<CharT,CStrTemplate,Obj2> operator+( const StringBase<CharT,Obj2> &b ) const { 
		const Obj2 &bt = *b._this();
		return CondenseHlpr<CharT,CStrTemplate,Obj2>(*this,bt); 
	}*/

	CStrTemplate& decStart( u32 amnt = 1 )	{ Len-=amnt; Str+=amnt; return *this; }
	CStrTemplate& decEnd( u32 amnt = 1 )		{ Len-=amnt; return *this; }

	template< class Write > void write( Write &out ) const {
		out<<(u16) Len;
		out.writeRawV( Str,Len);
	}

protected: friend class CStrTemplate<CharT>;
	const CharT * Str;
	u32 Len;

}; typedef CStrTemplate<char> CStr;  typedef CStrTemplate<wchar_t> CStrW; 


#define CSTR( str ) CStr( str, ArySize(str)-1 )  
#define CSTRW( str ) CStrW( str, ArySize(str)-1 )
#define S( str )  CStr( str, ArySize(str)-1 )  
/*
template<class CharT> bool charAreEqual( const CharT& l, const CharT& r, bool caseSensitive ) {
	if( l == r ) return true;
	if( caseSensitive ) return false;

	const s32 caseDiff = (s32)'a'-(s32)'A';
	s32 diff = (s32)l - (s32)r;
	if( diff == caseDiff ) {		
		return (l >= (CharT)'a') & (l <= (CharT)'z');
	} else if( diff == -caseDiff ) {
		return (r >= (CharT)'a') & (r <= (CharT)'z');
	} else 
		return false;
}
template<class CharT> bool isEqual( const CStrTemplate<CharT> &l, const CharT *const &r, bool caseSensitive = false ) {
	for( u32 i = 0;; i++ ) {
		if( i == l.len() ) return (r[i] == 0);  //if r[l.len] != 0 then r is longer than l
		if( (!r[i]) | !charAreEqual( l[i], r[i], caseSensitive )  ) return false;
	}
}
template<class CharT> bool isEqual( const CStrTemplate<CharT> &l, const CStrTemplate<CharT> &r, bool caseSensitive = false ) {
	u32 len = l.len();
	if( len != r.len() ) return false;
	for( u32 i = len; i--; ) {
		if( !charAreEqual( l.str()[i], r.str()[i], caseSensitive )  ) return false;
	}
	return true;
}
Template2 bool operator == ( const CStrTemplate<CharT>& l, const T2 &r ) { return isEqual(l,r,true); }
*/

template<class CharT, class Upper> StringBase<CharT,Upper>::operator const CStrTemplate<CharT>()	const { return CStrTemplate<CharT>((const CharT*)*this,len()); }

template<class CharT, class Upper> bool StringBase<CharT,Upper>::search(const CharT& c, CStrTemplate<CharT> &res) const {
	CharT* dat = const_cast<CharT*>( str() ), *end = dat+len();
	for( ; (sizet)dat < (sizet)end; dat++ )
		if( *dat == c ) {
			res = CStrTemplate<CharT>(dat,(sizet)end - (sizet)dat );
			return true;
		}
	return false;
}

template<class CharT, class Upper> CStrTemplate<CharT> StringBase<CharT,Upper>::nextWord() const {
	CharT* dat = const_cast<CharT*>( str() ), *end = dat+len();
	if(!skipWhiteSpace(dat, end)) Error("String parsing error - no next word");
	CharT* wrdEnd = dat;
	if( !findWhiteSpace(wrdEnd, end) ) Error("String parsing error - no next word");
	return CStrTemplate<CharT>(dat,(sizet)wrdEnd - (sizet)dat );
}
template<class CharT, class Upper> CStrTemplate<CharT> StringBase<CharT,Upper>::trimmedDeadSpace() const {
	CharT* dat = const_cast<CharT*>( str() ), *end = dat+len();
	trimEndDeadSpace(dat, end);
	return CStrTemplate<CharT>(dat,(sizet)end - (sizet)dat );
}

template<class CharT, class Upper> CStrTemplate<CharT> StringBase<CharT,Upper>::getParam( const CharT& c ) const {
	CharT* dat = const_cast<CharT*>( str() ), *end = dat+len();

	if(!find(dat, end, c )) Error("String parsing error - getParam 1 ");
	CharT* pEnd = ++dat;
	if(!find(pEnd, end, c )) Error("String parsing error - getParam 2 ");
	return CStrTemplate<CharT>(dat,(sizet)pEnd - (sizet)dat );
}

template<class CharT, class Upper> CStrTemplate<CharT> StringBase<CharT,Upper>::trimmedStartTo( const CharT * c ) const {
	CharT* dat = const_cast<CharT*>( str() ), *end = dat+len();
	return CStrTemplate<CharT>( c,(sizet)end - (sizet)c );
}
template<class CharT, class Upper> CStrTemplate<CharT> StringBase<CharT,Upper>::trimmedTo( const CharT * c ) const {
	CharT* dat = const_cast<CharT*>( str() ), *end = dat+len();
	return CStrTemplate<CharT>( dat,(sizet)c - (sizet)dat );
}

template<class CharT, class Upper> CStrTemplate<CharT> StringBase<CharT,Upper>::getPath() const {
	CharT* dat = const_cast<CharT*>( str() ), *end = dat+len();

	rvsFind(dat, end, '/', '\\' );

	return  CStrTemplate<CharT>(dat,(sizet)end - (sizet)dat );
}

/*
struct StackString : public CStr {
	Template1 Inline StackString( void *mem, const CharT& a ) { 
		Str = (char*)mem;
		Len = a.len();
		a.copyTo( (char*)mem );
		((char*)mem )[Len] = 0;
	}
	~StackString() { _freea( (void*)Str ); }
};
#define SSTR( in ) StackString( _malloca( (in).len() ), (in) )
*/

template<class CharT> bool charAreEqual( const CharT& l, const CharT& r, bool caseSensitive ) {
	if( l == r ) return true;
	if( caseSensitive ) return false;

	const s32 caseDiff = (s32)'a'-(s32)'A';
	s32 diff = (s32)l - (s32)r;
	if( diff == caseDiff ) {		
		return (l >= (CharT)'a') && (l <= (CharT)'z');
	} else if( diff == -caseDiff ) {
		return (r >= (CharT)'a') && (r <= (CharT)'z');
	} else 
		return false;
}

template<class CharT> CStrTemplate<CharT> getFileName( const CStrTemplate<CharT> &file ) {  
	Assume( file.len() > 0 );
	const CharT* start = &(file[file.len()-1]);
	for( ; (sizet)start >  (sizet)&(file[0]); start-- ) if( StringHlpr::isSlash( (start-1)[0] ) ) break;
	
	u32 max = (u32)((sizet)file.len() + (sizet)&(file[0]) - (sizet)start );
	u32 len = max;
	for( ; ; ) {
		if(!len) { len = max; break; }
		if( start[--len]  == (CharT)'.' ) break;
	}
	return CStrTemplate<CharT>( start, len );
}

template<class CharT> CStrTemplate<CharT> getFileType( const CStrTemplate<CharT> &file ) {  
	Assume( file.len() );
	for( u32 start = file.len()-1; ; ) {
		if( !start ) 
			return CStrTemplate<CharT>( 0,0 );
		if( file.str()[--start] == (CharT)'.' )
			return CStrTemplate<CharT>( &file.str()[start+1], file.len()-start-1 );
	}
}
/*
inline void printUnhandledChar( char* add, const char &c ) {
	char t[2] = { c, 0 };	
	printf("%s unrecognised character! [ %s ] [ %d ] \n",add, t, t[0] ); 
	error("");
}
inline void printCStr( const CStr &src ) {
	if( !src ) { printf( "empty string !\n"); return; }

	char* a = new char[src.len()+1];
	memcpy(a,src,src.len());
	a[src.len()] = 0;
	printf( "Cstr - <| %s |> \n", a);
}
inline void printCStr( char* add, const CStr &src ) {
	printf("%s",add); printCStr(src);
}
inline CStr fromFirstActionChar( const CStr &src ) {
	const char *c = src;
	//const char* lc = &src[src.len()-1];
	for( u32 i = src.len(); i--; c++ ) {
		switch( *c ) {
			case 0  : 
				error("0 ?? ");
			case 10 : case 13 : case 9 : case ' ' : //return
				break; //ignore whitespace
			case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
				error( "start with number? " );
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
			case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
			case'_':case'#':case'(':case'{':
				return CStr( c, i+1 );
			default:
				char t[2] = { *c, 0 };	
				printf("fromFirstActionChar:: unrecognised character! [  %s ] [ %d ] \n", t, *c ); 
				break;
		}
	}
	return CStr(0,0);
}
inline CStr toFirstSemiColon( const CStr &src ) {
	const char *c = src;
	//const char *lc = &src[src.len()-1];
	for( u32 i = src.len(); i--; c++ ) {
		switch( *c ) {
			case 0  : error("0 ?? ");
			case';' : return CStr( src, src.len()-i );
		}
	}
	return CStr(0,0);
}	
inline CStr fromFirstNonWhiteSpace( const CStr &src ) {
	const char *c = src;
	//const char* lc = &src[src.len()-1];
	for( u32 i = src.len(); i--; c++ ) {
		switch( *c ) {
			case 10 : case 13 : case 9 : case ' ' :
				break; //ignore whitespace
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
			case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
			case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			case'"':case'_':case'#':case'(':case')':case'{':case'}':case'[':case']':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case';':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
				return CStr( c, i+1 );
			default: printUnhandledChar( "fromFirstNonWhiteSpace:: ",*c); break;
		}
	}
	return CStr(0,0);
}
inline CStr trimWhiteSpace( const CStr &src ) {
	CStr ret = fromFirstNonWhiteSpace(src);
	for( ;ret.len(); ret.decEnd() ) {
		const char &c = ret[ret.len()-1];
		switch(c ) {
			case 10 : case 13 : case 9 : case ' ' :
				break; //ignore whitespace
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
			case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
			case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			case'"':case'_':case'#':case'(':case')':case'{':case'}':case'[':case']':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case';':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
				return ret;
			default: printUnhandledChar( "fromFirstNonWhiteSpace:: ",c); break;
		}
	}
	return ret;		
}			
inline CStr extractFirstWord( CStr &src ) {
	const CStr orig = src;
	for( ;src.len(); src.decStart() ) {
		const char &next = *src;
		switch( next ) {	
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
			case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
			case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			case'_':case'#':
				break;
			case'"':case'(':case')':case'{':case'}':case'[':case']':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case';':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
			case 0  : case 10 : case 13 : case 9 : case ' ': 
				return CStr( orig, orig.len()-src.len() );
			default: printUnhandledChar("extractFirstWord::", next ); break;
		}
	}
	return orig;
}
inline CStr extractString( CStr &src ) {
	const CStr orig = src;
	src.decStart();
	for( ;src.len(); ) {
		const char &next = *src; src.decStart();
		switch( next ) {	
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
			case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
			case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			case'_':case'#':
			case'(':case')':case'{':case'}':case'[':case']':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case';':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
			case 9 : case ' ': 
				break;
			case 0  : case 10 : case 13 :
				error("extractString  following \" not found" );
			case'"':
				return CStr( orig, orig.len()-src.len() );
			default: printUnhandledChar("extractString::", next ); break;
		}
	}
	return orig;
}
template< char cmp > CStr extractToMatching( CStr &src ) {
	const CStr orig = src;
	for( ;src.len(); ) {
		const char &c = *src; src.decStart();
		switch( c ) {	
//				case cmp :
			case')':
				return CStr( orig, orig.len()-src.len()-1 );
				
			case'(':case'{':case'}':case'[':case']':
				break;
				
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
			case'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z':
			case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			case 0  : case 10 : case 13 : case 9 : case ' ' :
			case'"':case'_':case'+':case'-':case'*':case'/':case'\\':case'=':case':':case'.':case'>':case'<':case',':case'%':case'&':case'^':case'|':case'!':case'?':
				break;
			default: printUnhandledChar( "extractToMatching:: ",c); break;
		}
	}
	return orig;
}
typedef CStr (*StrManip)( const CStr &src );
inline CStr extractTo_From( CStr &src, StrManip a, StrManip b ) { 
	CStr ret = b(a(src));
	//const char* nStr = ret.last()+1;
	//src.len() -= nStr - src; src = nStr; 
	src.decStart((u32)ret.str() - (u32)src.str() +ret.len() );
	return ret; 
}
inline u32 strTo_u32( const CStr &src ) {
		
	const char* c = &src[src.len()-1];
	u32 ret = 0; u32 mul = 1;
	for( u32 i = src.len(); i--; mul*=10, c-- ) {
		ret += (u32)(*c-'0') *mul;
	}
	return ret;
}
*/
}

namespace std { template<class _Elem, class _Traits> class basic_ostream; };
template<class Elem, class Traits, class Upper, class T> std::basic_ostream<Elem,Traits>& operator<<( std::basic_ostream<Elem, Traits>& stream, const Gem::StringBase<T,Upper> &str ) { return stream<<(const T*)str; }


#endif //GEM_STRING_CSTR_H