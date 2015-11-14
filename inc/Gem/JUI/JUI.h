#ifndef GEM_JUI_JUI_H
#define GEM_JUI_JUI_H

#include "../Basic.h"
#include "../Org/dList.h"
#include "../Math/vec2.h"

namespace Gem {  namespace JUI {

class JUI_Main;
	
class JUI_Coroutine_Base :  public dListNode<JUI_Coroutine_Base> {
private: friend class JUI_Main;
	virtual void _proc(JUI_Main &cntx) = 0;
};

template< class Base > class JUI_Coroutine  : public JUI_Coroutine_Base {
public:

private:
	void _proc(JUI_Main &cntx) override {
		return static_cast<Base*>(this)->Base::proc(cntx);
	}
}; 

enum Keycode {
	LMouse = 1,

	_Keycode_Last
};
vec2s16 mPos();
bool key(Keycode key);

}	}

#endif //GEM_JUI_JUI_H