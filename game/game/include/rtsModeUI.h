#ifndef RTSMODEUI_H
#define RTSMODEUI_H

#include "gameUI.h"
#include "texture.h"
#include "player.h"
#include "mech.h"

enum MenuItem{ITEM_NONE, ITEM_BUILDING, ITEM_MOVE, ITEM_PATROL, ITEM_ATTACK, ITEM_STOP, ITEM_SELECT_WEAPON, ITEM_SELECT_MODE};

class CRTSModeUI : public MGameUI
{
public:
	CRTSModeUI(const CPlayer *aPlayer);
	~CRTSModeUI();

	void draw(uint32 aTime);
	void handleInput();
	void activate(CGameObjPtr aTarget);
private:
	int getNumMenuItems();
private:
	const CPlayer *mPlayer;
	bool mActive;
	int mCounter;
	CGameObjPtr mSelected;
	MAnimation *mSelectedAnim;
	ID3DXSprite *mSprite;
	CTexture *mMouseCursor;
	CTexture *mMenuTexture;
	CTexture *mMechCommandTextures[ENumMechCommands];
	CTexture *mCancelTexture;
	IDirect3DTexture9 *mMinimapTexture;
	int mQueuePos;
	int mMenuPos;
	MenuItem mSelectedMenuItem;
	int mSelectedMenuItemIndex;
	int mButtonPressedTimer;
};

#endif // RTSMODEUI_H
