#include "stdafx.h"


#include "SpaceShip.h"
#include "Lazor.h"
#include "Audio.h"

#include <Gem/Math/Matrix2x3.h>


SpaceShip::SpaceShip( Scene2::Texture &tex, Scene2::Texture &wp, Scene2::Texture &lazr  ) :LazorTex(lazr) {
	Mvmnt.Key[0] = 'A';
	Mvmnt.Key[1] = 'D';
	Mvmnt.Key[2] = 'W';
	Mvmnt.Key[3] = 'S';
		
	Mvmnt.activate();


	Spr.Pos = 320.0f;
	Spr.Size = 100.0f;
	Spr.Tex = &tex;

	Weapon.Tex = &wp;

	Ship = this;

	Fire = 0;

	LaserSfx = Audio::load(CSTR("Media//Audio//pew.wav"),false,0.3f);
	DeathSfx = Audio::load(CSTR("Media//Audio//explosion.wav"));
		//DeathSfx;

	DeathTimer = -1;
}


SpaceShip::~SpaceShip()  {
//		Mvmnt.deactivate();
}

void SpaceShip::update( Scene2::Scene::UpdateCntx &cntx )  {


	if( DeathTimer > 0 ) {
		DeathTimer -= cntx.Delta;
		if( DeathTimer <= 0 ) {
		//	

			Ship = 0;
			cntx.detach(this);
		}		

		Spr.Rotation += cntx.Delta;

		return;
	}

	float speed = 200.0f  * cntx.Delta;
	auto a = Mvmnt.value();
	Spr.Pos += a*speed;

	Spr.Pos = max(Spr.Pos, Spr.Size*0.5f);
	Spr.Pos = min(Spr.Pos, vec2f(1024,768) - Spr.Size*0.5f);

	auto vec = Spr.Pos - (vec2f)JUI::mPos();
	Spr.Rotation = atan2(-vec.x, vec.y);

	Fire -= cntx.Delta;
	if( Fire < 0.0f && JUI::key(JUI::Keycode::LMouse) ) {
			
		mat2x3f m;
		m.setRotation(Spr.Rotation);
		cntx.Scn.add(new Lazor(LazorTex, Spr.Pos +vec2f(0, -60.0f )*m, Spr.Rotation ));
		LaserSfx->play();
		Fire = 0.3f;
	}

		
	//Spr.update(delta); 
}
void SpaceShip::addTo( Dis::DrawList & dl)  {

	if( DeathTimer > 0 ) {
		if( DeathTimer <= 4.0f ) {

			return;
		}
	}
		
	if( Fire > 0.2f ) {
		mat2x3f m;
		m.setRotation(Spr.Rotation);
		vec2f off = vec2f(0, -60.0f + (Fire-0.2f)*120.0f  ) *m;
		Weapon.Pos = Spr.Pos +off;
		Weapon.Rotation = Spr.Rotation+PIf;
		Weapon.Size = vec2f(56, 69)*1.2f   * (1.0f-abs(Fire-0.25f) *10.0f );
		Weapon.addTo(dl);
	} 

	Spr.addTo(dl); 

}
void SpaceShip::die() {
	if (DeathTimer > 0) return;
	DeathTimer = 6.0f;
	DeathSfx->play();
	Fire = 0;
}
SpaceShip *SpaceShip::Ship = 0;
