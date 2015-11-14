#ifndef GEM_DIS_RENDERSTATE_H
#define GEM_DIS_RENDERSTATE_H

#include "DisMain.h"
#include "DrawList.h"
#include "Gem/ConCur/TaskQ.h"
#include "Gem/Math/Vec2.h"
#include "Gem/Org/TemplateCmpnt.h"

namespace Gem {  namespace Dis {
	
class RenderState { };
Template1 class RenderState_T : public RenderState {
public:
	void apply() {
		T rs(*(T*)this);
		TaskSchedule(DisMain, rs.proc(cntx); );
	}

	
	Template1 class DrawMsg_RS : public DrawMsg_T<DrawMsg_RS<T> >, public T {
	friend class msgQ<DrawMsg>;
		DrawMsg_RS( const T* a ) : T(*a) {}
	friend class DrawMsg_TSpec;
		void proc(RenderingCntx &rc) { T::proc(renderingCntx_DM(rc)); }
	};

	void addTo(Dis::DrawList & dl) {
		dl.add<DrawMsg_RS<T>>((T*)this);
	}
};

class RenderState_2d : public RenderState_T<RenderState_2d> {
public:
	void proc( DisMain &dm ) const;
};
class RenderState_3d : public RenderState_T<RenderState_3d> {
public:
	void proc(DisMain &dm) const;
};


template< class Base, typename Typ1, typename Typ2= Nothing, typename Typ3 = Nothing, typename Typ4 = Nothing, typename Typ5 = Nothing > struct RS_Proc;
template< class Base, typename Typ1> struct RS_Proc<Base, Typ1, Nothing, Nothing, Nothing, Nothing> {
	typedef const Typ1& Prm;
	typedef typename dpndnC<Typ1> Req;
	typedef RS_Proc ProcBase;
	Template1 void proc_T(DisMain &dm, const T* _this) const { static_cast<const Base*>(this)->proc(dm, Prm(*static_cast<const Typ1*>(_this))); }
};
template< class Base, typename Typ1, typename Typ2> struct RS_Proc<Base, Typ1, Typ2, Nothing, Nothing, Nothing> {
	typedef cRefTuple<Typ1, Typ2> Prm;
	typedef typename dpndnC<Typ1, Typ2> Req;
	//typedef typename dpndnC<Typ1, Typ2>::Comp Req;
	typedef RS_Proc ProcBase;
	Template1 void proc_T(DisMain &dm, const T* _this) const { static_cast<const Base*>(this)->proc(dm, Prm(*static_cast<const Typ1*>(_this), *static_cast<const Typ2*>(_this))); }
};

namespace RS_D {
	struct _ScreenSize {
		vec2u ScrnSz;
	};
	struct _Ortho {
		float Height;
	};
	struct _Projection {
		float FoV, Near, Far;
	};
}

class ViewPort : public  RS_Proc<ViewPort, RS_D::_ScreenSize> {
private: friend struct ProcBase;
	void proc(DisMain &dm, const Prm &d) const;
};
class Ortho : public  RS_Proc<Ortho, RS_D::_ScreenSize, RS_D::_Ortho> {
private: friend struct ProcBase;
	void proc(DisMain &dm, const Prm &d) const;
};
class Projection : public  RS_Proc<Projection, RS_D::_ScreenSize, RS_D::_Projection> {
private: friend struct ProcBase;
	void proc(DisMain &dm, const Prm &d) const;
};

template< typename Typ1, typename Typ2 = Nothing, typename Typ3 = Nothing, typename Typ4 = Nothing, typename Typ5 = Nothing > struct RS_T;
template< typename Typ1> struct RS_T<Typ1, Nothing, Nothing, Nothing, Nothing> : public RenderState_T<RS_T<Typ1>>, public Typ1, public  Typ1::Req::Comp{
public:
	void proc(DisMain &dm) const { Typ1::proc_T(dm, this); }
};
template< typename Typ1, typename Typ2> struct RS_T<Typ1, Typ2, Nothing, Nothing, Nothing> : public RenderState_T<RS_T<Typ1, Typ2>>, public Typ1, public Typ2, 
	public dpndnC_Join< typename Typ1::Req, typename Typ2::Req>::R::Comp {
public:
	void proc(DisMain &dm) const {
		Typ1::proc_T(dm, this);
		Typ2::proc_T(dm, this);
	}
};

typedef RS_T<ViewPort> RS_ViewPort;
typedef RS_T<Ortho> RS_Ortho;
typedef RS_T<Projection> RS_Projection;

typedef RS_T<ViewPort, Ortho > RS_VpOrtho;

}	}

#endif //GEM_DIS_RENDERSTATE_H