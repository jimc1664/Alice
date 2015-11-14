#ifndef GEM_DIS_MESH_H
#define GEM_DIS_MESH_H

#include "DisMain.h"

#include "../Resource/Resource.h"
//#include "../Org/dList.h"
#include "../String/CStr.h"

namespace Gem {  namespace Dis {

class Mesh { //: public Resource<Texture>  { //:  public dListNode<Texture> {
public:
	Mesh(  );

	static Mesh* fromFile(const CStr &s);  //temporary

	void fromFile(DisMain &dm, const CStr &s);

	void draw(RenderingCntx &rc);

protected:

private:
	Dis_Dat( GLuint, u32, VA  );
	Dis_Dat( GLuint, u32, VB  );
	Dis_Dat( GLuint, u32, IB  );
	int IC;
};

}	}

#endif //GEM_DIS_MESH_H