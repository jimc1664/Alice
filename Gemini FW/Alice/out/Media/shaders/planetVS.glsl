#version 150

#include "commonDefferedVS.glsl"

in vec3 vertexPosition2;
in vec4 vertexCol;

out vec3 Pos;
out vec4 Col;
out float Radius;

uniform vec3 CamPos;
uniform float LodMod;

void main() {	
	Col = vertexCol;
	UV=vertexTexCoords;
	Radius = length( vertexPosition );
	Norm = mat3(MV)* normalize( vertexNorm );
	Pos = vertexPosition;
	//Norm = mat3(MV)* normalize( Pos );
	vec3 pos = vertexPosition;

//	if( Radius <= 15.0 ) {
//		pos *= 15/ Radius;
	//} else {
	
		float dis = length(CamPos - pos );	
		float d1 = LodMod*2.0, d2 = LodMod*1.5f;
		if( dis > d2 ) {
			if( dis < d1 ) {
				pos += (vertexPosition2-pos) * (dis - d2)/(d1-d2);
			} else 
				pos = vertexPosition2;
		}
//	}
	//pos = vertexPosition2;
	vec4 vPos = gl_Position = MVP * vec4(pos, 1.0);	
	VPos = vPos.xyz /  vPos.w;
}
