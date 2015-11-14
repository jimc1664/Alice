#ifndef GEM_BASIC_CALLBACK_H
#define GEM_BASIC_CALLBACK_H

#include "..\Basic.h"

namespace Gem {

template< class T1 > class Callback {
	template< class WT > class CallbackSub : public Callback {
	public:
		CallbackSub( WT  fnc ) : Fnc(fnc) {}
		void proc( T1 &a )  override  {
			Fnc(a);
		}
		WT Fnc;
		virtual ~CallbackSub() {}
	};
public:
	virtual void proc(T1 &a) = 0;
	virtual ~Callback() {}

	template< class WT > static Callback* wrap( WT  fnc ) {
		
		return new CallbackSub<WT>(fnc);
	}
}; //todo - static version

}


#endif //GEM_BASIC_CALLBACK_H