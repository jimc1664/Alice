#ifndef GEM_SCENE3_MATERIAL_H
#define GEM_SCENE3_MATERIAL_H

#include "../Basic.h"

#include "Scene.h"
#include "../Org/ary.h"

namespace Gem { 

namespace Dis { class ShaderProg;  }
	
namespace Scene3 {

class Texture;

namespace Cmpnt {
	class Camera;
};

class Material : public Cmpnt::Renderable {
public:
	/*Material(const Passive &o) : Tex(o.Tex), Mesh(o.Mesh), Prog(o.Prog) {
		//Hdwr = o.Hdwr;
		Count = 0;
	} */

	Material( Scene3::Texture &t, Dis::ShaderProg &p) : Tex(t), Prog(p) {}

	~Material() {}

//private:	
	//Dis::Mesh *Hdwr;
	Scene3::Texture &Tex;
	Dis::ShaderProg &Prog;


	struct RenderList {
		Cmpnt::Camera *Cam;
		ary<Cmpnt::Renderable> 
	};
	//ary<RenderList> 
	//dList<Cmpnt::Renderable> RList;

	//int Count;
};


}	}

#endif //GEM_SCENE3_MATERIAL_H