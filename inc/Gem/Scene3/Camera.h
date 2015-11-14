#ifndef GEM_SCENE3_CAMERA_H
#define GEM_SCENE3_CAMERA_H

#include "../Basic.h"
//#include "../Dis/Camera.h"
#include "../Math/Vec3.h"
#include "../Math/Quaternion.h"

#include "Scene.h"

namespace Gem { namespace Scene3 {
namespace Cmpnt {

class Camera : public ScnBaseComponent, public TCmpnt<Camera, Offset> {
public:
//	Camera(const vec3f &p, const quatF &r = quatF::identity()) : Pos(p), Rot(r) {}
//
	void setCam(Dis::DrawList & dl, Prm &p );
//	void update(Scene::UpdateCntx &cntx);


private:	
	// void render() override;
//	vec2u16 Size;
};

}
class CameraObj : public  S3_T<Cmpnt::Camera> {
public:
	CameraObj( const vec3f &p, const quatF &r = quatF::identity() )  { 
		Pos = p; Rot = r; 
	}
};


}	}

#endif //GEM_SCENE3_CAMERA_H