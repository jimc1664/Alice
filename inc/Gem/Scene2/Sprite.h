#ifndef GEM_SCENE2_SPRITE_H
#define GEM_SCENE2_SPRITE_H

#include "../Basic.h"
#include "../Math/Vec2.h"

#include "Scene.h"

namespace Gem { 


namespace Scene2 {

class Texture;


class Sprite : public ScnComponent {
public:
	//Sprite( const char * caption, const vec2u16 &size, const u32 &flags);

	void addTo( Dis::DrawList & dl);
	vec2f Pos;
	float Rotation;
	vec2f Size;

	Texture *Tex;
private:		
//	vec2u16 Size;
};

}	}

#endif //GEM_SCENE2_SPRITE_H