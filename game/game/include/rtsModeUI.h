#ifndef RTSMODEUI_H
#define RTSMODEUI_H

#include "gameUI.h"
#include "texture.h"

class CRTSModeUI : public MGameUI
{
public:
	CRTSModeUI();
	~CRTSModeUI();

	void draw(uint32 aTime);
	void handleInput();
	void activate(CGameObjPtr aTarget);
private:
	bool mActive;
	int mCounter;
	CGameObjPtr mSelected;
	MAnimation *mSelectedAnim;
	ID3DXSprite *mSprite;
	CTexture *mMouseCursor;
	CTexture *mMenuTexture;
	IDirect3DTexture9 *mMinimapTexture;
};

#endif // RTSMODEUI_H
