#ifndef GEM_SCENE3_TEXTURE_H
#define GEM_SCENE3_TEXTURE_H

#include "../Basic.h"
#include "../String/CStr.h"
#include "../Math/rect.h"


namespace Gem { 

namespace Dis { class Texture; }
	
namespace Scene3 {

class Texture {
public:

	Texture( const Texture &o ) {
		Hdwr = o.Hdwr;
		Section = o.Section;
	}
	//todo - preprocess flags - pre multiply, etc
	Texture( const CStr &file );
	Texture( const CStr &file, const rects &section, const vec2s &dim );
	Texture( const Texture &t, const rects &section, const vec2s &dim );

	~Texture();
	//Sprite( const char * caption, const vec2u16 &size, const u32 &flags);

//private:	
	Dis::Texture *Hdwr;
	rectf Section;
//	vec2u16 Size;
	void setSection( const rects &section, const vec2s &dim );

};

}	}

#endif //GEM_SCENE3_TEXTURE_H