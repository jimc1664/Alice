#ifndef GEM_JUI_INPUTGROUP_H
#define GEM_JUI_INPUTGROUP_H

#include "JUI.h"
#include "../Basic/Tuple.h"
//#include "../Org/dList.h"

namespace Gem {  namespace JUI {


template< int Dim>
class Axis_T : public JUI_Coroutine<Axis_T<Dim>> {
public:
	Axis_T() {
		for (int i = Dim; i--;)
			Value[i] = 0;
	}
	char Key[Dim*2];

	void activate();
	void deactivate();

	tupleD<Dim,float> value() { return tupleD<Dim, float>( (float*)&Value[0]); }

private: friend class JUI_Coroutine<Axis_T>;

	void proc(JUI_Main &cntx);
	volatile float Value[Dim];
};
typedef Axis_T<1> Axis;
typedef Axis_T<2> AxisDual;


class Toggle : public JUI_Coroutine<Toggle> { //todo - this is wrong - rebuild callback system
public:
	Toggle():Value(0), LKey(0) { }
	char Key;
	void activate();
	void deactivate();

	volatile bool Value;
private: friend class JUI_Coroutine<Toggle>;
	void proc(JUI_Main &cntx);
	bool LKey;
};

class InputGroup  { //:  public dListNode<InputGroup> {
public:
	InputGroup(  ) {}

protected:

private:

};

}	}

#endif //GEM_JUI_INPUTGROUP_H