#ifndef SKYBOX_H
#define SKYBOX_H

#include "gameobj.h"

enum TSkyBoxTexDir{ESkyBoxTop, ESkyBoxBottom, ESkyBoxLeft, ESkyBoxRight, ESkyBoxFront, ESkyBoxBack};

class CSkyBox
{
public:
				CSkyBox			();
				~CSkyBox		();
	void		setTexture		(TSkyBoxTexDir aDir, LPDIRECT3DTEXTURE9 aTex);
	void		setTextures		(LPDIRECT3DTEXTURE9 *aTex);
	void		draw			();
	void		restore			();
	void		release			();
private:
	LPDIRECT3DVERTEXBUFFER9	mVB;
	LPDIRECT3DTEXTURE9		mTex[6];
};

#endif // SKYBOX_H