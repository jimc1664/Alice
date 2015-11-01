#ifndef GEM_DIS_DX11_DISTASK_H
#define GEM_DIS_DX11_DISTASK_H

#include "Dx11_Main.h"
#include "Gem/ConCur/TaskQ.h"

namespace Gem {  namespace Dx11 { 


class Dx11_Task_Base : public TaskQ::Task { };
template< class Base > class Dx11_Task : public Dx11_Task_Base {
public:
	Dx11_Task() { Dx11_Main::task( this ); }
	void proc( ptr context ) override {
		Dx11_Main &man = *(Dx11_Main*)context;
		static_cast<Base*>(this)->proc( man );
		static_cast<Base*>(this)->~Base();
	}
};

} }

#endif //GEM_DIS_DX11_DISTASK_H