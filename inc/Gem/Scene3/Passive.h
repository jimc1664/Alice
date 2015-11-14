#ifndef GEM_SCENE3_PASSIVE_H
#define GEM_SCENE3_PASSIVE_H

#include "../Basic.h"
#include "../String/CStr.h"

#include "Scene.h"


namespace Gem { 

namespace Dis { class ShaderProg;  }
	
namespace Scene3 {

class Texture; class Mesh;

class Passive {
public:
	Passive( const Passive &o ) : Tex(o.Tex), Mesh(o.Mesh), Prog(o.Prog) {
		//Hdwr = o.Hdwr;
		Count = 0;
	}

	Passive( Scene3::Mesh &m, Scene3::Texture &t, Dis::ShaderProg &p ) : Mesh(m), Tex(t), Prog(p) {}

	~Passive();

//private:	
	//Dis::Mesh *Hdwr;
	Scene3::Texture &Tex;
	Scene3::Mesh &Mesh;
	Dis::ShaderProg &Prog;

	int Count;
};


namespace Cmpnt {
class PassiveInst : public ScnBaseComponent, public TCmpnt<PassiveInst, Offset,Updateable,Renderable >{
public:
	//Sprite( const char * caption, const vec2u16 &size, const u32 &flags);
	//TestObj(Texture* tex, const vec3f &p, const quatF &r = quatF::identity(), const vec3f &s = vec3f(1.0f, 1.0f, 1.0f)) : Tex(tex), Scale(s) { Pos = p; Rot = r; }

	PassiveInst() : Dat( 0 ) {}
	void onAdd( AddCntx &ac, Prm &p  ) { }

	void addTo(Dis::DrawList & dl,  Prm &p );
	void onUpdate( UpdateCntx &cntx,  Prm &p);

	vec3f Scale;

	const Passive *const Dat; //todo - reference wrapper 
private:
};

}
class PassiveObj : public  S3_T<Cmpnt::PassiveInst> {
public:
	PassiveObj( Passive& d, const vec3f &p, const quatF &r = quatF::identity(), const vec3f &s = vec3f(1.0f, 1.0f, 1.0f))  { 
		Pos = p; Rot = r; 

		*(( Passive**)&Dat) = &d; Scale = s;
	}
};

}	}

#endif //GEM_SCENE3_PASSIVE_H