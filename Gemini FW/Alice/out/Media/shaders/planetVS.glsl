#version 150

in vec3 vertexPosition;
in vec2 vertexTexCoords;
in vec3 vertexNorm;
in vec4 vertexCol;

out vec2 UV;
out vec3 Norm;
out vec3 Pos;
out vec4 Col;
out float Radius;

uniform mat4 MVP;
//uniform mat4 MVP;

void main()
{	
	Col = vertexCol;
	UV=vertexTexCoords;
	Radius = length( vertexPosition );
	Norm = vertexNorm;
	Pos = vertexPosition;
	vec3 pos = vertexPosition;
	if( Radius <= 15.0 ) {
		pos *= 15/ Radius;
	}

	gl_Position = MVP * vec4(pos, 1.0);
}
