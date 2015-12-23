#include "../Gem/stdafx.h"

#include "Gem/Dis/DisMain.h"


namespace Gem { namespace Dis {






}	}

#include "Gem/Org/TemplateCmpnt.h"




template<  typename Comp > struct AlphabetBase2 : public Comp {
	virtual ~AlphabetBase2() {}

	template<typename SubCmpnt> typename SubCmpnt::Prm prmFor() { return PrmHelper<typename SubCmpnt::Prm>::cast(this); }
};
template< typename Typ1, typename Typ2 = Nothing, typename Typ3 = Nothing, typename Typ4 = Nothing, typename Typ5 = Nothing > class AlphabetBase;
template< typename Typ1> class AlphabetBase<Typ1, Nothing, Nothing, Nothing, Nothing> : public AlphabetBase2<  typename Typ1::Req::Comp > { };
//
/*
struct A : public TCmpnt<A> { 

	int a; 
};

class B : public TCmpnt<B, A> { 
public:
	virtual void test( ) = 0;  
protected: 
	void test( Prm &p);
public:
	int b; 


	template<typename Cmpnt, class Base> class TopIF : public Cmpnt { 
		void test( ) override { 
			return 	Cmpnt::test( PrmHelper<typename Cmpnt::Prm>::cast(  static_cast<Base*>(this) ) ); 
		} 
	}; 
};


template<typename Base> struct TopIFFor<Base, B> : public B::TopIF<B,Base>{};

	 
	 



//template< typename Typ1, typename Typ2> class AlphabetBase<Typ1, Typ2, Nothing, Nothing, Nothing> : public AlphabetBase2<  typename dpndnC_Join< typename Typ1::Req, typename Typ2::Req>::R::Comp > {};

class BObj : public  AlphabetBase<B> {

};
*/

struct A { int a; };
struct B { int b; };
struct C { int c; };

typedef dpndnC_Join< dpndnC<A>, dpndnC<B> >::R dpndnC_AB;
typedef dpndnC_Join< dpndnC_AB, dpndnC<C> >::R dpndnC_ABC;
typedef dpndnC_Join< dpndnC_AB, dpndnC<A> >::R dpndnC_ABA;

void test_foo() {

	dpndnC_AB::Comp ab; ab.a = ab.b = 0;
	dpndnC_ABC::Comp abc; abc.c = abc.b = abc.a = 0;
	dpndnC_ABA::Comp aba; aba.a = 0;

	/*auto b = new BObj;
	B*  b2 = b;

	b2->test(); */
	
}	
