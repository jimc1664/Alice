#version 150

#include "commonDefferedFS.glsl"


in vec2 UV;

uniform	sampler2D texture0;

void main() {

	vec4 col = texture(texture0,	UV );
	vec2 uv = (VPos.xy + 1)*vec2(0.5,0.5);
	vec4 lightDat = texture(LightBuff, uv );
	col.rgb = col.rgb * lightDat.rgb + lightDat.rgb *lightDat.a; 
	FragColor = col;
}
