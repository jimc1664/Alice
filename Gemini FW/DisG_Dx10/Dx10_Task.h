#ifndef GEM_DIS_DX10_DISTASK_H
#define GEM_DIS_DX10_DISTASK_H

#include "Dx10_Main.h"
#include "Gem/ConCur/TaskQ.h"

namespace Gem {  namespace Dx10 { 


class Dx10_Task_Base : public TaskQ::Task { };
template< class Base > class Dx10_Task : public Dx10_Task_Base {
public:
	Dx10_Task() { Dx10_Main::task( this ); }
	void proc( ptr context ) override {
		Dx10_Main &man = *(Dx10_Main*)context;
		static_cast<Base*>(this)->proc( man );
		static_cast<Base*>(this)->~Base();
	}
};

} }

#endif //GEM_DIS_DX10_DISTASK_H