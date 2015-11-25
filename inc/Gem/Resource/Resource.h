#ifndef GEM_RESOURCE_RESOURCE_H
#define GEM_RESOURCE_RESOURCE_H

#include "../Basic.h"
#include "../String/DStr.h"
#include "../ConCur/CritSec.h"
namespace Gem { 

template<class Base> class Resource {
protected:

//	Base* get(
};


template<class ResTyp> class ResourceDat  {
protected:
	volatile ResTyp* Hndl;
	CritSec CS; //todo - don't really need CS per resource handle... more elegant solution trivial

	template<typename ResDat> ResTyp* get( ResDat &rd  ) {
		if(Hndl) return (ResTyp*)Hndl;
		CritSec::Handle h(CS);
		if(Hndl) return (ResTyp*)Hndl;
		else 
			return (ResTyp*)(Hndl = rd.prep() );
	}
public:
	ResourceDat() { 	Hndl = 0; }
};


template<typename ResTyp, typename Typ1, typename Typ2 = Nothing, typename Typ3 = Nothing> class Res_From_T;
template<typename ResTyp, typename Typ1> class Res_From_T<ResTyp, Typ1, Nothing, Nothing> : public ResourceDat<ResTyp> {
	Typ1 A;
friend class ResourceDat<ResTyp>;
	ResTyp* prep() { return ResTyp::prep(A); }
public:
	Template1 Res_From_T( T &&a ) : A(a) {}
	operator ResTyp& () { return *get(*this);  }
};
template<typename ResTyp, typename Typ1, typename Typ2> class Res_From_T<ResTyp, Typ1, Typ2, Nothing> : public ResourceDat<ResTyp> {
	Typ1 A; Typ2 B;
friend class ResourceDat<ResTyp>;
	ResTyp* prep() { return ResTyp::prep(A, B); }
public:
	Template2 Res_From_T( T &&a, T2 &&b) : A(a), B(b) {}
	operator ResTyp& () { return *get(*this);  }
};

}

#endif //GEM_RESOURCE_RESOURCE_H 