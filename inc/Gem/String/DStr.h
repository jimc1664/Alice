#ifndef GEM_STRING_DSTR_H
#define GEM_STRING_DSTR_H

#include "../Org/ary.h"
#include "CStr.h"

/*
namespace File {
	Template1 struct ReadIF;
};*/

template<class CharT> class DStrTemplate  : public StringBase<CharT,DStrTemplate<CharT>>, public Serialisable<DStrTemplate<CharT>> {
public:
	DStrTemplate() {}
	DStrTemplate(const Reserve &r) { Data.addMul(r.Amnt); }

	Template1A DStrTemplate( CpySpcf<DStrTemplate,T1A> &a )  : Data( a.copy(a->Data) ) {}
	DStrTemplate( DStrTemplate &&a )  : Data( Steal(a.Data) ) {}

	Template1A void init( const StringBase_Sub<CharT,T1A> &a) { 
		Assume(Data.count() == 0); 
		Data.addMul(a.len()+1); a.copyTo(str()); str()[a.len()] = 0; 
	}
	void init( const StringUknC &a ) { 
		Assume(Data.count() == 0);
		u32 l = a.len();
		Data.addMul(l+1); a.copyTo(str()); str()[l] = 0; 
	}
	Template1A DStrTemplate( const StringBase_Sub<CharT,T1A> &a ) { init(a); }
	explicit DStrTemplate( const StringUknC &a ) { init(a); }
	Template1A DStrTemplate& operator=( const T1A &a) { Data.clear(); init(a); return *this; }


//	DStrTemplate( const CStr & a) : Data(a.len()+1,0) { Data.addMul(a.len()+1); a.copyTo(str()); str()[a.len()] = 0; }

//	template<class T2,class T3> DStrTemplate( const CondenseHlpr<T2,T3> & a) : Data(a.len()+1,0) { Data.addMul(a.len()+1); a.copyTo(str()); str()[a.len()] = 0; }
//	DStrTemplate& operator=( const CStr & a) { Data.clear(); Data.addMul(a.len()+1); a.copyTo(str()); str()[a.len()] = 0; return *this; }

	~DStrTemplate() { }

	operator CharT*() { return Data.raw(); }
	//operator CStrTemplate<CharT>() const { return CStrTemplate<CharT>( len(), str() ); }
	//operator const CharT*() const { return &Str[0]; }

	void copyTo( char* const &to ) const override; // { memcpy( to, Str, Len); } //does not null terminate!
	void copyTo( wchar_t* const &to ) const override;

	u32 len() const { return Data.count()-1; } //-1 as this is null terminated

	CharT* str() { return Data.raw(); }
	const CharT* str() const { return Data.raw(); }

	Template1A void write( T1A &out ) const {
		u16 l = len();
		out<<l;
		if( l ) 
			out.writeRawV( Data.raw(), l );
	}
	/*Template1A void read( File::ReadIF<T1A> &in ) {
		Data.clear();
		_read(in);
	}
	Template1A DStrTemplate( File::ReadIF<T1A> &in ) {
		_read(in);
	}*/


private: friend class DStrTemplate<CharT>;

	/*Template1A void _read( File::ReadIF<T1A> &in ) {
		u16 l; in>>l;
		if( l ) {
			Data.addMul( l+1 );
			in.readRawV( Data.raw(), l );
			Data.raw()[l] = 0;
		}
	}*/
	struct UnsafeAry : public ary<CharT,Ctor::Simple> {
		UnsafeAry() {}
		Template1A UnsafeAry( CpySpcf<UnsafeAry,T1A> &a )  : ary( a.sub<ary<CharT,Ctor::Simple>>() ) {}
		CharT* raw() { return Data; }
		const CharT* raw() const { return Data; }
		setUncopyable(UnsafeAry);
	} Data;
	setUncopyable(DStrTemplate);
};typedef DStrTemplate<char> DStr;  typedef DStrTemplate<wchar_t> DStrW; 

/*
template<class T> bool isEqual( const DStrTemplate<T> &l, const CStrTemplate<T> &r, bool caseSensitive = false ) {
	u32 len = l.len();
	if( len != r.len() ) return false;
	for( u32 i = len; i--; ) {
		if( !charAreEqual( l.str()[i], r.str()[i], caseSensitive )  ) return false;
	}
	return true;
}
template<class T> bool isEqual( const DStrTemplate<T> &l, const DStrTemplate<T> &r, bool caseSensitive = false ) {
	u32 len = l.len();
	if( len != r.len() ) return false;
	for( u32 i = len; i--; ) {
		if( !charAreEqual( l.str()[i], r.str()[i], caseSensitive )  ) return false;
	}
	return true;
}
*/
#endif //GEM_STRING_DSTR_H