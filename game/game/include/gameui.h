#ifndef GAMEUI_H
#define GAMEUI_H

#include "texture.h"

class CGameUI
{
public:
	CGameUI();
	~CGameUI();

	void draw();
private:
	ID3DXSprite *mSprite;
	CTexture *mMouseCursor;
};

//extern CGameUI *gameUI;

#endif // GAMEUI_H
