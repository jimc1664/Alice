#include "stdafx.h"

#include "Gem/Dis/DrawList.h"
#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "Gem/Dis/Texture.h"

#include "OGL_Task.h"

#include "Gem/ConCur/ConCur.h"


#include "Gem/Scene2/Sprite.h"
#include "Gem/Scene2/Texture.h"


DrawSprite::DrawSprite( Scene2::Sprite* spr ) : Tex( *spr->Tex ){

	Trans.setTransformation(spr->Pos, spr->Rotation, spr->Size);
}

void DrawSprite::proc(RenderingCntx &rc) {

	Tex.Hdwr->apply(rc);

	rectf uv = Tex.Section;
	glBegin(GL_QUADS);
		glTexCoord2f(uv.x,uv.w);
		glVertex2fv( (vec2f(-0.5f,0.5f)*Trans).ptr() );
		glTexCoord2f(uv.z,uv.w);
		glVertex2fv( (vec2f(0.5f,0.5f)*Trans).ptr() );
		glTexCoord2f(uv.z,uv.y); 
		glVertex2fv( (vec2f(0.5f,-0.5f)*Trans).ptr() );
		glTexCoord2f(uv.x,uv.y); 
		glVertex2fv( (vec2f(-0.5f,-0.5f)*Trans).ptr() );

	glEnd();


}


void DrawList::render( RenderingCntx &rc ) {

	procAll(&rc);
}


BufferedDrawList::BufferedDrawList() {
	ToRender = &Dl[0];	
	Ui = 2;
	/*
	static_assert( sizeof(Offset) == sizeof(u32), "err" );
	Inds.B[0] = 0;
	Inds.B[1] = 1;
	Inds.B[2] = 2; */
}

BufferedDrawList::DrawListRef BufferedDrawList::forUpdate() {

	DrawList* dl;
	for(;;) {
		dl = &Dl[Ui];
		if(--Ui == 0) Ui = 2;
		if (dl == Rendering || dl == ToRender)continue;
		break;
	}
	dl->clear();
	return DrawListRef( *dl, *this);
}

void BufferedDrawList::render(RenderingCntx &rc) {

	Rendering = ToRender;

	Rendering->render(rc);

	Rendering = null;

	/*
	for(;;) {
		Offset o = Inds, n = o;
		n.B[Render] = o.B[Mid];
		n.B[Mid] = o.B[Render];

		if(!ConCur::compareSwap(Inds.I, (u32)n.I, (u32)o.I)) continue;

		Dl[n.B[Render]].render(dm);
		break;
	}*/

	
}


void BufferedDrawList::doneUpdate( DrawList& dl ) {

	ToRender = &dl;
	/*
	for(;;) {
		Offset o = Inds, n = o;
		n.B[Update] = o.B[Mid];
		n.B[Mid] = o.B[Update];

		if(!ConCur::compareSwap(Inds.I, (u32)n.I, (u32)o.I)) continue;

		Dl[n.B[Update]].clear();
		break;
	}
	*/
}
