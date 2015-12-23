#version 150

#include "common.glsl"

out vec4 FragColor;
in vec3 Norm;
in vec3 VPos;



void main() {

	vec3 nrm = normalize(  Norm )*0.5 +vec3(0.5,0.5,0.5);
	vec3 pos = VPos*0.5 +vec3(0.5,0.5,0.5);
	FragColor = vec4(nrm.x, nrm.y, zPack(pos.z) );
	//FragColor = vec4(nrm.x, nrm.y, pos.z, 0 );
	
	//FragColor = vec4(nrm.x, nrm.y, nrm.z, 0 );
	//FragColor = vec4(pos.x, pos.y, pos.z, 0 );
}
