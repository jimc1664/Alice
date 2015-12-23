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
protected:
public:
//	Camera(const vec3f &p, const quatF &r = quatF::identity()) : Pos(p), Rot(r) {}
//
	void setCam( BuildDrawLCntx & dl, Prm &p );
//	void update(Scene::UpdateCntx &cntx);

	virtual void buildDL(BuildDrawLCntx &cntx, BuildDrawLCntx::CamEntry &ce ) = 0;

	virtual void preBuildDL( Dis::DrawList &dl) = 0;   //todo - could wrap round caller too kill a virtual
	virtual void postBuildDL( Dis::DrawList &dl) = 0; 
	void buildDL( BuildDrawLCntx &cntx, BuildDrawLCntx::CamEntry &ce , Prm &p  );
	void preBuildDL( Dis::DrawList &dl, Prm &p  );
	void postBuildDL( Dis::DrawList &dl, Prm &p  );
	template<typename Cmpnt, class Base> class TopIF : public Cmpnt { //this could/should be implemented more cleanly ..time constraints
		void buildDL( BuildDrawLCntx &a, BuildDrawLCntx::CamEntry &b ) override { return Cmpnt::buildDL( a, b,  PrmHelper<typename Cmpnt::Prm>::cast(  static_cast<Base*>(this) ) );  } 
		void preBuildDL( Dis::DrawList &a ) override { return Cmpnt::preBuildDL( a, PrmHelper<typename Cmpnt::Prm>::cast(  static_cast<Base*>(this) ) );  } 
		void postBuildDL( Dis::DrawList &a ) override { return  Cmpnt::postBuildDL( a, PrmHelper<typename Cmpnt::Prm>::cast(  static_cast<Base*>(this) ) );  } 
	}; 
};

}}
template<typename Base> struct TopIFFor<Base, Scene3::Cmpnt::Camera> : public Scene3::Cmpnt::Camera::TopIF<Scene3::Cmpnt::Camera,Base>{};

namespace Scene3 {
class CameraObj : public  S3_T<Cmpnt::Camera> {
public:
	CameraObj( const vec3f &p, const quatF &r = quatF::identity() )  { 
		Pos = p; Rot = r; 
	}
};
}

namespace Dis {

	
class DM_DefferedPrep : public Dis::DrawMsg_T<DM_DefferedPrep> {
friend class msgQ<DrawMsg>;
	DM_DefferedPrep( ) {}
friend class DrawMsg_TSpec;
	void proc( Dis::RenderingCntx &rc);
public:

};


class DM_DefferedLightPass : public Dis::DrawMsg_T<DM_DefferedLightPass> {
friend class msgQ<DrawMsg>;
	DM_DefferedLightPass( Scene3::Cmpnt::Camera &c, const mat3x4f &v ) : Cam(c), View(v) {}
friend class DrawMsg_TSpec;
	void proc( Dis::RenderingCntx &rc);
public:
	Scene3::Cmpnt::Camera &Cam;
	mat3x4f View;
};


class DM_DefferedFinal : public Dis::DrawMsg_T<DM_DefferedFinal> {
friend class msgQ<DrawMsg>;
	DM_DefferedFinal( Scene3::Cmpnt::Camera &c, const mat3x4f &v ) : Cam(c), View(v) {}
friend class DrawMsg_TSpec;
	void proc( Dis::RenderingCntx &rc);
public:
	Scene3::Cmpnt::Camera &Cam;
	mat3x4f View;
};


}

}

#endif //GEM_SCENE3_CAMERA_H