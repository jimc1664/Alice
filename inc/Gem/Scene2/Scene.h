#ifndef GEM_SCENE2_SCENE_H
#define GEM_SCENE2_SCENE_H

#include "../Basic.h"
#include "../Org/dList.h"

#include <type_traits>

namespace Gem { 
namespace Dis { class DrawList; }
	
namespace Scene2 {

template<class Typ> class ScnNode;

class Scene {
public:
	Scene(){}
	~Scene() { clear(); }
	//Sprite( const char * caption, const vec2u16 &size, const u32 &flags);

	struct UpdateCntx;
	class Node_Base : public dListNode<Node_Base> {
	public:
		virtual void update( UpdateCntx &cntx ) =0;
		virtual void addTo( Dis::DrawList & dl) =0;
		virtual ~Node_Base(){
			int a = 0;
		}
	//	dList<Node> Children;
	};

	template<class Typ>  void add( ScnNode<Typ> *nd ) { return add( static_cast<Node_Base*>(nd) ); }
	void add( Node_Base *nd );
	void clear();

	void addTo( Dis::DrawList & dl);

	struct UpdateCntx {
		UpdateCntx( Scene &scn, const float &d);
		const float Delta;

		void destroy(Node_Base *nd);
		void detach(Node_Base *nd);

		Scene &Scn;
	private:
		dList<Node_Base>::iter It;
	};
	void update( const f32 &delta );
private:	
	//todo seperate draw and update lists
	dList<Node_Base> Root;

	//simplifictaion of a system yet to add -  to defer deletion of 'stuff'for a few frames 
	//- so temporary references are dropped - so they don't have to be kept track of

	dList<Node_Base> ToBeDeleted[3];
	int TBDi;

};

class ScnComponent {
public:
	void update( Scene::UpdateCntx &cntx  ) {}
	void addTo( Dis::DrawList & dl) {}
};

template<class Typ> class ScnNode : public Scene::Node_Base, public Typ {
public:
	ScnNode() {	
		static_assert( std::is_convertible<Typ,ScnComponent >::value, "Type mismatch" );
	}
	~ScnNode() override {}

	void update( Scene::UpdateCntx &cntx ) override { Typ::update(cntx); }
	void addTo( Dis::DrawList & dl) override {Typ::addTo(dl); }

};

}	}

#endif GEM_SCENE2_SCENE_H