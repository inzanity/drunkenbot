#include "../include/rtsModeUI.h"
#include "../include/directInput.h"
#include "../include/animationStorage.h"
#include "../include/game.h"

CRTSModeUI::CRTSModeUI(const CPlayer *aPlayer) :
	mPlayer(aPlayer), mActive(false), mCounter(0), mSprite(NULL), mMouseCursor(NULL), mQueuePos(0),
	mMenuPos(0), mSelectedMenuItem(ITEM_NONE), mSelectedMenuItemIndex(0)
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
    D3DXCreateSprite(device, &mSprite);
	mMouseCursor = CAnimationStorage::ptr()->getTexture("data/cursor.png");
	mMenuTexture = CAnimationStorage::ptr()->getTexture("data/rts.bmp");
	mSelectedAnim = CAnimationStorage::ptr()->getAnimation("../tools/particleEditor/particleEffects/selection.lua");
	mMechCommandTextures[MECH_MOVE] = CAnimationStorage::ptr()->getTexture("data/images/move_button.bmp");
	mMechCommandTextures[MECH_PATROL] = CAnimationStorage::ptr()->getTexture("data/images/patrol_button.bmp");
	mMechCommandTextures[MECH_ATTACK] = CAnimationStorage::ptr()->getTexture("data/images/attack_button.bmp");
	mMechCommandTextures[MECH_STOP] = CAnimationStorage::ptr()->getTexture("data/images/stop_button.bmp");
	mCancelTexture = CAnimationStorage::ptr()->getTexture("data/images/cancel_button.bmp");
	mSelected.setPtrToNULL();
	D3DXCreateTexture(device, 128, 128, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &mMinimapTexture);

	D3DXMATRIX projectionMatrix;
	float h = (float)game->mHeightMap->horizontalSize() - 1;
	float v = (float)game->mHeightMap->verticalSize() - 1;
	D3DXMatrixOrthoLH(&projectionMatrix, h, v, h/v, 150.0f);
//	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI/2, 1.f, 1.0f, 150.0f);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	d3dObj->mMatrixStack->LoadIdentity();
	d3dObj->mMatrixStack->Translate(-h/2.f, -v/2.f, -v/2.f);
	d3dObj->mMatrixStack->RotateYawPitchRoll(0, -D3DX_PI / 2, 0);
	IDirect3DSurface9 *orig, *minimapSurface;
	device->GetRenderTarget(0, &orig);
	mMinimapTexture->GetSurfaceLevel(0, &minimapSurface);
	device->SetRenderTarget(0, minimapSurface);
	device->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	device->BeginScene();
	game->mHeightMap->draw(0);
	device->EndScene();
	device->SetRenderTarget(0, orig);
}

CRTSModeUI::~CRTSModeUI()
{
	if (mSprite)
		mSprite->Release();
}

void CRTSModeUI::handleInput()
{
	if (!mActive) return; /* Active UI will handle input */
	int mouseX = directInput->getMouseX(), mouseY = directInput->getMouseY();
	MColliding *selected = (CDrawable *)mSelected.ptr();
	CMech *selectedMech = game->getObjType(mSelected) == ETypeMech ? (CMech *)selected : NULL;
	CBuilding *selectedBuilding = game->getObjType(mSelected) == ETypeBuilding ? (CBuilding *)selected : NULL;
	if (directInput->checkKey(KEY_1) && selectedMech)
	{
		mActive = false;
		game->mFPSModeUI->activate(mSelected);
		return;
	}
	if (!mCounter) return; /* Controlling requires 100% RTS mode */
	if (directInput->checkMouseButton(0))
	{
		if (mouseX > d3dObj->width() - 148)
		{
			if (mouseY >= 242 && mouseY < 242 + 5 * 70)
			{
				int num = getNumMenuItems();
				int ind = 2 * mMenuPos + (int)((mouseY - 242) / 70) * 2 + (mouseX > d3dObj->width() - 148 + 70 ? 1 : 0);
				if (ind < num)
				{
					mSelectedMenuItemIndex = ind;
					if (mSelectedMenuItem == ITEM_NONE)
					{
						if (selectedMech)
						{
							if (ind < NUM_MECH_COMMANDS)
								mSelectedMenuItem = (MenuItem)(ITEM_MOVE + ind);
						}
						else if (selectedBuilding)
						{
						}
						else if (!selected)
							mSelectedMenuItem = ITEM_BUILDING;
						return;
					}
					else if (mSelectedMenuItem == ITEM_SELECT_WEAPON)
					{
					}
					else if (mSelectedMenuItem == ITEM_SELECT_MODE)
					{
					}
					mSelectedMenuItem = ITEM_NONE;
				}
			}
		}
		else
		{
			D3DXVECTOR3 m = game->mHeightMap->mouseCoords(mouseX, mouseY);
			if (mSelectedMenuItem == ITEM_NONE)
			{
				mQueuePos = mMenuPos = 0;
				mSelectedMenuItem = ITEM_NONE;
				int i;
				for (i = game->mMechs.first(); i != game->mMechs.end(); i = game->mMechs.mTable[i].mNext)
				{
					D3DXVECTOR3 p = m - *game->mMechs.mTable[i].mObj->pos();
					const TBox *b = game->mMechs.mTable[i].mObj->boundingBox();
					if (p.x >= b->mMin.x && p.x <= b->mMax.x && p.z >= b->mMin.z && p.z <= b->mMax.z)
					{
						mSelected = game->mMechs.mTable[i].mObj->objectPtr();
						return;
					}
				}
				for (i = game->mBuildings.first(); i != game->mBuildings.end(); i = game->mBuildings.mTable[i].mNext)
				{
					D3DXVECTOR3 p = m - *game->mBuildings.mTable[i].mObj->pos();
					const TBox *b = game->mBuildings.mTable[i].mObj->boundingBox();
					if (p.x >= b->mMin.x && p.x <= b->mMax.x && p.z >= b->mMin.z && p.z <= b->mMax.z)
					{
						mSelected = game->mBuildings.mTable[i].mObj->objectPtr();
						return;
					}
				}
				mSelected.setPtrToNULL();
			}
			else if (mSelectedMenuItem >= ITEM_MOVE && mSelectedMenuItem <= ITEM_STOP)
				game->sendMessage(EMsgMechMove + mSelectedMenuItem - ITEM_MOVE, selectedMech, *(int *)&m.x, *(int *)&m.z);
		}
	}
}

void CRTSModeUI::draw(uint32 aTime)
{
	if (mActive && mCounter < 1000) mCounter += 25;
	else if (!mActive && mCounter > 0) mCounter -= 25;
	else if (mCounter == 0) return;
//	const DIMOUSESTATE2 *mouseState = directInput->getMouseState();
	MColliding *selected = (CDrawable *)mSelected.ptr();
	CMech *selectedMech = game->getObjType(mSelected) == ETypeMech ? (CMech *)selected : NULL;
	CBuilding *selectedBuilding = game->getObjType(mSelected) == ETypeBuilding ? (CBuilding *)selected : NULL;
	float w = (float)(d3dObj->width() - 148);// * (mCounter / 1000.f));
	D3DCOLOR color = (0xFF * mCounter / 1000) << 24 | 0x00FFFFFF;
	if (selected)
	{
		d3dObj->mMatrixStack->Push();
		const D3DXVECTOR3 *p = selected->pos();
		d3dObj->mMatrixStack->TranslateLocal(p->x, p->y+.1f, p->z);
		const TBox *box = selected->boundingBox();
//		float r = sqrt(selected->radiusSqr());
		float r = .5f + sqrt(max(box->mMax.x*box->mMax.x + box->mMax.z*box->mMax.z, box->mMin.x*box->mMin.x + box->mMin.z*box->mMin.z));
//		float r = fabs(box->mMin.z);
		d3dObj->mMatrixStack->ScaleLocal(r, r, r);
		mSelectedAnim->draw(aTime);
		d3dObj->mMatrixStack->Pop();
		
	}
	D3DXMATRIX matrix, matrix2;
	D3DXMatrixScaling(&matrix, 148/128.f, 768/512.f, 1.f);
	D3DXMatrixTranslation(&matrix2, w, 0.f, 0.f);
	matrix *= matrix2;
	mSprite->SetTransform(&matrix);
	mSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	mSprite->Draw(*mMenuTexture, NULL, NULL, &D3DXVECTOR3(0, 0, 0), color);
	mSprite->End();
	D3DXMatrixIdentity(&matrix);
	mSprite->SetTransform(&matrix);
	mSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	int num = getNumMenuItems();
	for (int i = 0; i < num && i < 10; i++)
	{
		int ind = mMenuPos * 2 + i;
		D3DXVECTOR3 pos = D3DXVECTOR3(w + 6.f + (i & 1 ? 72.f : 0.f), 246.f + (int)(i / 2) * 72.f, 0.f);
		if (i == num - 1 && mSelectedMenuItem != ITEM_NONE && mSelectedMenuItem != ITEM_STOP)
			mSprite->Draw(*mCancelTexture, NULL, NULL, &pos, color);
		else if (selectedMech)
			mSprite->Draw(*mMechCommandTextures[ind], NULL, NULL, &pos, color);
		else if (!selected)
			mSprite->Draw(*mPlayer->getBuildingData(ind)->getPicture(), NULL, NULL, &pos, color);
		if (mSelectedMenuItem == ITEM_STOP && ind == 3 /*TODO*/)
		{
			mSelectedMenuItem = ITEM_NONE;
		}
	}
	mSprite->Draw(mMinimapTexture, NULL, NULL, &D3DXVECTOR3(w + 10.f, 46.f, 0.f), color);
//	if (mSelectedMenuItem != ITEM_BUILDING)
		mSprite->Draw(*mMouseCursor, NULL, NULL,
			&D3DXVECTOR3((float)directInput->getMouseX(), (float)directInput->getMouseY(), 0.f), color);
	mSprite->End();
	if (mSelectedMenuItem == ITEM_BUILDING)
	{
		D3DXVECTOR3 m = game->mHeightMap->mouseCoords(directInput->getMouseX(), directInput->getMouseY());
		d3dObj->mMatrixStack->Push();
		d3dObj->mMatrixStack->TranslateLocal(m.x, m.y, m.z);
		mPlayer->getBuildingData(mSelectedMenuItemIndex)->getAnimation()->draw(aTime);
		d3dObj->mMatrixStack->Pop();
	}
}

void CRTSModeUI::activate(CGameObjPtr aTarget)
{
	mActive = true;
	mSelected = aTarget;
	game->mCam->setRTSMode();
}

int CRTSModeUI::getNumMenuItems()
{
	MColliding *selected = (CDrawable *)mSelected.ptr();
	CMech *selectedMech = game->getObjType(mSelected) == ETypeMech ? (CMech *)selected : NULL;
	CBuilding *selectedBuilding = game->getObjType(mSelected) == ETypeBuilding ? (CBuilding *)selected : NULL;
	if (mSelectedMenuItem > ITEM_NONE && mSelectedMenuItem < ITEM_STOP)
		return 1;
	else if (!selected)
		return mPlayer->numBuildings();
	else if (selectedMech)
	{
		if (mSelectedMenuItem == ITEM_SELECT_WEAPON)
			return 0;
		else if (mSelectedMenuItem == ITEM_SELECT_MODE)
			return 0;
		else
			return NUM_MECH_COMMANDS;
	}
	return 0;
}