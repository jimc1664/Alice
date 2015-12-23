#version 150

in vec3 vertexPosition;

out vec2 UV;
out vec3 Pos;

uniform mat4 MVP;

void main() {
	UV=(vertexPosition.xy + 1)*vec2(0.5,0.5);


	gl_Position = vec4( Pos = vertexPosition, 1.0);
	
}
