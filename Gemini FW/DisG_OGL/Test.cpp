#include "stdafx.h"

#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "Gem/Dis/DrawList.h"
#include "Gem/Dis/Texture.h"
#include "Gem/Dis/Mesh.h"
#include "Gem/Dis/ShaderProg.h"

#include "OGL_Task.h"

#include "Gem/ConCur/ConCur.h"

#include "Gem/Math/vec3.h"
#include "Gem/Math/vec4.h"
#include "Gem/Math/matrix4.h"
#include <Gem/Math/Randomizer.h>
#include "Gem/Org/Ary.h"

#include "Gem/Scene3/TestObj.h"
#include "Gem/Scene3/Texture.h"
#include "Gem/Scene3/Passive.h"
#include "Gem/Scene3/Mesh.h"


struct Vertex2
{
	vec3f Pos, Norm;  
	float Ang, B, C, D;
};


GLuint VA = -1;
int IC;

void DrawTestCube::proc(RenderingCntx &rc) {
//	return;

//	return;
	glDisable(GL_CULL_FACE);

	if( VA  == -1 ) {
		glGenVertexArrays(1, &VA);
		glBindVertexArray(VA);

		GLuint vb, ib;
		glGenBuffers(1, &vb);
		glBindBuffer(GL_ARRAY_BUFFER, vb);

		int iter = 1000; float m1 = 0.0005f, m2 = m1*0.975f, l1 = 1.2f, l2 = 0.8f;
		Randomizer rand(2);

		struct PlaneHelper {
			vec3f N; f32 D;
		};
		ary<PlaneHelper>  plains(iter, iter);
		for( int i = iter; i--; ) {
			auto &p = plains[i];
			p.N = vec3f(rand.randf()-0.5f, (rand.randf()-0.5f)*1.2f, rand.randf()-0.5f) .normalise();
			p.D = rand.randf() -0.5f;
		}


		int dim = 200, vc = dim*dim ;  IC = (dim-1)*(dim-1) *6;

		ary<Vertex2, Ctor::SimpleZeroed> verts(vc*6 );
		ary<int,Ctor::Simple> inds(IC*6 );
		
		mat3f sm[] = {
			mat3f::identity(),
			mat3f::rotationY( 90* DEG_TO_RAD ),
			mat3f::rotationY( 180* DEG_TO_RAD ),
			mat3f::rotationY( -90* DEG_TO_RAD ),
			mat3f::rotationX( 90 * DEG_TO_RAD ),
			mat3f::rotationX( -90 * DEG_TO_RAD ),
		};
		for( int s = 6; s--; ) {
			int ia = verts.count();
			for( int y = dim; y--; ) 
			for( int x = dim; x--; ) {
				Vertex2 v;
				v.Pos = vec3f(((f32)x / (f32)(dim-1)) -0.5f, ((f32)y / (f32)(dim-1)) -0.5f, 0.5).normalise() * sm[s];
				v.Norm = vec3f(0,0,0);

				v.Ang = atan2(v.Pos.x, v.Pos.z) /PIf;

				float radius = 15.0f, mod = 1.0f;
				for( int i = iter; i--; ) {
					auto &p = plains[i];
					if( dot( v.Pos, p.N ) > p.D ) {
						mod += (l1-mod) *m1;
					}  else {
						mod += (l2-mod) *m2;
					}
				}
				radius *= mod;
				v.Pos *= radius;
				verts.add(v);
			}

	

			auto tri = [&]( int a, int b, int c ) {
				inds.add( a );
				inds.add( b );
				inds.add( c );

				Vertex2& v1 = verts[a], &v2 = verts[b], &v3 = verts[c];
				vec3f n = cross((v3.Pos-v1.Pos), (v2.Pos-v1.Pos)).normalise();
				v1.Norm += n;
				n *= 0.5f;
				v2.Norm += n; v3.Norm += n;
			};
			for( int x = dim-1; x--; )
			for( int y = dim-1; y--; ) {		
				tri( ia+x + y*dim, ia+x + (y+1)*dim, ia+x+1 + y*dim ); 
				tri( ia+x+1 + (y+1)*dim, ia+x+1 + y*dim, ia+x + (y+1)*dim );
			}
		}
		vc *= 6; IC *= 6;

		for(int i = vc; i--;) verts[i].Norm.normalise();

		glBufferData(GL_ARRAY_BUFFER, vc*sizeof(Vertex2), &verts[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IC*sizeof(int), &inds[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), NULL);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2),  (void**)(sizeof(vec3f)*2));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2),  (void**)(sizeof(vec3f)));

		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void**)(sizeof(vec3f) + sizeof(vec4f)));

		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	auto projMatrix = mat4f::projection(70.0f * DEG_TO_RAD, 1024.0f/768.0f,0.1f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto mvp = Trans.as<mat4f>()*projMatrix;// *worldMatrix;
	Prog.apply(rc, mvp);

	Tex.Hdwr->apply(rc);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, 0);
	/*
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void**)(sizeof(vec3f)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void**)(sizeof(vec3f) + sizeof(vec4f)));
		*/
	glBindVertexArray(VA);
	glDrawElements(GL_TRIANGLES, IC, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}
