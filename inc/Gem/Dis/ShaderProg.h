#ifndef GEM_DIS_SHADERPROG_H
#define GEM_DIS_SHADERPROG_H

#include "DisMain.h"

#include "../Resource/Resource.h"
//#include "../Org/dList.h"
#include "../String/CStr.h"
#include "../Math/Matrix4.h"


namespace Gem {  namespace Dis {

class ShaderProg { // public Resource<Shader>  { //:  public dListNode<Texture> {
public:
	ShaderProg(  );

	static ShaderProg* fromFile(const CStr &vs, const CStr &ps );  //temporary
	static ShaderProg* prep(const CStr &vs, const CStr &ps) { return fromFile(vs, ps); }

	void fromFile( WorkHost&, const CStr &vs, const CStr &ps );

	void apply(RenderingCntx &rc, const mat4f &mvp, const mat4f &mv );


//private:
	Dis_Dat( GLuint, u32, Prog );
	Dis_Dat( GLint, s32, MVPLocation );
	Dis_Dat( GLint, s32, MVLocation );

};

}	}

#endif //GEM_DIS_SHADERPROG_H