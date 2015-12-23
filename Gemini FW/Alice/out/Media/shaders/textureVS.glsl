#version 150

#include "commonDefferedVS.glsl"


void main() {
	UV =vertexTexCoords;
	Norm = normalize(  mat3(MV)*vertexNorm );
	vec4 vPos = gl_Position = MVP * vec4(vertexPosition, 1.0);	
	VPos = vPos.xyz /  vPos.w;	
}
