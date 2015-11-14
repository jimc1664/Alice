#ifndef GEM_SCENE3_MESH_H
#define GEM_SCENE3_MESH_H

#include "../Basic.h"
#include "../String/CStr.h"
#include "../Math/rect.h"


namespace Gem { 

namespace Dis { class Mesh; }
	
namespace Scene3 {

class Mesh {
public:

	Mesh( const Mesh &o ) {
		Hdwr = o.Hdwr;
	}
	//todo - preprocess flags - pre multiply, etc
	Mesh( const CStr &file );

	~Mesh();

//private:	
	Dis::Mesh *Hdwr;

};

}	}

#endif //GEM_SCENE3_MESH_H