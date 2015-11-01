#ifndef GP1_SPACESHIP_H
#define GP1_SPACESHIP_H

#include <Gem/Scene2/Sprite.h>
#include <Gem/JUI/InputGroup.h>
#include <Gem/Scene2/Texture.h>
#include <Gem/Scene2/Scene.h>

class Audio;

class SpaceShip : public Scene2::Scene::Node_Base {
public:

	//typedef void(*SpawnProjectile)( const vec2f &at, const float &rot );
	SpaceShip(Scene2::Texture &tex, Scene2::Texture &wp, Scene2::Texture &lazr);
//private:
	~SpaceShip() override;

	void update(Scene2::Scene::UpdateCntx &cntx) override;
	void addTo(Dis::DrawList & dl) override;
	void die();
	float Fire;
	Scene2::Sprite Spr, Weapon;
	Scene2::Texture& LazorTex;
	JUI::AxisDual Mvmnt;

	static SpaceShip* Ship;

	Audio *LaserSfx, *DeathSfx;

	float DeathTimer;
};


#endif //GP1_SPACESHIP_H


