#version 150

#include "common.glsl"

out vec4 FragColor;

in vec3 Pos;
in vec2 UV;

uniform	sampler2D texture0;
uniform mat4 MVP;  //actually this is the view...

vec4 lighting( vec3 nrm, vec3 ld, float att  ) {
	
	float specPow = 10;
	vec3 lightCol = vec3( 1,1,0.8 );
	
	vec3 vn = vec3(0.0, 0.0, 1.0);
	float dt = dot(ld, nrm ); 
	
	float spec = pow( clamp( dot(reflect(vn, nrm), ld), 0, 1), specPow );
	return vec4( lightCol*att, spec ) * (dt); 
	//float spec = dot(reflect(vn, nrm), ld);
	//return vec4( lightCol * (dt*att), spec ); 
}

void main() {

	vec4 smp = texture(texture0, UV);
	vec3 nrm = vec3(smp.x,smp.y, 0)*2 - 1; 
	nrm.z = -sqrt( 1- dot(nrm.xy,nrm.xy) );
	
	float depth = zUnpack( smp.zw );
	
	vec3 pos = normalize( Pos.xyz ) *depth;
	
	vec4 ambient = vec4(0.9,0.9,1, 0)*0.3;
	FragColor = ambient + max( lighting( nrm, mat3(MVP)*-normalize( vec3( 1,-2,10) ), 0.8 ), vec4(0,0,0,0) ) ;
	//FragColor =vec4( (pos+1)*0.5, 1 );
	//FragColor =vec4( (nrm+1)*0.5, 1 );  
	//FragColor = smp;
}
