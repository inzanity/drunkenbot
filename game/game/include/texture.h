#ifndef TEXTURE_H
#define TEXTURE_H

#include "d3dUtil.h"

class CTexture
{
public:
						CTexture	(const char *aFileName);
	virtual				~CTexture	();
	operator LPDIRECT3DTEXTURE9		();
	void				release		();
	void				restore		(const char *aFileName);
	int					width		();
	int					height		();
private:
	LPDIRECT3DTEXTURE9	mTexture;
};

#endif // TEXTURE_H