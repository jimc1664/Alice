#ifndef GEM_SCENE3_PLANET_H
#define GEM_SCENE3_PLANET_H

#include "../Basic.h"

#include "Scene.h"

namespace Gem { 
namespace Dis { class ShaderProg; }
namespace Scene3 {

class Texture;

namespace Cmpnt {
	class PlanetCmp : public ScnBaseComponent, public TCmpnt<PlanetCmp, Offset, Updateable, Renderable > {
public:
	//Sprite( const char * caption, const vec2u16 &size, const u32 &flags);
	//TestObj(Texture* tex, const vec3f &p, const quatF &r = quatF::identity(), const vec3f &s = vec3f(1.0f, 1.0f, 1.0f)) : Tex(tex), Scale(s) { Pos = p; Rot = r; }

	void onAdd( AddCntx &ac, Prm &p  ) {

		int hi = 0;
	}

	void onBuildDl( BuildDrawLCntx &cntx,  Prm &p );
	void addToDl( Dis::DrawList &dl, Prm &p );
	void onUpdate( UpdateCntx &cntx,  Prm &p);

	vec3f Scale;

	Material *Mat;

	//Dis::ShaderProg *Prog;
	//Texture *Tex; //todo ref wrapper
private:
};

}
class PlanetObj : public  S3_T<Cmpnt::PlanetCmp> {
public:
	PlanetObj(Material &m, const vec3f &p, const quatF &r = quatF::identity(), const vec3f &s = vec3f(1.0f, 1.0f, 1.0f)) {
		Pos = p; Rot = r; 
		Scale = s;
		Mat = &m;
	}
};

}	}

#endif //GEM_SCENE3_PLANET_H