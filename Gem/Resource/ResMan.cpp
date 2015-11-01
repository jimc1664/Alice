#include "../stdafx.h"

#include <Gem/Scene3/Camera.h>
#include <Gem/Scene3/TestObj.h>
#include <Gem/Scene3/Scene.h>
#include <Gem/Dis/DrawList.h>


#include <Gem/Scene3/Texture.h>
#include <Gem/Dis/Texture.h>


using namespace Scene3;

/*
void Camera::render() {


} */

void Scene::add( Node_Base *nd ) {
	Root.add( nd );
}


void Scene::addTo( Dis::DrawList & dl) {
	for( Node_Base &n : Root ) 
		n.addTo(dl);
}

Scene::UpdateCntx::UpdateCntx(Scene &scn, const float &d )  : Delta(d), Scn(scn), It( scn.Root.start() ) {
	
	for(;It != scn.Root.end(); It++ ) {
		It->update(*this);
	}	
}
 
void Scene::UpdateCntx::destroy( Node_Base * nd ) {
	detach(nd);
	Scn.ToBeDeleted[Scn.TBDi].add(nd);
}
 
void Scene::UpdateCntx::detach( Node_Base * nd ) {
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

void TestObj::addTo( Dis::DrawList & dl) {
	dl.add<Dis::DrawTestCube>(this);
}

void TestObj::update( Scene::UpdateCntx &cntx) {
	Rot *= quatF::yRotation(cntx.Delta *0.1f);
	//Pos += 0.1f;
}


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