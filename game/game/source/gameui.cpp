#include "../include/gameui.h"
#include "../include/directInput.h"
#include "../include/animationStorage.h"

//CGameUI *gameUI;

CGameUI::CGameUI() : mSprite(NULL), mMouseCursor(NULL)
{
//	gameUI = this;
    D3DXCreateSprite(d3dObj->mD3DDevice, &mSprite);
	mMouseCursor = CAnimationStorage::ptr()->getTexture("data/cursor.bmp");
}

CGameUI::~CGameUI()
{
//	gameUI = NULL;
	if (mSprite)
		mSprite->Release();
}

void CGameUI::draw()
{
//	const DIMOUSESTATE2 *mouseState = directInput->getMouseState();
	mSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	mSprite->Draw(*mMouseCursor, NULL, NULL,
		&D3DXVECTOR3((float)directInput->getMouseX(), (float)directInput->getMouseY(), 0), 0xFFFFFFFF);
	mSprite->End();
}
