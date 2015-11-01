#ifndef GP1_ASTEROID_H
#define GP1_ASTEROID_H

#include <Gem/Scene2/Sprite.h>
#include <Gem/JUI/InputGroup.h>
#include <Gem/Scene2/Texture.h>
#include <Gem/Scene2/Scene.h>

class Asteroid : public Scene2::Scene::Node_Base, public dListNode<Asteroid> {
public:
	Asteroid(Scene2::Texture &tex, const vec2f &at, const vec2f &size, const vec2f &vel, const float &rot, const float &aVel );
	~Asteroid() override;


//private:
	void die();
	void update(Scene2::Scene::UpdateCntx &cntx) override;
	void addTo(Dis::DrawList & dl) override;
	Scene2::Sprite Spr;

	float DestroyTimer;
	float AVel, Timer;
	vec2f Vel, OSize;
	static dList<Asteroid> Roids;

};



#endif //GP1_ASTEROID_H
