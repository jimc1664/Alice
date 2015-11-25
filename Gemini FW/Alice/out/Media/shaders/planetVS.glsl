#version 150

in vec3 vertexPosition;
in vec3 vertexPosition2;
in vec2 vertexTexCoords;
in vec3 vertexNorm;
in vec4 vertexCol;

out vec2 UV;
out vec3 Norm;
out vec3 Pos;
out vec4 WPos;
out vec4 Col;
out float Radius;

uniform mat4 MVP;
uniform vec3 CamPos;
uniform float LodMod;
//uniform mat4 MVP;

void main()
{	
	Col = vertexCol;
	UV=vertexTexCoords;
	Radius = length( vertexPosition );
	Norm = vertexNorm;
	Pos = vertexPosition;
	vec3 pos = vertexPosition;
	//pos += (vertexPosition - vertexPosition2)*2;
	if( Radius <= 15.0 ) {
		pos *= 15/ Radius;
	} else {
	
		float dis = length(CamPos - pos );	
		float d1 = LodMod*2.0, d2 = LodMod*1.5f;
		if( dis > d2 ) {
			if( dis < d1 ) {
				pos += (vertexPosition2-pos) * (dis - d2)/(d1-d2);
			} else 
				pos = vertexPosition2;
		}
	}
	//pos = vertexPosition2;
	WPos = gl_Position = MVP * vec4(pos, 1.0);
	WPos /= WPos.w;
}
