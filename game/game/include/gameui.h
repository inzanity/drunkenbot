#ifndef GAMEUI_H
#define GAMEUI_H

#include "texture.h"
#include "gameobj.h"

class CGameUI
{
public:
	CGameUI();
	~CGameUI();

	void draw(uint32 aTime);
	void handleInput();
	CGameObjPtr selected();
private:
	CGameObjPtr mSelected;
	MAnimation *mSelectedAnim;
	ID3DXSprite *mSprite;
	CTexture *mMouseCursor;
	CTexture *mMenuTexture;
	IDirect3DTexture9 *mMinimapTexture;
};

//extern CGameUI *gameUI;

#endif // GAMEUI_H
