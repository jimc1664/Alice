#include "stdafx.h"

#include "Gem/String/DStr.h"


template<> void CStrTemplate<char>::copyTo( char* const &to ) const { memcpy( to, Str, Len); }
template<> void CStrTemplate<char>::copyTo( wchar_t* const &to ) const {
	for( u32 i = Len; i--; ) to[i] = (wchar_t)Str[i];	
}

template<> void CStrTemplate<wchar_t>::copyTo( wchar_t* const &to ) const { memcpy( to, Str, Len); }
template<> void CStrTemplate<wchar_t>::copyTo( char* const &to ) const {
	for( u32 i = Len; i--; ) to[i] = (char)Str[i];	
}


template<> void DStrTemplate<char>::copyTo( char* const &to ) const { memcpy( to, str(), len()); }
template<> void DStrTemplate<char>::copyTo( wchar_t* const &to ) const {
	auto s = str();
	for( u32 i = len(); i--; ) to[i] = (wchar_t)s[i];	
}

template<> void DStrTemplate<wchar_t>::copyTo( wchar_t* const &to ) const { memcpy( to, str(), len()); }
template<> void DStrTemplate<wchar_t>::copyTo( char* const &to ) const {
	auto s = str();
	for( u32 i = len(); i--; ) to[i] = (char)s[i];	
}


//int Gem::stringTestX = 0;