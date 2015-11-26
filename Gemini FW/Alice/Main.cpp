#include "stdafx.h"

#include "Gem/Math/vec3.h"
vec3f& debugPosition();


#include <Gem/Main.h>
#include <Gem/MainWindow.h>
#include <Gem/ConCur/Thread.h>
#include <Gem/ConCur/ConCur.h>

#include <Gem/Dis/ShaderProg.h>


#include <Gem/Scene2/Camera.h>
#include <Gem/Scene2/Sprite.h>
#include <Gem/Scene2/Texture.h>
#include <Gem/Scene2/Scene.h>

#include <Gem/Scene3/Camera.h>
#include <Gem/Scene3/TestObj.h>
#include <Gem/Scene3/Passive.h>
#include <Gem/Scene3/Texture.h>
#include <Gem/Scene3/Mesh.h>
#include <Gem/Scene3/Scene.h>
#include <Gem/Scene3/Planet.h>

#include <Gem/Resource/Resource.h>

#include <Gem/Dis/RenderState.h>
#include <Gem/JUI/InputGroup.h>
#include <Gem/OS/Time.h>

#include <Gem/Math/Randomizer.h>

#include "Audio.h"
#include <array> //counter



typedef Scene2::ScnNode<Scene2::Sprite> Label;
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

/*
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

/*
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

*/


class Game *Gm;


class Game  {
public:
	Game() : 
		CntrTex( CSTR("Media//ui//counter.png") ),
		StdShdr( CSTR("Media//shaders//textureVS.glsl"), CSTR("Media//shaders//textureFS.glsl") ),
		PlanetShdr( CSTR("Media//shaders//planetVS.glsl"), CSTR("Media//shaders//planetFS.glsl")  ),
		Tex1( CSTR("Media//armoredrecon_diff.png") ),
		Tex2( CSTR("Media//Tank1DF.png") ),
		NoiseTex( CSTR("Media//magic clouds.png") ),
		Mesh1( CSTR("Media//armoredrecon.fbx") ),
		Mesh2( CSTR("Media//Tank1.FBX") ),
		Car( Mesh1, Tex1, StdShdr ),
		Tank( Mesh2, Tex2, StdShdr ),
		Cntr( CntrTex, vec2f(900,50), 0.4f, 0 )
	{

		Mvmnt.Key[0] = 'A';
		Mvmnt.Key[1] = 'D';
		Mvmnt.Key[2] = 'S';
		Mvmnt.Key[3] = 'W';
		MoveObj = 0;

		Gm = this;
		for( int i = 12; i--; ) {
			DebugToggles[i].Key = 0x70+i;
			DebugToggles[i].activate();
		}
	}

	void loop( volatile bool &shutdown, Dis::BufferedDrawList &bdl ) {

		OS::Time time = NoConstruct();

		sizet frameTime = 18;
		  
		float deltaTime = 0.001f*(f32)frameTime;


		Cntr.Value = 0;

		Scene.clear();
				/*auto *to = new Scene3::TestObj();
		to->Pos= vec3f(0, -2, 5);
		to->Tex = &TestTex;
		Scene.add(to);

		auto lambda = [&] ( ) { 

		to->Pos= vec3f(0, -2, 5);
		to->Tex = &TestTex;
			
		};   */


		//Scene3::ScnNode<Scene3::Camera>* cam = Scene.add(new Scene3::ScnNode<Scene3::Camera>( vec3f(0,0,0) );
		MoveObj = Scene.add(new Scene3::PlanetObj(NoiseTex, PlanetShdr, vec3f(0, 0, 0)));
		//MoveObj = Scene.add(new Scene3::TestObj(NoiseTex, PlanetShdr, vec3f(0, 0, 0)));
		auto car = MoveObj = Scene.add(new Scene3::PassiveObj(Car, debugPosition() = vec3f(0, 10, 20)));
	//	Scene.add( new Scene3::PassiveObj( Tank, vec3f(7,0,-3), quatF::identity(), vec3f(1,1,1)*1.25f ) );
	//	Scene.add( new Scene3::PassiveObj( Tank, vec3f(-7,0,-3), quatF::identity(), vec3f(1,1,1)*0.75f ) );

		auto cam = Scene.add( new Scene3::CameraObj( vec3f(0,5,-30.0f) ) );
		
		Mvmnt.activate();
		MoveObj = cam;

		Dis::RenderState_2d rs2;
		Dis::RenderState_3d rs3;
				
		Dis::RS_ViewPort vp; vp.ScrnSz = vec2u(1024, 768);
		vp.apply();
		Dis::RS_Ortho ortho; ortho.ScrnSz = vec2u(1024, 768); ortho.Height = 768;
		Dis::RS_Projection proj; proj.ScrnSz = vec2u(1024, 768);

		bool lMove = false; vec2s16 lMp;
		for(;!shutdown;) {
			
			time.update();
			
			Scene.update(deltaTime);

			car->Pos =  debugPosition();
			//		printf("p  %f %f %f \n", car->Pos.x, car->Pos.y, car->Pos.z);
			if( MoveObj ) {
				float speed = 5.0f  * deltaTime;
				auto a = Mvmnt.value();

				f32 disFromP = max( MoveObj->Pos.leng() -14.95f, 0.1f ); 
				speed *= disFromP /5.0f;
				MoveObj->Pos += vec3f(a.x,0,a.y)*speed  * MoveObj->Rot.as<mat3f>() ;
				//printf("p  %f %f %f \n", MoveObj->Pos.x, MoveObj->Pos.y, MoveObj->Pos.z);

				//todo mouse drag object
				if( JUI::key(JUI::Keycode::LMouse) ) {
					if( lMove ) {
						vec2f mr = ((vec2f)(lMp - JUI::mPos())) *deltaTime*1.0f;
						//MoveObj->Rot = (quatF::euler( vec3f( mr.y, mr.x, 0)  ) * MoveObj->Rot ).normalise();

						//MoveObj->Rot = (quatF::yRotation( mr.x ) * MoveObj->Rot );
						//MoveObj->Rot = (quatF::xRotation( mr.y ) * MoveObj->Rot );
						MoveObj->Rot *= quatF::euler(vec3f(mr.y, mr.x, 0));
						MoveObj->Rot.normalise();
					}
					lMp = JUI::mPos();
				}
				lMove = JUI::key(JUI::Keycode::LMouse);
			}

			{
				auto dl = bdl.forUpdate();			
				//BackgroundSpr.addTo(dl);
			
				proj.addTo(dl);
				rs3.addTo(dl);

				//Scene.addTo(dl);

				Scene.render( dl, cam);

				ortho.addTo(dl);
				rs2.addTo(dl);
				Cntr.addTo(dl);
				//ScoreSpr.addTo(dl);
			}
			//Cntr.Value++;

			sizet ct = (sizet)time.updateElap().asI();
			if( ct < frameTime ) ConCur::sleep(frameTime-ct);
		}

		//Scene.clear();
	}

	Res_From_T<Dis::ShaderProg, DStr, DStr > StdShdr, PlanetShdr;

	Scene2::Texture CntrTex;
	
	Scene3::Scene Scene;
	Scene3::Texture  Tex1, Tex2, NoiseTex;
	Scene3::Mesh Mesh1, Mesh2;
	Scene3::Passive Tank, Car;

	//Res_FromFile<Scene3::Texture> Tex1, Tex2, NoiseTex;

	Counter Cntr;
	
	JUI::AxisDual Mvmnt;
	Scene3::Cmpnt::Offset *MoveObj;

	

	JUI::Toggle DebugToggles[12];

};
bool debugFlag( const u32 &i ) {
	return Gm->DebugToggles[i].Value;
}

class Main { 
friend class Gem::Main_Hlpr;
	
	Main()  { 
		DEFClassFunc( void, mainLoopWrp, Main, mainLoop );
		MainThread.start( mainLoopWrp, this );			
	//	Cam.setOutput(Wndw);		
	}

	~Main() {
		MainThread.waitFor();
	}

	void mainLoop() {
		Shutdown = false;
		Audio::initOAL();


		Game game;
		for(;!Shutdown;) {		
			game.loop( Shutdown, Wndw.DrawL );						
		}		
	}

	void shutdown() {
		Shutdown = true;
	}


	AutoCtor( MainWindow, Wndw,	( "Alice", vec2u16(1024,768), OS::Window::Flg_Resizeable ) );

//	Scene2::Camera Cam;

	Thread MainThread;
	volatile bool Shutdown;
};

DEF_MainClass( Main );
