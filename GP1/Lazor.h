#ifndef GP1_LAZOR_H
#define GP1_LAZOR_H

#include <Gem/Scene2/Sprite.h>
#include <Gem/JUI/InputGroup.h>
#include <Gem/Scene2/Texture.h>
#include <Gem/Scene2/Scene.h>

class Lazor : public Scene2::Scene::Node_Base {
public:
	Lazor(Scene2::Texture &tex, const vec2f &at, const float &rot);
	~Lazor() override;

	static int * HitCounter;
private:
	vec2f Vel;
	vec2f OSize; 
	float Timer;

	void update(Scene2::Scene::UpdateCntx &cntx) override;
	void addTo(Dis::DrawList & dl) override;
	Scene2::Sprite Spr;
};



#endif //GP1_LAZOR_H

