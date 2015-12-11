#include "stdafx.h"

#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "Gem/Dis/DrawList.h"
#include "Gem/Dis/Texture.h"
#include "Gem/Dis/Mesh.h"
#include "Gem/Dis/ShaderProg.h"


#include "Gem/Math/vec3.h"
#include "Gem/Math/vec4.h"
#include "Gem/Math/matrix4.h"

#include "Gem/Scene3/Texture.h"
#include "Gem/Scene3/Passive.h"
#include "Gem/Scene3/Mesh.h"
#include "Gem/Scene3/Material.h"



void DrawPassive::proc(RenderingCntx &rc) {
//	return;
	auto projMatrix = mat4f::projection(45.0f * DEG_TO_RAD, 1024.0f/768.0f,0.001f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto mvp = Trans.as<mat4f>()*projMatrix;// *worldMatrix;


	Dat.Mat.Prog.apply(rc, mvp);

	glActiveTexture(GL_TEXTURE0);
	Dat.Mat.Tex.Hdwr->apply(rc);

	Dat.Mesh.Hdwr->draw(rc);
}

