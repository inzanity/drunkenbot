#include "../include/gameui.h"
#include "../include/directInput.h"
#include "../include/animationStorage.h"
#include "../include/game.h"

CGameUI::CGameUI() : mSprite(NULL), mMouseCursor(NULL)
{
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
    D3DXCreateSprite(device, &mSprite);
	mMouseCursor = CAnimationStorage::ptr()->getTexture("data/cursor.png");
	mMenuTexture = CAnimationStorage::ptr()->getTexture("data/rts.bmp");
	mSelectedAnim = CAnimationStorage::ptr()->getAnimation("../tools/particleEditor/particleEffects/selection.lua");
	mSelected.setPtrToNULL();
	D3DXCreateTexture(device, 128, 128, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &mMinimapTexture);

	D3DXMATRIX projectionMatrix;
	int h = game->mHeightMap->horizontalSize() - 1;
	int v = game->mHeightMap->verticalSize() - 1;
	D3DXMatrixOrthoLH(&projectionMatrix, h, v, h/(float)v, 150.0f);
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

CGameUI::~CGameUI()
{
	if (mSprite)
		mSprite->Release();
}

void CGameUI::handleInput()
{
	int mouseX = directInput->getMouseX(), mouseY = directInput->getMouseY();
	if (directInput->checkMouseButton(0))
	{
		D3DXVECTOR3 m = game->mHeightMap->mouseCoords(mouseX, mouseY);
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
	}
}

void CGameUI::draw(uint32 aTime)
{
//	const DIMOUSESTATE2 *mouseState = directInput->getMouseState();
	MColliding *selected = (CDrawable *)mSelected.ptr();
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
	int w = d3dObj->width() - 148;
	D3DXMATRIX matrix, matrix2;
	D3DXMatrixScaling(&matrix, 148/128.f, 768/512.f, 1.f);
	D3DXMatrixTranslation(&matrix2, w, 0, 0);
	matrix *= matrix2;
	mSprite->SetTransform(&matrix);
	mSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	mSprite->Draw(*mMenuTexture, NULL, NULL, &D3DXVECTOR3(0, 0, 0), 0xFFFFFFFF);
	mSprite->End();
	D3DXMatrixIdentity(&matrix);
	mSprite->SetTransform(&matrix);
	mSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	mSprite->Draw(mMinimapTexture, NULL, NULL, &D3DXVECTOR3(w + 10, 46, 0), 0xFFFFFFFF);
	mSprite->Draw(*mMouseCursor, NULL, NULL,
		&D3DXVECTOR3((float)directInput->getMouseX(), (float)directInput->getMouseY(), 0), 0xFFFFFFFF);
	mSprite->End();
}
