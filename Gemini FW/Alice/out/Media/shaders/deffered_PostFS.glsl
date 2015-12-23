#version 150

#include "common.glsl"

out vec4 FragColor;

in vec3 Pos;
in vec2 UV;

uniform	sampler2D texture0;
uniform mat4 MVP;  //actually this is the view...

void main() {

	vec4 smp = texture(texture0, UV);

	FragColor = smp;
}
