#ifndef GEM_SCENE3_PASSIVE_H
#define GEM_SCENE3_PASSIVE_H

#include "../Basic.h"

#include "Scene.h"


namespace Gem { 


namespace Scene3 {

class Material; class Mesh;

class Passive {
public:
	Passive(const Passive &o) : Mat(o.Mat), Mesh(o.Mesh) {
		//Hdwr = o.Hdwr;
	//	Count = 0;
	}

	Passive(Scene3::Mesh &m, Scene3::Material &mt ) : Mesh(m), Mat(mt) {}

	~Passive();

//private:	
	//Dis::Mesh *Hdwr;
	Scene3::Material &Mat;
	Scene3::Mesh &Mesh;

};


namespace Cmpnt {
class PassiveInst : public ScnBaseComponent, public TCmpnt<PassiveInst, Offset,Updateable,Renderable >{
public:

	PassiveInst() : Dat( 0 ) {}
	void onAdd( AddCntx &ac, Prm &p  ) { }

	void onBuildDl( BuildDrawLCntx &cntx,  Prm &p );
	void addToDl( Dis::DrawList &dl, Prm &p );
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