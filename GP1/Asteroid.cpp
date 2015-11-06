#include "stdafx.h"

#include "Asteroid.h"
#include "SpaceShip.h"

#include <Gem\Math\Matrix2x3.h>

#include <Gem\Dis\Camera.h>

Asteroid::Asteroid( Scene2::Texture &tex, const vec2f &at, const vec2f &size, const vec2f &vel, const float &rot, const float &aVel  ) : Vel(vel), AVel(aVel) {
	Spr.Pos = at;
	Spr.Rotation = rot;
	Spr.Size = OSize = size;
	Spr.Tex = &tex;

	DestroyTimer = -1;
	Timer = 0;
	Roids.add(this);
}
Asteroid::~Asteroid() {

	if( dListNode<Asteroid>::isListed() )
		Roids.detach(this);	
}


inline bool sprite_spriteCollision( const Scene2::Sprite &a, const Scene2::Sprite &b  ) {

	/*mat2x3f am(a.Pos, a.Rotation, a.Size), bm(b.Pos, b.Rotation, b.Size);

	obb2f ar = am, br = bm;

	aabb2f ab = ar, bb = br;

	if( ab.br().x < bb.tl().x || ab.br().y < bb.tl().y || 
		ab.tl().x > bb.br().x || ab.tl().y > bb.br().y) 
				 return false;
	return true;
				 */
	return (a.Pos - b.Pos).sqrLeng() < pow2((a.Size.x + a.Size.y + b.Size.x + b.Size.y)*0.25f*0.75f );
}

const float timeToDie = 0.3f;

void Asteroid::die() {
	Roids.detach(this);
	DestroyTimer = timeToDie;
}
void Asteroid::update( Scene2::Scene::UpdateCntx &cntx ) {
	
	Spr.Pos += Vel*cntx.Delta;
	Spr.Rotation += AVel * cntx.Delta;
	Spr.update(cntx); 

	if( DestroyTimer > 0 ) {
		DestroyTimer -= cntx.Delta;
		if( DestroyTimer <= 0 ) {
			cntx.destroy(this);
		}
		Spr.Size = OSize  * pow2(DestroyTimer / timeToDie);

	} else {
		if (Timer  += cntx.Delta >= 10.0f) {
			die();
		}

		if(  SpaceShip::Ship && sprite_spriteCollision( Spr, SpaceShip::Ship->Spr ) ) {
			//std::cout << "col\n";

			die();
			SpaceShip::Ship->die();
			//cntx.destroy(this);

		}
	}

}
void Asteroid::addTo( Dis::DrawList & dl)  {
	Spr.addTo(dl); 
}


dList<Asteroid> Asteroid::Roids;
