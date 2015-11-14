#ifndef STRINGTemplate_H
#define STRINGTemplate_H

#include "BasicDefines.h"
#include "BasicTypes.h"

#include "AllocHeap.h"
#include "mBlk.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace File { class cRead; };

template<class T, class Alloc = AllocHeap>
class string  {    //alotta stuff can be copied from "array.h" but i won't inherit directly as alot of it needs modified slightly to account for the fact that this is null terminated
	//in string allocated allways has to be > Size for null termination
	public:
	
	string() : Size(0), Allocated(0), Data( ) {}
	string( const string &other ) : Data( ) { 
		Size = other.Size;
		Allocated = Size+1;
		Data.init(Allocated);
		if(Size) memcpy(Data,other.Data,sizeof(T)*Size);
		Data[Size] = (T)0;
	}
	string( const T* data ) : Size(0), Allocated(0), Data( )  {
		if(!data) return;
		while( data[Size] ) { Size++; }
		Allocated = Size + 1;
		Data.init(Allocated);
		memcpy( Data, data, sizeof(T)*Size );
		Data[Size] = (T)0;
	}
	u32 NumToString( u32 number, T* buff ) {

		u32 i = 15;

		if (!number) {
			buff[15] = '0';
			return 15;
		}
		do{
			buff[--i] = ('0' + (T)(number % 10));
			number /= 10;
		} while(number && i);

		return i;
	}
	explicit string( u32 number) : Data( ) {

		T buff[16]={0};
		u32 i = NumToString( number, buff );
		Size = 16-i;
		Data.init(Allocated = Size+1);
		memcpy(&Data[0],&buff[i],Size);
		Data[Size] = (T)0;

	}
	explicit string( s32 number) : Data( ) {

		T buff[16]={0};
		if(number<0) {
			u32 i = NumToString( (u32)abs(number), buff );
			Size = 17-i;
			Data.Allocate(Allocated = Size+1);
			memcpy(&Data[1],&buff[i],Size-1);
			Data[0] = '-';
		} else {
			u32 i = NumToString( number, buff );
			Size = 16-i;
			Data.Allocate(Allocated = Size+1);
			memcpy(&Data[0],&buff[i],Size);
		}
		Data[Size] = (T)0;
	}
	template<class T2> explicit string( T2* ptr ) : Data( ) {
		const u32 tl = sizeof(size_t)*2;
		size_t number = (size_t)ptr;
		struct{
			union {
				s32 i;
				s8 c[4];
			};
		}t; t.c[0] = t.c[1] = t.c[2] = t.c[3] = '0';
		T buff[tl]={'0'}; memset(buff,t.i,tl);
		u32 i = tl-1;
		while(number) {
			u32 digit = (number % 16);
			if( digit > 9 )
				buff[i--] = ('A' + (T)(digit-10));
			else
				buff[i--] = ('0' + (T)digit);
			number /= 16;
		}

		Size = sizeof(size_t)*2+2;
		Data.init(Allocated = Size+1);
		memcpy(&Data[2],buff,Size);
		Data[Size] = (T)0;
		Data[0] = '0';
		Data[1] = 'x';
	}
	explicit string(const double number) :  size(0), Allocated(0), Data( ) {
		char t[255]; _gcvt_s(t,number,5); *this = t;
	}
	explicit string( File::cRead* in ); //reads whole file
	~string() { if(Data) Data.deallocate(); }

	T& GetC( const u32 &index ) { return Data[index]; }
	const T&  GetC( const u32 &index ) const { return Data[index]; }
 
	operator const T* () const { return &GetC(0); }
	u32 size() const { return Size; };
	const bool operator == ( const string<T> &other ) const { 
		if( Size != other.Size ) return false;

		for( u32 t = 0; t < Size; t++ )		
			if( Data[t] != other.Data[t] ) return false;
		
		return true;
	}
	const bool operator == ( const T* &other ) const {  return *this == string<T>(other); }
	const bool Contains( const string<T> &other, const u32 &at ) const { 
		
		if( (Size<at)||(Size-at < other.Size) ) return false;

		for( u32 t = 0; t < other.Size; t++ )		
			if( Data[t+at] != other.Data[t] ) return false;
		
		return true;
	}

	template<class T2> const bool operator != ( const T2 &other ) const { return !(*this == other); }
	void Extend(const u32 &add ) { 
		Assert( add != 0, "add == 0? " );
		Data.Reallocate(Allocated+=add);	
	}		
	void PreAlloc( const u32 needed, const u32 add ) { //checks theres enough space for "needed" elements if not it increments by add
		Assert( needed <= add,"Ehh?" );
		if( Size+needed > Allocated ) Extend(Size+add-Allocated);
	}
	void PreAlloc( const u32 needed) { PreAlloc(needed,needed);}

	template<class T2> string& operator+=( const T2 &item )  { return *this += stringC(item); }
	string& operator += ( const string<T> &other ) { 
		if( Allocated-Size <= other.Size)
			Extend(1024);

		memcpy( &Data[Size], other.Data, sizeof(T)*other.Size );
		Size += other.Size;
		Data[Size] = (T)0;

		return *this; 
	}
	string& operator += ( T c ) { 
		if( Allocated-Size <= 1 )
			Extend(1);

		Data[Size] = c;
		Size ++;
		Data[Size] = (T)0;

		return *this; 
	}

	template<class T2> string& operator<<( const T2 &item )  { return *this += item; }
	string operator + ( const string<T> &other ) const { 
		string<T> temp; temp.Extend(Size+other.Size+1);  //todo optimize
		temp+= *this; temp+=other; return temp; 
	}
	const string& operator = ( const string<T> &other ) { 
		
		Size = other.Size;
		if(Size >= Allocated ) {
			Allocated = Size+1;
			Data.Reallocate(Allocated);
		}

		memcpy( Data, other.Data, sizeof(T)*Size);
		Data[Size] = (T)0;

		return *this; 
	}
	const T& Insert(const T& element, const u32 &ind ) {
		if( ind >= Size ) { *this += element; return Data[ind]; }
		if( ++Size >= Allocated) {
			T e(element);  // make a copy just in case element is inside Data[]
			Extend(1);
			for( u32 i = Size; i > ind; i-- ) Data[i] = Data[i-1];
			return Data[ind] = e;
		}
		else {
			for( u32 i = Size; i > ind; i-- ) Data[i] = Data[i-1];
			return Data[ind] = element;
		}
	}
	u32 find( const T& element ) const { 
		for( u32 i = 0; i != Size; i ++ )
			if( Data[i] == element ) return i;
		return FINDFAILED;	
	}	
	u32 find( const T& element, const u32 &start ) const { 
		for( u32 i = start; i < Size; i ++ )
			if( Data[i] == element ) return i;
		return FINDFAILED;	
	}
	void RemoveAll() { Size = 0; if(Allocated) Data[0] = 0; }
	void Remove( const u32 &at, const u32 &amount = 1 ) {
		if( amount >= Size ) { RemoveAll(); return; }
		u32 end = 1+ (Size-= amount);
		for( u32 i = at; i != end; i++ )
			Data[i] = Data[i+amount];
	}
	string Section( const u32 &start, const u32 &end) const {
		string ret; ret.Extend(end-start+2);  //todo allocate
		for( u32 i = start; i <= end; i++ ) ret.Data[ret.Size++] = Data[i];
		ret.Data[ret.Size] = 0;
		return ret;
	}
	string& LowerCase() {
		for( u32 c = 0; c < Size; c++ )
			if( (Data[c] >= (T)'A') && (Data[c] <= (T)'Z') )
				Data[c] += ( (T)'a' - (T)'A' );
		return *this;
	}
	string& UpperCase() {
		for( u32 c = 0; c < Size; c++ )
			if( (Data[c] >= (T)'a') && (Data[c] <= (T)'z') )
				Data[c] += ( (T)'A' - (T)'a' );
		return *this;
	}
	template< class FileIF > size_t Read( FileIF* fi ) {
		
		size_t ret = File::Read(Size,fi);
		Data.Reallocate(Allocated = Size+1);
		ret += fi->Process(Data,sizeof(T)*Size);
		Data[Size] = (T)0;

		Assert(ret == sizeof(Size)+sizeof(T)*Size, "Unexpected EoF" );
		return ret;
	}
	template< class FileIF > size_t Write( FileIF* fi ) {
		size_t ret = File::Write(Size,fi);
		ret += fi->Process(&Data[0],sizeof(T)*Size);  
		Assert( ret == Size*sizeof(T)+sizeof(Size), "Failed write" );
		return ret;
	}
private:
	u32 Size, Allocated;
	mBlk<T,MB::Manual,Alloc> Data;
};

typedef string<char> stringC;
typedef string<wchar_t> stringW;

typedef string<char> Create;
typedef string<char> Get;
typedef string<char> Find;


namespace File {
template< class T, class FileIF > size_t Write( string<T> &a, FileIF* fi ) { return a.Write(fi);}
template< class T, class FileIF > size_t Read( string<T> &a, FileIF* fi ) { return a.Read(fi);}
};



#endif //STRINGTemplate_H
