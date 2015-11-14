#ifndef GEM_STRING_STRSTREAM_H
#define GEM_STRING_STRSTREAM_H

#include "StrParser.h"
#include "WStr.h"

namespace Gem {

template<u32 Size, class CharT> class WStrTmplt;
template<u32 Size> class WStr;

Template1 class StrStream { };

#define This (*((T*)(&stream)))

	Template1 T& operator<<( StrStream<T>& stream, const char &c )	{ return This<<CStr(&c,1); } //todo optimize
	Template1 T& operator<<( StrStream<T>& stream, const wchar_t &w )	{ return This<<CStrW(&w,1); }
	Template1 T& operator<<( StrStream<T>& stream, const char* const &c )	{ return This<<CStr(c,(u32)strlen(c)); } //todo optimize
	//Template1 T& operator<<( StrStream<T>& stream, const wchar_t* const &w )	{ return This<<CStrW::Initator(,w); }
	Template1 T& operator<<( StrStream<T>& stream, const ptr &p ) { StrParser sp; return This<<sp.parsePtr(p); }
	Template1 T& operator<<( StrStream<T>& stream, const u8  &n ) { StrParser sp; return This<<sp.parseUnsigned(n); }
	Template1 T& operator<<( StrStream<T>& stream, const u16 &n ) { StrParser sp; return This<<sp.parseUnsigned(n); }
	Template1 T& operator<<( StrStream<T>& stream, const u32 &n ) { StrParser sp; return This<<sp.parseUnsigned(n); }
	Template1 T& operator<<( StrStream<T>& stream, const u64 &n ) { StrParser sp; return This<<sp.parseUnsigned(n); }
	//Template1 T& operator<<( StrStream<T>& stream, const s8  &n ) { StrParser sp; return This<<sp.parseSigned(n); }
	Template1 T& operator<<( StrStream<T>& stream, const s16 &n ) { StrParser sp; return This<<sp.parseSigned(n); }
	Template1 T& operator<<( StrStream<T>& stream, const s32 &n ) { StrParser sp; return This<<sp.parseSigned(n); }
	Template1 T& operator<<( StrStream<T>& stream, const s64 &n ) { StrParser sp; return This<<sp.parseSigned(n); }

	Template1 T& operator<<( StrStream<T>& stream, const f32 &n ) { StrParser sp; return This<<sp.parseFloat(n); }

	Template1 T& operator<<( StrStream<T>& stream, const StringUkn &s ) { 
		Assume(s.len() < 2048);
		WStr<2048> ws( s );
		return This<<(CStr)ws;
	}
/*	template<u32 Size, class CharT, class T> 
	T& operator<<( StrStream<T>& stream,
				  const WStrTmplt<Size,CharT> &s ) { 
		This<<(const CStrTemplate<CharT>)s;
	}	
	template<u32 Size, class T>  T& operator<<( StrStream<T>& stream, WStr<Size> &s ) {  This<<(CStr)s; } */

#ifdef _WINDOWS_
	Template1 T& operator<<( StrStream<T>& stream, const DWORD &n ) { StrParser sp; return This<<sp.parseUnsigned(n); }
#endif
#undef This

}

#endif //GEM_STRING_STRSTREAM_H  *note* the s below are purposely below this, incase for instance this file is included before "windows.h", which would lead to problems parsing DWORD's

	/*
#if defined(_WINDEF_) && !defined(STRSTREAM_H__WINDEF_)
#define STRSTREAM_H__WINDEF_

#define This (*((T*)(&stream)))

#undef This

#endif

#if defined(VEC3_H) && !defined(STRSTREAM_H__VEC3_H)
#define STRSTREAM_H__VEC3_H

#define This (*((T*)(&stream)))
Template2 T& operator<<( StrStream<T>& stream, const vec3<T2> &a ) {  return This<<(s32)a.x<<CSTR(" X")<<(s32)a.y<<CSTR(" X")<<(s32)a.z; }
#undef This

#endif */




