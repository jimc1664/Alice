
#include "commonDeffered.glsl"

in vec3 Norm;
in vec3 VPos;

/*  typical deffered
layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 
layout (location = 3) out vec3 TexCoordOut; 
*/

out vec4 FragColor;


uniform	sampler2D LightBuff;

