#ifndef GEM_STRING_STRPARSER_H
#define GEM_STRING_STRPARSER_H

#include "CStr.h"
#include <stdio.h> //for sprintf_s

namespace Gem {

class StrParser { //note after string is parsed it may not be at the start of the Buff, hence why CStr is returned from parse
public:

	CStr parsePtr( const void *const &p ) {
		//Buff[BuffSize-1] = 0;

		u32 size = sizeof(size_t)*2;  //4byte point = 0x--------  :: 8 dashs
		size_t num = (size_t)p;

		Buff[0] = '0';
		Buff[1] = 'x';
		Buff[size+2] = 0;
		struct{
			union {
				s32 i;
				char c[4];
			};
		}t; t.c[0] = t.c[1] = t.c[2] = t.c[3] = '0';
		memset(&Buff[2],t.i,size/4);

		u32 i = size-1+2;
		while(num) {
			u32 digit = (num % 16);
			if( digit > 9 ) //todo  optimize ( bitwise and  + table )
				Buff[i--] = ('A' + (char)(digit-10));
			else
				Buff[i--] = ('0' + (char)digit);
			num /= 16;
		}
		return CStr( &Buff[0], size );
	}
	Template1 CStr parseUnsigned( const T &number) { return parseNumber( number ); }
	Template1 CStr parseSigned( const T &number) {
		if(number<0) {
			CStr t = parseNumber( abs(number) );
			Assume( t.len() < 30); 

			u32 i = BuffSize-2-t.len();

			Buff[i] = '-';
			return CStr( &Buff[i], t.len()+1 );
		} else
			return parseNumber( number ); 
	}
	CStr parseFloat( const f32 &number) { //todo ...
		sprintf_s( Buff,32,"%f",number);
		auto len = (u32)strlen(Buff);
		Assume( len < BuffSize-2 );
		return CStr( &Buff[0], len );
	}
private:
	Template1 CStr parseNumber( T number ) { //number is +ve, but not nesscarily unsigned

		if (!number) {
			Buff[0] = '0';
			Buff[1] = 0;
			return CStr( &Buff[0], 1 );
		}
		u32 i = BuffSize-1;
		Buff[i] = 0;
		do{
			Assume(i!=0); //i should never reach 0
			Buff[--i] = ('0' + (char)(number % 10));
			number /= 10;
		} while(number);

		return CStr( &Buff[i], BuffSize-1-i );
	}
	static const u32 BuffSize = 32;
	char Buff[BuffSize];
};

} 

#endif //GEM_STRING_STRPARSER_H
