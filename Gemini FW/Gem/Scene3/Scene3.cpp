#include "../stdafx.h"

#include <Gem/Scene3/Scene.h>
#include <Gem/Dis/DrawList.h>



using namespace Scene3;


void Scene::addNode(Node_Base *nd) {
	Root.add( nd );
	//return nd;
}



UpdateCntx::UpdateCntx(Scene &scn, const float &d )  : Delta(d), Scn(scn), It( scn.Updateables.start() ) {
	
	for(;It != scn.Updateables.end(); It++ ) {
		It->updateObj(*this);
	}
	
}
 
void UpdateCntx::destroy( Node_Base * nd ) {
	detach(nd);
	Scn.ToBeDeleted[Scn.TBDi].add(nd);
}
 
void UpdateCntx::detach( Node_Base * nd ) {
	It.detachCur();
	It--;
}
void Scene::update( const f32 &delta ) {
	UpdateCntx(*this, delta);

	TBDi = (TBDi + 1) % 3;
	ToBeDeleted[TBDi].deleteAll();
}

void Scene::clear() {
	for (auto it = Root.start(); it; ) {
		Node_Base* nd = it;
		it.detachCur();
		ToBeDeleted[TBDi].add(nd);
	}
	Root.detachAll();
}

#include <Gem/Scene3/Texture.h>
#include <Gem/Dis/Texture.h>

Texture::Texture( const CStr &file ) : Section(0,0,1,1) {

	Hdwr = Dis::Texture::fromFile(file);
}

Texture::Texture( const CStr &file, const rects &s, const vec2s &dim ) {
	Hdwr = Dis::Texture::fromFile(file);

	setSection(s, dim);
}

Texture::Texture( const Texture &t, const rects &s, const vec2s &dim ) {
	Hdwr = t.Hdwr;
	setSection(s, dim);
}

void Texture::setSection( const rects &section, const vec2s &dim ) {
	rects s = section;
	//todo - proper rect code...
	s.z += s.x; 
	s.y = dim.y - s.y;
	s.w = s.y -  s.w ;

	auto t = s.w;
	s.w = s.y;
	s.y = t;
	//s.y -= dim.y*2.0f;
	
	Section = s;

	Section.x /= (f32)dim.x;
	Section.z /= (f32)dim.x;
	Section.y /= (f32)dim.y;
	Section.w /= (f32)dim.y;
}

Texture::~Texture() {
	//delete Hdwr;
}

#include <Gem/Scene3/Mesh.h>
#include <Gem/Dis/Mesh.h>

Mesh::Mesh( const CStr &file ) {

	Hdwr = Dis::Mesh::fromFile(file);
}


Mesh::~Mesh() {
	//delete Hdwr;
}

#include <Gem/Scene3/Camera.h>
#include <Gem/Math/Matrix4.h>

void Cmpnt::Camera::setCam(Dis::DrawList & dl, Prm &p ) {
	auto &off = p.get<Offset>();
	auto m2 = mat3x4f::view(off.Pos, off.Rot.as<mat3f>());
	//dl.View *= mat3x4f::view(off.Pos, off.Rot.as<mat3f>());
	
	auto m = mat3x4f::camLookDir(off.Pos, vec3f(0, 0, 1)* off.Rot.as<mat3f>() , vec3f(0, 1, 0)* off.Rot.as<mat3f>() );
	dl.View = m2;
}

#include <Gem/Scene3/TestObj.h>

void Cmpnt::TestCmp::addTo(Dis::DrawList & dl, Prm &p) {
	auto &off = p.get<Offset>();
	dl.add<Dis::DrawTestCube>( mat3x4f::transform(off.Pos,off.Rot.as<mat3f>(), Scale ) *dl.View, *Tex, *Prog ); //todo == vec3f::one()
}

void Cmpnt::TestCmp::onUpdate( UpdateCntx &cntx, Prm &p) {
	auto &off = p.get<Offset>();
	//off.Rot *= quatF::yRotation(cntx.Delta *0.1f);
	//Pos += 0.1f;
}
#include <Gem/Scene3/Passive.h>

void Cmpnt::PassiveInst::addTo(Dis::DrawList & dl, Prm &p) {
	auto &off = p.get<Offset>();
	auto w = mat3x4f::transform(off.Pos, off.Rot.as<mat3f>(), Scale);
	dl.add<Dis::DrawPassive>( mat3x4f::transform(off.Pos,off.Rot.as<mat3f>(), Scale ) *dl.View, *Dat ); //todo == vec3f::one()
}

void Cmpnt::PassiveInst::onUpdate( UpdateCntx &cntx, Prm &p) {
	auto &off = p.get<Offset>();
	off.Rot *= quatF::yRotation(cntx.Delta *0.1f);
	//Pos += 0.1f;
}

Passive::~Passive() {

}


