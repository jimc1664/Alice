
#include "stdafx.h"

#include <Gem/Main.h>
#include <Gem/MainWindow.h>
#include <Gem/ConCur/Thread.h>
#include <Gem/ConCur/ConCur.h>
#include <Gem/Scene2/Camera.h>
#include <Gem/Dis/RenderState.h>

#include <Gem/OS/Time.h>
#include <Gem/Math/Randomizer.h>

#include "SpaceShip.h"
#include "Asteroid.h"
#include "Lazor.h"
#include "Audio.h"

#include <Stdio.h>
#include <iostream>
#include <array>

#include <Gem/Dis/Texture.h>
#include "Gem/String/DStr.h"


template<class typ> class obb2 : public tuple<vec2_T<typ>, vec2_T<typ>, vec2_T<typ>, vec2_T<typ>> {
protected:
	typedef vec2_T<typ> v2;
public:
	obb2( ) {}
	obb2( const mat2x3f &trns ) {
		x = v2(-0.5f, -0.5f);
		y = v2( 0.5f, -0.5f);
		z = v2(-0.5f,  0.5f);
		w = v2( 0.5f,  0.5f);		

		*this *= trns;
	}

	Template1 obb2& operator*= (const T &i) { *this = *this * i; return *this; }

	/*
	//obb intersection 
	//http://www.flipcode.com/archives/2D_OBB_Intersection.shtml 

	bool overlap( const obb2 &o ) {
		axis[0] = corner[1] - corner[0]; 
        axis[1] = corner[3] - corner[0]; 

        // Make the length of each axis 1/edge length so we know any
        // dot product must be less than 1 to fall within the edge.

        for (int a = 0; a < 2; ++a) {
            axis[a] /= axis[a].squaredLength();
            origin[a] = corner[0].dot(axis[a]);
        }
	}

	bool overlaps_sub(const obb2& o) const {

		for(int a = 0; a < 2; ++a) {

			double t = other.corner[0].dot(axis[a]);

			// Find the extent of box 2 on axis a
			double tMin = t;
			double tMax = t;

			for (int c = 1; c < 4; ++c) {
				t = other.corner[c].dot(axis[a]);

				if (t < tMin) {
					tMin = t;
				} else if (t > tMax) {
					tMax = t;
				}
			}

			// We have to subtract off the origin

			// See if [tMin, tMax] intersects [0, 1]
			if ((tMin > 1 + origin[a]) || (tMax < origin[a])) {
				// There was no intersection along this dimension;
				// the boxes cannot possibly overlap.
				return false;
			}
		}

		// There was no dimension along which there is no intersection.
		// Therefore the boxes overlap.
		return true;
	} */
};

template<class T> obb2<T> operator* ( const obb2<T> &a, const mat2x3_T<T> &b ) {
	obb2<T> r = a;
	r.x *= b;
	r.y *= b;
	r.z *= b;
	r.w *= b;
	return r;
}

typedef obb2<f32> obb2f;

template<class T> class aabb2 : public rect<T> {
	typedef rect<T> rct;
	typedef obb2<T> obb;
public:
	aabb2( const rct &r ) {
		if( r.x > r.z ) {
			x = r.z; z = r.x;
		} else {
			x = r.x; z = r.z;
		}
		if( r.y > r.w ) {
			y = r.w; w = r.y;
		} else {
			y = r.y; w = r.w;
		}
	}
	aabb2( const obb &r ) {
		tl() = br() = r.x;
		envelop(r.y);
		envelop(r.z);
		envelop(r.w);
	}

	aabb2& envelop( const v2 &v ) {
		if( v.x < tl().x ) {
			tl().x = v.x;
		} else if( v.x > br().x ) {
			br().x = v.x;
		}

		if( v.y < tl().y ) {
			tl().y = v.y;
		} else if( v.y > br().y ) {
			br().y = v.y;
		}

		return *this;
	}


};
typedef aabb2<f32> aabb2f;


typedef  Scene2::ScnNode<Scene2::Sprite> Label;

class Button  : public Scene2::ScnNode<Scene2::Sprite> {
public:
	Button( Scene2::Texture& tex, Scene2::Texture& tex2, const vec2f &at,const vec2f &size ) : T1(tex), T2(tex2)  {
		Tex = &tex;
		Pos = at;
		Size = size;
		Rotation = 0;
		Clicked = false;

	}

	void update(Scene2::Scene::UpdateCntx &cntx) override {

		rectf r;
		r.tl() = Pos - Size *0.5f;
		r.br() = Pos + Size *0.5f;

		bool mPress = JUI::key(JUI::Keycode::LMouse);

		if( r.pointCheck( (vec2f) JUI::mPos() ) ) {
			Tex = &T2;
			if (!LMouse && mPress) Pressed = true;
		} else {
			Tex = &T1;
			Pressed = false;
		}
		if (!mPress && Pressed) {
			Clicked = true;
			Pressed = false;
		} 
		LMouse = mPress;

	}
	Scene2::Texture &T1, &T2;

	bool Clicked, LMouse, Pressed;
};


class Counter : public Scene2::Scene::Node_Base {
public:
	
	const float Stride = 94;
	Counter(Scene2::Texture &tex, const vec2f &at, const f32 &scale, const u32 &val)
		: Tex({	Scene2::Texture( tex, rectf(0,164, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride,164, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride*2,164, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride*3,164, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride*4,164, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(0,0, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride,0, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride*2,0, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride*3,0, Stride, 164), vec2s(496,329)  ), 
			Scene2::Texture( tex, rectf(Stride*4,0, Stride, 164), vec2s(496,329)  )
	}), Value(val)
	{
		for( int i = 3; i--; ) {
			Spr[i].Size = vec2f(99, 164)*scale;
			Spr[i].Pos = at + vec2f(Spr[i].Size.x,0)*(f32)i;
			Spr[i].Tex = &Tex[0];
			Spr[i].Rotation = 0;
		}
	}
//private:
	~Counter() override {}

	void update(Scene2::Scene::UpdateCntx &cntx) override {

	}
	void addTo(Dis::DrawList & dl) override {

		int v = Value;
		for(int i = 3; i-- ; ) {
			
			Spr[i].Tex = &Tex[v % 10];
			Spr[i].addTo(dl);

			v /= 10;
		}

	}
	int Value;
	Scene2::Sprite Spr[3];
	std::array<Scene2::Texture, 10> Tex;
	//Scene2::Texture Tex[10];
};


class Frame {
protected:	
	Frame( const CStr & str ) : BackgroundTex( str ) {
		BackgroundSpr.Size = vec2f(1024, 768);
		BackgroundSpr.Pos = BackgroundSpr.Size*0.5f;
		BackgroundSpr.Tex = &BackgroundTex;
		BackgroundSpr.Rotation = 0;
	}

	Scene2::Scene Scene;

	Scene2::Sprite  BackgroundSpr;	
	Scene2::Texture BackgroundTex;
};


class MainMenu : public Frame{
public:

	MainMenu() 
		: Frame( CSTR("Media//menubackground.png") ),
		TitleTex( CSTR("Media//UI//title.png") ),
		HowToTex( CSTR("Media//UI//HowTo.png") ),
		PlayBttnTex1( CSTR("Media//UI//buttons.png"), rects(0,192, 256,64), vec2s(256,256) ),
		PlayBttnTex2( PlayBttnTex1, rects(0,128, 256,64), vec2s(256,256) ),	
		QuitBttnTex1( PlayBttnTex1, rects(0,64, 256,64), vec2s(256,256) ),	
		QuitBttnTex2( PlayBttnTex1, rects(0,0, 256,64), vec2s(256,256) ),
		PlayBttn(PlayBttnTex1,PlayBttnTex2, vec2f( 300,600), vec2f(256,64) ), 
		QuitBttn( QuitBttnTex1,QuitBttnTex2, vec2f( 724,600), vec2f(256,64))
	{

		Scene.add(&PlayBttn);
		Scene.add(&QuitBttn);

		
		Scene.add(&Title);

		Audio::load(CSTR("Media//Audio//music.wav"), true)->play();
	}

	void loop( volatile bool &shutdown, Dis::BufferedDrawList &bdl ) {

		OS::Time time = NoConstruct();
		sizet frameTime = 18;

		float deltaTime = 0.001f*(f32)frameTime;
		
		PlayBttn.Clicked = QuitBttn.Clicked = false;
		Title.Pos = vec2f(512, 300);
		Title.Size = vec2f(600,200);
		Title.Rotation = 0;
		Title.Tex = &TitleTex;

//		Dis::ViewPort_RS vp(vec2u(1024, 768));
//		vp.apply();

		for(;!shutdown;) {
			
			time.update();

			
			Scene.update(deltaTime);
			{
				auto dl = bdl.forUpdate();			
				BackgroundSpr.addTo(dl);
				Scene.addTo(dl);

				//Spr3.addTo(dl);
			}

			sizet ct = (sizet)time.updateElap().asI();
			if( ct < frameTime ) ConCur::sleep(frameTime-ct);
			//else uh oh

			if( PlayBttn.Clicked ){
				if (Title.Tex == &HowToTex) return;

				Title.Size = vec2f(0.0f,0.0f);
				Title.Tex = &HowToTex;
				Title.Size = vec2f(400,200);

				PlayBttn.Clicked = false;
			}
			if (QuitBttn.Clicked) ConCur::fullExit(-1);
		}

		Scene.clear();
	}

	Scene2::Texture PlayBttnTex1, PlayBttnTex2, QuitBttnTex1, QuitBttnTex2, TitleTex, HowToTex;
	Button PlayBttn, QuitBttn;
	Label Title;
};

class Game : public Frame {
public:


	Game()
		: ShipTex( CSTR("Media//redfighter//redfighter0005.png") ),  
		Roid1( CSTR("Media//roids//large//a10000.png") ),  
		Roid2( CSTR("Media//roids//large//a30000.png") ),  
		BeamsTex( CSTR("Media//beams.png"), rects(51,148, 56,69), vec2s(490,446) ),
		BeamsTex2( BeamsTex, rects(227,357, 66,86), vec2s(490,446) ),
		CntrTex( CSTR("Media//ui//counter.png") ),
		ScoreTex( CSTR("Media//ui//score.png") ),
		Frame( CSTR("Media//background.png") ),
		Cntr( CntrTex, vec2f(900,50), 0.4f, 0 )
	{

		ScoreSpr.Pos = vec2f(800, 50);
		ScoreSpr.Size = vec2f(160, 60);
		ScoreSpr.Tex = &ScoreTex;
		ScoreSpr.Rotation = 0;

		Lazor::HitCounter = &Cntr.Value;
	}

	void loop( volatile bool &shutdown, Dis::BufferedDrawList &bdl ) {

		OS::Time time = NoConstruct();

		sizet frameTime = 18;
		  
		float deltaTime = 0.001f*(f32)frameTime;
		
		/*Scene.add(new Asteroid(Tex2, vec2f(400, 100)));
		Scene.add(new Asteroid(Tex2, vec2f(600, 100)));
		Scene.add(new Asteroid(Tex2, vec2f(100, 300)));
		Scene.add(new Asteroid(Tex2, vec2f(600, 300)));
		Scene.add(new Asteroid(Tex2, vec2f(100, 600)));
		Scene.add(new Asteroid(Tex2, vec2f(400, 600)));
		Scene.add(new Asteroid(Tex2, vec2f(600, 600))); */
//		
		Scene.clear();
		Scene.add( new SpaceShip( ShipTex, BeamsTex, BeamsTex2 ) );

		Cntr.Value = 0;

		vec2f scrnSize(1024, 768), mid = scrnSize *0.5f;
		Randomizer rand = Randomize();
		float spawn = 0,  nSpawn = 0.7,  speedMd = 1.0f;
		for(;!shutdown;) {
			
			time.update();

			if( (spawn -= deltaTime)  < 0 ) {

				//auto p = scrnSize*0.5f;
				auto p = mid - vec2f(rand.randf() - 0.5f, rand.randf() - 0.5f).getNormal() *mid * 1.5f;
				auto vel = (mid+ vec2f(rand.randf() - 0.5f, rand.randf() - 0.5f)*scrnSize*0.6f - p).getNormal()  *40.0f *(1+rand.randf()*0.5f )  *speedMd;
				auto scl = vec2f(1 + rand.randf(), 1 + rand.randf()) * (50 + rand.randf() * 20);
				///auto r =  rand.randf() *2.0f*PIf
				//mat2x3f m; m.setRotation( 
				Scene.add(new Asteroid( rand.randf() > 0.5f ? Roid1 : Roid2, p,scl, vel, rand.randf() *2.0f*PIf, ( rand.randf()-0.5f )*speedMd*2.0f ));

				spawn = 0.2f+ nSpawn;
				nSpawn *= 0.975f;

				speedMd += spawn *0.05f;

				
			}
			Scene.update(deltaTime);

			if (SpaceShip::Ship == 0) return;
			{
				auto dl = bdl.forUpdate();			
				BackgroundSpr.addTo(dl);
				Scene.addTo(dl);
				Cntr.addTo(dl);
				ScoreSpr.addTo(dl);
				//Spr3.addTo(dl);
			}

			sizet ct = (sizet)time.updateElap().asI();
			if( ct < frameTime ) ConCur::sleep(frameTime-ct);
			//else uh oh
		}

		Scene.clear();
	}
	Scene2::Sprite ScoreSpr;	
	Scene2::Texture ShipTex, Roid1, Roid2, BeamsTex, BeamsTex2, CntrTex, ScoreTex;

	Counter Cntr;
};


class Main { 
friend class Gem::Main_Hlpr;
	
	Main()  { 
		DEFClassFunc( void, mainLoopWrp, Main, mainLoop );
		MainThread.start( mainLoopWrp, this );			
	//	Cam.setOutput(Wndw);		
	}

	~Main() {
		MainThread.waitFor();
//		Mvmnt.deactivate();
	}

	void mainLoop() {
		Shutdown = false;
		Audio::initOAL();
		MainMenu menu;
		Game game;

		//Dis::RS_Ortho ortho; ortho.ScrnSz = vec2u(1024, 768); ortho.Height = 768.0f; ortho.apply();
		//Dis::RS_ViewPort vp; vp.ScrnSz = vec2u(1024, 768); vp.apply();

		Dis::RS_VpOrtho vp; vp.ScrnSz  = vec2u(1024, 768); vp.Height = 768.0f; vp.apply();

		Dis::RenderState_2d rs; rs.apply();

		for(;!Shutdown;) {		
			menu.loop( Shutdown, Wndw.DrawL );						
			game.loop( Shutdown, Wndw.DrawL );						
		}		
	}

	void shutdown() {
		Shutdown = true;
	}


	AutoCtor( MainWindow, Wndw,	( "GP1", vec2u16(1024,768), OS::Window::Flg_Resizeable ) );

//	Scene2::Camera Cam;



	Thread MainThread;
	volatile bool Shutdown;
};

DEF_MainClass( Main );
