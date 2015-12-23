#include "stdafx.h"

#include "Gem/Math/vec3.h"
vec3f& debugPosition(){
	static vec3f p; return p; }

#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "Gem/Dis/DrawList.h"
#include "Gem/Dis/Texture.h"
#include "Gem/Dis/Mesh.h"
#include "Gem/Dis/ShaderProg.h"

#include "OGL_Task.h"

#include "Gem/ConCur/ConCur.h"


#include "Gem/Math/vec4.h"
#include "Gem/Math/matrix4.h"
#include <Gem/Math/Randomizer.h>
#include "Gem/Org/Ary.h"

#include "Gem/Scene3/TestObj.h"
#include "Gem/Scene3/Texture.h"
#include "Gem/Scene3/Passive.h"
#include "Gem/Scene3/Mesh.h"


#include "../procedural/simplexnoise.h"

#include <iostream>
#include <fstream>


struct PlanetDat {
	vec3f Pos, Norm; 

	union {
		struct { float Ang, B, C, D; };
		struct { vec3f Col; };
	};
};


struct Vertex2 : public PlanetDat {
	vec2f Uv; vec3f Pos2;
};


//todo - "mipmapping" of planet data

void getPlanet( const int &dim , ary<PlanetDat> &verts ) {


	int magicNo = 0xdeadbea7; int version = 2;

	int dim1 = dim+1, seed = 1;

	verts.clear();
	verts.addMul(dim1*dim1  * 6);

	for(;;) {
		//break;
		std::fstream fi;
		fi.open ("Media//Planet//Alpha07.plnt",  std::ios::in | std::ios::binary );
		if(!fi.is_open()) break;
		int mn, v, d, s =seed;
		fi.read( (char*)&mn, sizeof(mn) );
		fi.read( (char*)&v, sizeof(v) );
		fi.read( (char*)&d, sizeof(d) );
	//	fi.read( (char*)&s, sizeof(s) );
		if( mn != magicNo || v != version || d != dim   || s != seed  ) {
			fi.close();
			break;
		}

		fi.read( (char*)&verts[0], sizeof(verts[0])*verts.count() ); 	

		fi.close();

		if( fi  ) { //sucess
			return;
		}
		break;
	}

	int iter = 10000; float m1 = 0.0005f, m2 = m1, l1 = 1.02f, l2 = 1.0f/l1; float radius = 15.0f;
	Randomizer rand(seed);

	struct PlaneHelper {
		vec3f N; f32 D;
	};
	ary<PlaneHelper>  plains(iter, iter);
	for( int i = iter; i--; ) {
		auto &p = plains[i];
		p.N = vec3f(rand.randf()-0.5f, (rand.randf()-0.5f)*1.2f, rand.randf()-0.5f) .normalise();
		p.D = rand.randf() -0.5f;
	}



	int avg = 0; int cnt = 0; int mx = 0, mn = 0;


		
	mat3f sm[] = {
		mat3f::identity(),
		mat3f::rotationY( 90* DEG_TO_RAD ),
		mat3f::rotationY( 180* DEG_TO_RAD ),
		mat3f::rotationY( -90* DEG_TO_RAD ),
		mat3f::rotationX( 90 * DEG_TO_RAD ),
		mat3f::rotationX( -90 * DEG_TO_RAD ),
	};
	for( int s = 6; s--; ) {
		int si = s*dim1*dim1;
		int sAvg = 0;
		for( int y = dim1; y--; ) 
		for( int x = dim1; x--; ) {
			PlanetDat& v = verts[si+x+y*dim1];
			v.Pos = vec3f(((f32)x / (f32)dim) -0.5f, ((f32)y / (f32)dim) -0.5f, 0.5).normalise() * sm[s];
			v.Norm = vec3f(0,0,0);

			//v.Ang = atan2(v.Pos.x, v.Pos.z) /PIf;
			vec3f np = v.Pos *20.0f;
				
			int mod = 0;
			for( int i = iter; i--; ) {
				auto &p = plains[i];
				if( dot( v.Pos, p.N ) > p.D ) {
					//mod += (l1-mod) *m1;
					//mod += m1;
					mod++;
				}  else {
					//mod += (l2-mod) *m2;
					mod--;
				}
			}
			sAvg += mod;
			if(mod > mx) mx = mod;
			else if(mod < mn) mn = mod;
			//radius *= mod;
			//v.Pos *= radius;
			v.D = *(float*)&mod;
			
		}
		avg += sAvg / (dim1*dim1);

		auto tri = [&]( int a, int b, int c ) {
			PlanetDat& v1 = verts[a], &v2 = verts[b], &v3 = verts[c];
			vec3f n = cross((v3.Pos-v1.Pos), (v2.Pos-v1.Pos)).normalise();
			v1.Norm += n;
			n *= 0.5f;
			v2.Norm += n; v3.Norm += n;
		};

		for( int x = dim; x--; )
		for( int y = dim; y--; ) {		
			tri( si+x + y*dim1, si+x+1 + y*dim1, si+x + (y+1)*dim1 ); 
			tri( si+x+1 + (y+1)*dim1, si+x + (y+1)*dim1, si+x+1 + y*dim1 );
		}
	}

	avg /= 6;
	printf("avg  %d mn %d mx %d \n", avg, mn, mx);
	mx -= avg;
	mn = avg -mn;

	float mxf = 0;
	for(int i = verts.count(); i--;) {
		auto &v = verts[i];
		v.Norm.normalise();
		int modI = *(int*)&verts[i].D;
		float mod;
		if(modI > avg) mod = 1.0f - (1.0f-l1)*(float)(modI-avg)/(float)mx;
		else mod = 1.0f - (1.0f-l2)*(avg-modI)/(float)mn;

		///very unoptimal - but simplex is fast so -meh

		float lattitude = atan2f(v.Pos.z, v.Pos.x)/PIf, longatude = asinf(v.Pos.y)/(PIf/2.0f);

		vec3f np = v.Pos;
		f32 ns = octave_noise_3d(8.0f, 0.8f, 1, np.x, np.y, np.z);
		np *= 5.0f;
		v.D = octave_noise_3d(8.0f, 0.6f, 1, np.x, np.y, np.z);
		//f32 ns2 = cosf( ns*PIf );

		vec2f np2 = vec2f(np.x, np.z).normalise(); 
		f32 tb = (np.y>0.0f) ? 1.0f : -1.0f;


		float y = longatude;
		if( y < 0 )  y = -y;
			
		ns *= 4.0f; if(ns > 1.0f) ns = 1.0f; else if(ns< -1.0f) ns = -1.0f; // todo - clamp
				
		float artic1 = 0.9f +0.025f* octave_noise_3d(8.0f, 0.4f, 1.0f, np2.x, np2.y, 0.2f*tb );
		float artic2 = artic1 +0.025f*ns;
		float artic3 = artic1 -0.05f;
		float articM = (y- artic3) / (artic1-artic3);
		if(y > artic1 ) articM = 1;

		float oMod = mod;
		if( y > artic3 ) {
			float m2 = mod; if(m2 < 1.0f) m2 = 1.0f/m2;
			mod += (1.00001f*m2 -mod)*pow(articM,2.5f);
		}
		if(y > artic2 && mod < 1.0f) mod = 1.000001f;

		if( mod < 1.0f ) {
			float lim = 1.0f + (l2-1.0f)*0.1f;
			if( mod > lim ) {	
				v.Col = vec3f(56, 57, 137)/255.0f;
				v.Col *= 1.0f + 0.7f* ((mod - lim)/(1.0f-lim));
			} else 
				v.Col = vec3f(56, 57, 137)/255.0f;

		//mountains... todo..    detect or force add?   ..... wait till tectonic model?
		//} else if( mod > 1.0f + (l1-1.0f)*0.75 ) {							
	//		v.Col = vec3f(143, 127, 101)/255.0f *0.8f;
		} else {
			if( y > artic2 || oMod  < 1.0f) {
				//if( y > artic2 )
					v.Col = vec3f(0.9f,0.9f,1.0f);
				//else {
						
				//	v.Col = vec3f(0.2f, 0.2f, 1.0f) * articM  + vec3f(0.3f, 0.6f, 0.59f) *(1.0f-articM);
				//}						
			} else {
				y = longatude + 0.2f*octave_noise_2d(4.0f, 0.4f, 1.0f, np2.x, np2.y );
				if( y < 0 ) y = -y;


				if( y > 0.35f+0.10f*(octave_noise_3d(8.0f, 0.4f, 1.0f, np2.x, np2.y, 0.3f*tb )+ns) )  {
					v.Col = vec3f(0.3f,0.6f,0.19f);
				} else if( y > 0.1f +0.075f*(octave_noise_3d(8.0f, 0.4f, 1.0f, np2.x, np2.y, 0.7f*tb )+ns) )  {
					v.Col = vec3f(0.99f,0.8f,0.55f);
				} else {
					v.Col = vec3f(0.15f,0.55f,0.08f);
				}
			}	
		}
		//v.Col = vec3f(1,1,1)* ns*0.5f + 0.5f;
		if(mod > mxf) mxf = mod;
		v.Pos *= mod *radius;
				
			

		//v.B = octave_noise_4d(8.0f, 0.5f, 1.0f, np.x, np.y, np.z, 0.0f);
		//float mod = (float)(modI-mn)/(mx-mn);
	}
	printf("avg  %f \n", mxf );

//fix seams
	for( int s = 4; s--; ) {
		int s1 = s*dim1*dim1, s2 = ((s+1)%4)*dim1*dim1;
		int sAvg = 0;
		for( int y = dim1; y--; ) {
			verts[s2+y*dim1] = verts[s1+y*dim1 +dim1-1];
		}
	}
	int sT = 4*dim1*dim1, sB = 5*dim1*dim1, s1 = 0*dim1*dim1, s2 = 2*dim1*dim1, s3 = 1*dim1*dim1, s4 = 3*dim1*dim1;
	for( int i = dim1; i--; ) {
		//break;
		verts[sT+ i + dim*dim1] = verts[s1 + i];
		verts[sT+ i] = verts[s2 +dim-  i];
		verts[sT+ i*dim1 ] = verts[s4 +  i];
		verts[sT+ dim+i*dim1 ] = verts[s3 +dim-  i];

		verts[sB+ i + dim*dim1] = verts[s2 + dim- i+ dim*dim1];
		verts[sB+ i] = verts[s1 + i+ dim*dim1];
		verts[sB+ i*dim1 ] = verts[s4 +dim-  i + dim*dim1 ];
		verts[sB+ dim+i*dim1 ] = verts[s3 +  i+ dim*dim1];
	}//*/

	
	//todo - writing lots of unnessacry stuff ... zilch compression... i feel dirty	
		
	std::fstream myfile;
	myfile.open ("Media//Planet//Alpha07.plnt",  std::ios::out | std::ios::binary | std::ios::trunc);
	
	myfile.write( (char*)&magicNo, sizeof(magicNo) );
	myfile.write( (char*)&version, sizeof(version) );
	myfile.write( (char*)&dim, sizeof(dim) );
	myfile.write( (char*)&seed, sizeof(seed) );
	myfile.write( (char*)&verts[0], sizeof(verts[0])*verts.count() ); 	
	myfile.close();
}

GLint LodModLocation;
	


struct AABB {
	AABB() : Min(HIGHf,HIGHf,HIGHf), Max(LOWf,LOWf,LOWf) {}
	void envelop( const vec3f p ) {
		Min = min(p, Min);
		Max = max(p, Max);
	}

	f32 pntSqrDis( const vec3f &pv ) const {
		f32 ret = 0;
		for( u32 i = 3; i--; ) {
			auto &p = pv[i];
			auto &mn = Min[i];
			auto &mx = Max[i];
			if(p < mn ) ret += pow2(p-mn);
			else if(p > mx ) ret += pow2(p-mx);
		}
		return ret;
	}
	vec3f Min, Max;
};
class Planet;
class PNode : public sListNode<PNode> {
public:
	PNode() {
		VA = 0;
		Level = 0;
	}
	~PNode() {
		clearBuffers();
		clearSubNodes();
	}
	void init(Planet &p, const u32 &tl, const u32 &s, const u32 &x1, const u32 &y1, const f32 &ld);
	void gen(Planet &p, const u32 &dLod ) {
		DesLod = dLod;
		gen(p);
	}
	void checkLod(Planet &p) {
		if( DesLod != Lod ) gen(p);
	}
	void gen(Planet &p );
	void clearBuffers() {
		if( VA ) {
			glDeleteVertexArrays(1, &VA);
			glDeleteBuffers(2, Buffers);
			VA = 0;
			Lod = 0;
		}
	}
	void draw( RenderingCntx &rc ) {
		glBindVertexArray(VA);
		
		glUniform1f(LodModLocation, EffLodDis );
		glDrawElements(GL_TRIANGLES, Lod, GL_UNSIGNED_INT, 0);
	}

	void proc(Planet &p, sList<PNode> &drawList, sList<PNode> &procList, const vec3f &camPos);

	void clearSubNodes() { //todo - not straight clear, cache for awhile
		for( int i = 16; i--; ) {
			auto &n = SubNodes[i].N;
			if(!n) continue;
			n->clearSubNodes();
			delete n;
			n = 0;
		}
	}
	u32 Lod, DesLod;

	f32 LodDis, EffLodDis;
	u32 Tl, Stride, X1, Y1;

	u32 Level;

	GLuint VA;
	union {
		struct { GLuint IB, VB; };
		GLuint Buffers[2];
	};

	struct SubNode {
		PNode *N;
		vec3f Mid;
		AABB BB;
	//	float LodD;
	};
	SubNode SubNodes[16];
};

class Planet {
public:
	class Side : public PNode {
	public:
		vec3f Norm;
		vec3f Corners[4];
		GLuint  Tex;
	};
	Planet() {}
	const ary<PlanetDat> Pd; //todo - stream
	Side Sides[6];

	u32 Res, Res1, Dim, Dim1;
};

Planet Plnt;
bool Init = false;

ary<int,Ctor::Simple> inds;
ary<Vertex2> verts;


void PNode::init( Planet &p, const u32 &tl, const u32 &s, const u32 &x1, const u32 &y1, const f32 &ld ) {
	Tl = tl; Stride = s; X1 = x1;  Y1 = y1; LodDis = ld;
	Lod = 0;
	for( int x = 4; x--; )
	for( int y = 4; y--; ) {
		auto &sn = SubNodes[x+y*4];
		sn.Mid = p.Pd[tl+  Stride*p.Dim/8 +  x*Stride*p.Dim/4 +   (Stride*p.Dim/8 +  y*Stride*p.Dim/4)*p.Res1].Pos;
		sn.N = 0;
		for( int cx = 2; cx--; )
		for( int cy = 2; cy--; ) {
			sn.BB.envelop(p.Pd[tl+(cx+x) * Stride*p.Dim/4 + ((cy+ y)*Stride*p.Dim/4)*p.Res1].Pos);
		}
		sn.BB.envelop(sn.Mid);

		/*		sn.LodD = 0;

		for( int cx = 2; cx--; )
		for( int cy = 2; cy--; ) {
			sn.LodD = max(sn.LodD, (p.Pd[tl+(cx+x) * Stride*p.Dim/2 + ((cy+ y)*Stride*p.Dim/2)*p.Res1].Pos - sn.Mid).sqrLeng());
		}
		sn.LodD = sqrt(sn.LodD)*2.0f;*/
		int a = 0;
	}

	VA = 0;
//	OtherVA = -1;
}
void PNode::proc( Planet &p, sList<PNode> &drawList, sList<PNode> &procList, const vec3f &camPos  ) {

	u32 lod1 = p.Dim *p.Dim *6, lod2 = lod1/4, lod3 = lod2 / 3;
	bool sub = false;
	if( Stride > 1  ) {
		
		/*
		for( u32 y =3; y--; ) 
		for( u32 x =3; x--; ) {
			if((p.Pd[Tl+x * Stride*p.Dim/2 + (y*Stride*p.Dim/2)*p.Res1].Pos-camPos).sqrLeng() < LodDis*LodDis) sub = true; //todo - store flasgs per corner
		} */

		//*
		for( int i = 16; i--; ) {
			auto &sn = SubNodes[i];
			f32 dis = sn.BB.pntSqrDis(camPos);
			if( dis  < pow2(LodDis) ) {
				sub = true;				
				if( sn.N == null ) {
					auto& n = *(sn.N = new PNode());
					u32 y = i /4, x = i - y*4;
					u32 x1 = X1 + x*Stride*p.Dim/4, y1 = Y1 + y*Stride*p.Dim/4;
					n.init(p, Tl  + x*Stride*p.Dim/4 + (y*Stride*p.Dim/4)*p.Res1, Stride/4, x1, y1, LodDis*0.25f );
					n.Level = Level + 2;

				} 	
				if( dis <  pow2(LodDis*0.5f) ) {
					sn.N->DesLod = lod1;
					procList.add(sn.N);
				} else
					sn.N->DesLod = lod2; 

			} else if( sn.N ) 
				sn.N->DesLod = lod3; 
		} //*/
	}
	if(sub) {
		clearBuffers();

		for( u32 i =16; i--; ) { 
			auto &sn = SubNodes[i];

			if( sn.N == null ) {
				auto& n = *(sn.N = new PNode());
				u32 y = i /4, x = i - y*4;
				u32 x1 = X1 + x*Stride*p.Dim/4, y1 = Y1 + y*Stride*p.Dim/4;
				n.init(p, Tl  + x*Stride*p.Dim/4 + (y*Stride*p.Dim/4)*p.Res1, Stride/4, x1, y1, LodDis*0.25f );

				n.Level = Level + 2;
				n.gen(p, lod3);
			} else  if( sn.N->DesLod != lod1 ) {
				
				sn.N->clearSubNodes();
				sn.N->checkLod(p);
				drawList.add(sn.N);
				int a = 0;
			} 
		}

/*		for( u32 y =2; y--; ) 
		for( u32 x =2; x--; ) {  //todo back to i
			auto &sn = SubNodes[x+y*2];
			u32 dLod = lod2;

			for( int cx = 2; cx--; )
			for( int cy = 2; cy--; ) {
				if( (p.Pd[Tl+(cx+x) * Stride*p.Dim/2 + ((cy+ y)*Stride*p.Dim/2)*p.Res1].Pos - camPos).sqrLeng() < LodDis*LodDis ) {
					dLod = lod1;
					break; //todo jumpout
				}
			} 
			if( sn.N == null ) {
				auto& n = *(sn.N = new PNode());
				u32 x1 = X1 + x*Stride*p.Dim/2, y1 = Y1 + y*Stride*p.Dim/2;
				n.init(p, Tl  + x*Stride*p.Dim/2 + (y*Stride*p.Dim/2)*p.Res1, Stride/2, x1, y1, LodDis*0.5f );;
			}  
			sn.N->DesLod = dLod;
			
			if( dLod == lod2 ) {
				sn.N->clearSubNodes();
				sn.N->checkLod(p);
				drawList.add(sn.N);
				int a = 0;
			} else {
				procList.add(sn.N);
			}
			
		}	*/	
	} else {
		clearSubNodes();
		checkLod(p);
		drawList.add(this);
	}
}
void PNode::gen(Planet &p ) {

	clearBuffers();

	u32 lod1 = p.Dim *p.Dim *6, lod2 = lod1/4, lod3 = lod2 / 3;

	u32 stride = Stride, iw = p.Dim; 
	EffLodDis = LodDis;
	f32 lm = (f32)(Level & 1);

	f32 cm = 0.5f;

	if( ((X1  / (Stride*p.Dim) ) & 1)  != ((Y1  / (Stride*p.Dim)) & 1)) cm = 1.5f;

	if( DesLod == lod2 ) {
		iw >>= 1; stride <<= 1;
		EffLodDis *= 2.0f;
		lm = 1- lm;
	} else if( DesLod == lod3 ) { 
		iw >>= 2; stride <<= 2;
		EffLodDis *= 4.0f;
	}

	u32 vw = iw+1;

	for( int y = vw; y--; )
	for( int x = vw; x--; ) {
		auto &v = verts[x+y*vw];
		*(PlanetDat*)&v = p.Pd[Tl+ x*stride + y*stride*p.Res1];
		v.Uv = vec2f( (f32)(X1+x*stride), (f32)(Y1+y*stride)) /(f32)p.Res;
		v.Pos2 = v.Pos;
		v.Col = vec4f(1.0f, 1.0f, 1.0f, 1.0f);

		v.Ang = lm;
		v.B = cm;
	}

	//if( Stride != p.Res/ p.Dim ) {

		//optimizable
		for( int y = vw; y--; )
		for( int x = vw; x--; ) {
			u32 vi = x+y*vw;
			auto &v = verts[vi];		
			if(x & 1) {			
				if(y & 1) {
					v.Pos2 = (verts[vi-vw].Pos + verts[vi+vw].Pos+verts[vi-1].Pos + verts[vi+1].Pos ) *0.25f;
				} else 
					v.Pos2 = (verts[vi-1].Pos + verts[vi+1].Pos) *0.5f;
			} else if(y & 1) {
				v.Pos2 = (verts[vi-vw].Pos + verts[vi+vw].Pos) *0.5f;
			}

		}

	//}

	auto tri = [&]( int a, int b, int c ) {
		inds.add( a );
		inds.add( b );
		inds.add( c );
	};
	for( int x = iw; x--; )
	for( int y = iw; y--; ) {		
		tri( x + y*vw, x+1 + y*vw, x + (y+1)*vw ); 
		tri( x+1 + (y+1)*vw, x + (y+1)*vw, x+1 + y*vw );
	}
	//todo - only actually need 2 index buffers for whole planet - share

	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);
		
	glGenBuffers(2, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, verts.count()*sizeof(Vertex2), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (Lod = inds.count())*sizeof(int), &inds[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), NULL);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2),  (void**)(sizeof(vec3f)*2 ));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2),  (void**)(sizeof(PlanetDat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2),  (void**)(sizeof(vec3f)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2),  (void**)(sizeof(PlanetDat)+sizeof(vec2f)));

	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void**)(sizeof(vec3f) + sizeof(vec4f)));
	inds.clear();
	glBindVertexArray(0);
}


bool debugFlag(const u32 &i);

void DrawTestCube::proc(RenderingCntx &rc) {
//	return;

	auto &p = Plnt;

	u32 res = 1024, dim = 16, ic = dim*dim*6;
	
	if( !Init ) {
		Init = true;
		
		p.Res = res; p.Dim = dim; p.Res1 = p.Res +1; p.Dim1 = p.Dim +1;

		ary<PlanetDat> &pd = *(ary<PlanetDat> *)&p.Pd;
		getPlanet(res, pd );
		
		u32 stride = p.Res/dim;

		verts.addMul(  p.Dim1*p.Dim1);
		inds.enSpace( dim*dim*6 );
		ary<u8> imageBuf(p.Res*p.Res*3,p.Res*p.Res*3);

		for( int si = 6; si--; ) {	
			auto &s = p.Sides[si];
			u32 svi = si*p.Res1*p.Res1;
			s.Norm = pd[svi+ p.Res/2 + p.Res/2*p.Res1].Pos.getNormal();
			s.Corners[0] = pd[svi+ 0].Pos;
			s.Corners[1] = pd[svi+ p.Res].Pos;
			s.Corners[2] = pd[svi+ p.Res+p.Res*p.Res1].Pos;
			s.Corners[0] = pd[svi+ p.Res*p.Res1].Pos;

			
			s.init(p, svi, stride, 0, 0, (pd[svi+ p.Res/2 + p.Res/2*p.Res1].Pos- s.Corners[0]).leng()*2.0f );// (pd[svi+ p.Res/2 + p.Res/2*p.Res1].Pos - s.Corners[3]).leng()  );
			s.DesLod =  dim*dim*6;


			glGenTextures(1, &s.Tex);
			glBindTexture(GL_TEXTURE_2D, s.Tex);
        
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 	
			//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

			GLenum format = GL_RGB;
			GLenum intFormat = GL_RGB8;	
			u32 formatSize = 3;

			for( int x = p.Res; x--; )
			for( int y = p.Res; y--; ) {
				u8* pix = &imageBuf[(x+y*p.Res)*3];
				vec3u8 c = (vec3u8)(vec3s)(pd[si*p.Res1*p.Res1+ x+y*p.Res1].Col*255.0f);
				(*(vec3u8*)pix) = c;
			}

			glTexImage2D(
				GL_TEXTURE_2D, 0, intFormat, p.Res, p.Res,
				0, format, GL_UNSIGNED_BYTE, &imageBuf[0]
			);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
			//*/
		

		}

	}
	
	//return;
	if(debugFlag(0)) {
		glDisable(GL_CULL_FACE);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	if(debugFlag(1)) glDisable(GL_CULL_FACE);

	auto projMatrix = mat4f::projection(90.0f * DEG_TO_RAD, 1024.0f/768.0f,0.001f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto mvp = Trans.as<mat4f>()*projMatrix;// *worldMatrix;
	Prog.apply(rc, mvp);



	Tex.Hdwr->apply(rc);

	vec3f camPos = vec3f(0, 0, 0)* Trans.inverse();
	
	vec3f surfN = camPos.getNormal(), surfP = surfN*15.0f;
	//camPos = (camPos + surfP*3.0f)*0.25f;
	GLint camPosLocation = glGetUniformLocation(Prog.Prog, "CamPos");
	LodModLocation = glGetUniformLocation(Prog.Prog, "LodMod");
	glUniform3fv(camPosLocation, 1, &camPos.x );


	//debugPosition() = surfP;// p.normalise() *15.0f;
	//debugPosition() = P.Pd[2*p.Res1*p.Res1+ p.Res/2 + p.Res/2*p.Res1].Pos;
	static int iter = 0; iter++;

	

	for( int si = 6; si--; ) {	
		//si = 2;
		auto &s = p.Sides[si];
		
		//if( si == 2 ) debugPosition() = s.SubNodes[ (iter/1024) %4].Mid;// *Trans.inverse();
		//debugPosition() = vec3f(0, 0, 0) *Trans.inverse();

		//if(dot(s.Norm, s.Corners[0].getNormal()) >  dot(s.Norm, surfP.getNormal())) goto label_breakContinue;

		
		for( int i = 4; i--; ) {
			if(dot(surfN, s.Corners[i]) > 0) goto label_jumpOut;
		}
		continue;


		label_jumpOut:;
		if( si == 2 ) {
		//	printf("d %f  %f  %f  %f  \n", (s.SubNodes[0].Mid- camPos).leng(), (s.SubNodes[1].Mid- camPos).leng(), (s.SubNodes[2].Mid- camPos).leng(), (s.SubNodes[3].Mid- camPos).leng());
		}

		
		sList<PNode> drawList, procList; procList.add(&s);
		for(PNode *n; !procList.isEmpty();) {
			n = procList.pop();
			n->proc(p, drawList, procList, camPos);
		}
		if(drawList.isEmpty()) continue;

		glBindTexture(GL_TEXTURE_2D, s.Tex);
		for(PNode *n; !drawList.isEmpty();) {
			n = drawList.pop();
			n->draw(rc);
		}

	}



	glBindVertexArray(0);

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glEnable(GL_CULL_FACE);
}
