#ifndef GEM_ORG_OBJREFARY_H
#define GEM_ORG_OBJREFARY_H

#include "Ary.h"

namespace Gem {
	
template<class ObjP > class ObjRefAry : protected ary<ObjP> {
public:
	ObjP sf_get( const u32 &i ) {
		if(i < Count) return Data[i];
		else return 0;
	}
	u32 add( ObjP a ) {
		u32 i;
		if( FreeInds.count() ) {
			i = FreeInds.pop();			
		} else {
			i = pushI(a);
		}
		Data[i] = a;
		return i;
	}
	void free( u32 i ) {
		ObjP &p = (*this)[i];
		Assume(p != null);
		p = null;
		if( i != Count-1 ) {
			FreeInds.push(i);
		} else {
			Count--;
		}
	}
private:
	ary<u32> FreeInds;
};

}

#endif //GEM_ORG_OBJREFARY_H