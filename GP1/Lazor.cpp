#include "stdafx.h"

#include "Lazor.h"
#include "Asteroid.h"

#include <Gem/Math/Matrix2x3.h>


Lazor::Lazor( Scene2::Texture &tex, const vec2f &at , const float &rot  ) {
	Spr.Pos = at;
	Spr.Rotation = rot;
	Spr.Size = OSize = vec2f(66, 86)*0.6f;
	Spr.Tex = &tex;


	mat2x3f m; m.setRotation(rot);
	Vel = vec2f(0, -500)*m;

	Timer = 0;
}
Lazor::~Lazor() {}

inline bool sprite_spriteCollision( const Scene2::Sprite &a, const Scene2::Sprite &b  ) {
	return (a.Pos - b.Pos).sqrLeng() < pow2((a.Size.x + a.Size.y + b.Size.x + b.Size.y)*0.25f*0.5f);
}


void Lazor::update( Scene2::Scene::UpdateCntx &cntx ) {


	Spr.Pos += Vel*cntx.Delta;
	Spr.update(cntx); 

		
	if (Timer  += cntx.Delta >= 4.0f) {
		cntx.destroy(this);
		return;
	}
	
	for( auto &roid : Asteroid::Roids ) {
		if( sprite_spriteCollision( Spr, roid.Spr ) ) {
			cntx.destroy(this);
			roid.die();
			(*HitCounter)++;
			return;
		}
	}

	Spr.Size = OSize * (0.8f + 0.2f*sin(Timer*4.0f));
}
void Lazor::addTo( Dis::DrawList & dl)  {
	Spr.addTo(dl); 
}

int * Lazor::HitCounter = 0;