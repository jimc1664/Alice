#ifndef GEM_DIS_DRAWLIST_H
#define GEM_DIS_DRAWLIST_H

#include "DisMain.h"
#include "../Org/msgQ.h"
#include "../Math/Matrix2x3.h"
#include "../Math/Matrix3x4.h"

namespace Gem {  

namespace Scene2 { class Sprite; class Texture; }
namespace Scene3 { namespace Cmpnt { class TestObj; } class Texture; class Passive; }
	
namespace Dis {

class ShaderProg;
struct RenderingCntx;

class DrawMsg : public msgQNode<DrawMsg> {
friend class msgQ<DrawMsg>;
	virtual sizet proc(ptr cntx) = 0;
};
Template1 class DrawMsg_T : public DrawMsg {
	sizet proc( ptr cntx ) override {
		static_cast<T*>(this)->proc( *(RenderingCntx*) cntx );
		return sizeof(T);
	}
protected:
	typedef DrawMsg_T DrawMsg_TSpec;
};

Template1 class DrawMsg_Generic : public DrawMsg_T<DrawMsg_Generic<T> >, public T {
friend class msgQ<DrawMsg>;
	DrawMsg_Generic( const T* a ) : T(*a) {}
friend class DrawMsg_TSpec;
	void proc(RenderingCntx &rc) { T::proc(rc); }
};

class DrawSprite : public DrawMsg_T<DrawSprite> {
friend class msgQ<DrawMsg>;
	DrawSprite( Scene2::Sprite *spr );
friend class DrawMsg_TSpec;
	void proc(RenderingCntx &rc);
public:
	mat2x3f Trans;
	Scene2::Texture &Tex;
	// Scene2::Sprite &Spr;
};

class DrawPassive : public DrawMsg_T<DrawPassive>  {
friend class msgQ<DrawMsg>;
	DrawPassive( const mat3x4f &m, const Scene3::Passive &d ) : Trans(m), Dat(d) {}
friend class DrawMsg_TSpec;
	void proc(RenderingCntx &rc);
public:
	mat3x4f Trans;
	const Scene3::Passive &Dat;
};


class DrawPlanet : public DrawMsg_T<DrawPlanet> {
	friend class msgQ<DrawMsg>;
	DrawPlanet(const mat3x4f &m, Scene3::Texture &t, Dis::ShaderProg &p) : Trans(m), Tex(t), Prog(p) {}
	friend class DrawMsg_TSpec;
	void proc(RenderingCntx &rc);
public:
	mat3x4f Trans;
	Scene3::Texture &Tex;
	Dis::ShaderProg &Prog;
};
class DrawTestCube : public DrawMsg_T<DrawTestCube> {
	friend class msgQ<DrawMsg>;
	DrawTestCube(const mat3x4f &m, Scene3::Texture &t, Dis::ShaderProg &p) : Trans(m), Tex(t), Prog(p) {}
	friend class DrawMsg_TSpec;
	void proc(RenderingCntx &rc);
public:
	mat3x4f Trans;
	Scene3::Texture &Tex;
	Dis::ShaderProg &Prog;
};


//class RenderTarget;
class DrawList : public msgQ<DrawMsg> {
public:
	DrawList(  ) : View(mat3x4f::identity()){

	}
	Template2 void proc( T &a, T2 &p ) { a.addTo( *this, p ); }
	//void setOutput( RenderTarget& rt );   //todo thread checks..
//#ifdef DIS_INTERNAL
	void render(RenderingCntx &rc);
	//void setOutput( RenderingCntx &rc, RenderTarget& rt );
//#endif //DIS_INTERNAL

	mat3x4f View;
protected:

private:
	
};

class BufferedDrawList {
public:
	BufferedDrawList();
	class DrawListRef {
	public:
		~DrawListRef() {
			Base.doneUpdate(Dl);
		}
		operator DrawList&() {return Dl; }
	private: friend class BufferedDrawList;
		DrawListRef(DrawList& dl, BufferedDrawList &b ) : Dl(dl), Base(b) {}
		DrawList& Dl; BufferedDrawList &Base;
	};

	DrawListRef forUpdate();

//#ifdef DIS_INTERNAL
	void render(RenderingCntx &rc);
//#endif //DIS_INTERNAL

private: friend class DrawListRef;
	void doneUpdate( DrawList& dl );
	void swap(u8 a, u8 b);
	DrawList  * volatile ToRender, *volatile Rendering;



	DrawList Dl[3];
	int Ui;

	/*
	__declspec(align(4)) struct Offset {
		union {
			u8 B[3]; 
			volatile u32 I; 
		};
	};
	Offset Inds;
	enum {
		Update = 0,
		Render,
		Mid,
	};*/

};

}	}

#endif //GEM_DIS_DRAWLIST_H