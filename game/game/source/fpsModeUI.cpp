#include "../include/fpsModeUI.h"
#include "../include/directInput.h"
#include "../include/animationStorage.h"
#include "../include/game.h"

CFPSModeUI::CFPSModeUI() : mActive(false), mCounter(0), mMech()
{
	mTimer = 0;
	LPDIRECT3DDEVICE9 device = d3dObj->mD3DDevice;
    D3DXCreateSprite(device, &mSprite);
	mCrossCursor = CAnimationStorage::ptr()->getTexture("data/cross.png");
	mWeaponBarTexture = CAnimationStorage::ptr()->getTexture("data/weaponbar.bmp");
	mRadarTexture = CAnimationStorage::ptr()->getTexture("data/fpsradar.bmp");
	mRadarAnim = CAnimationStorage::ptr()->getAnimation("../tools/particleEditor/particleEffects/radar.lua");
	mMinimapPingAnim = CAnimationStorage::ptr()->getAnimation("../tools/particleEditor/particleEffects/minimapping.lua");
}

CFPSModeUI::~CFPSModeUI()
{
}

void CFPSModeUI::handleInput()
{
	if (!mActive) return; /* Active UI will handle input */
	int mouseX = directInput->getMouseX();
	int mouseY = directInput->getMouseY();

	CMech *mech = (CMech *)mMech.ptr();
	if (directInput->checkKey(KEY_1))
	{
		mActive = false;
		game->mRTSModeUI->activate(mMech);
		mech->setSpeed(&D3DXVECTOR3(0, 0, 0));
		return;
	}
	if (!mCounter || !mech) return; /* Controlling requires 100% FPS mode */
//reset stuff before new button check
	{
		mech->setSpeed(&D3DXVECTOR3(0, 0, 0));
		mech->setRotSpeed(&D3DXQUATERNION(0, 0, 0, 1));
	}

	if (directInput->isPressed(MOVE_FORWARD))
	{
		if (mouseY < 200)
		{
			D3DXQUATERNION quaternion(-.001f, 0, 0, 1);
			mech->setRotSpeed((const D3DXQUATERNION *)&quaternion);
		}
		else if (mouseY > d3dObj->height() - 200)
		{
			D3DXQUATERNION quaternion(.001f, 0, 0, 1);
			mech->setRotSpeed((const D3DXQUATERNION *)&quaternion);
		}
	}

	if (directInput->isPressed(MOVE_UP))
	{
		D3DXMATRIX mat;
		D3DXVECTOR4 out;
		D3DXMatrixRotationQuaternion(&mat, mech->orientation());
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, .005f), &mat);
		mech->setSpeed((const D3DXVECTOR3 *)&out);
	}
	if (directInput->isPressed(MOVE_DOWN))
	{
		D3DXMATRIX mat;
		D3DXVECTOR4 out;
		D3DXMatrixRotationQuaternion(&mat, mech->orientation());
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, -.005f), &mat);
		mech->setSpeed((const D3DXVECTOR3 *)&out);
	}
	if (mouseX < 200)
	{
		D3DXQUATERNION quaternion(0, -.001f, 0, 1);
		mech->setRotSpeed((const D3DXQUATERNION *)&quaternion);
	}
	else if (mouseX > d3dObj->width() - 200)
	{
		D3DXQUATERNION quaternion(0, .001f, 0, 1);
		mech->setRotSpeed((const D3DXQUATERNION *)&quaternion);
	}
}

void CFPSModeUI::draw(uint32 aTime)
{
	mTimer++;
	if (mActive && mCounter < 1000) mCounter += 50;
	else if (!mActive && mCounter > 0) mCounter -= 50;
	else if (mCounter == 0) return;

	mSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	mSprite->Draw(*mCrossCursor, NULL, NULL, &D3DXVECTOR3(d3dObj->width()/2.f, d3dObj->height()/2.f, 0.f), 0xFFFFFFFF);
//	mSprite->Draw(*mWeaponBarTexture, NULL, NULL, &D3DXVECTOR3(1, d3dObj->height() - 128, 0.1f), 0xFFFFFFFF);
//	mSprite->Draw(*mRadarTexture, NULL, NULL, &D3DXVECTOR3(d3dObj->width() - 128, d3dObj->height() - 128, 0.1f), 0xFFFFFFFF);
	mSprite->End();

	CMech *mech = (CMech *)mMech.ptr();
	const D3DXQUATERNION *mechOrientation = mech->orientation();
	float angle;
	D3DXVECTOR3 unusedvec;
	D3DXQuaternionToAxisAngle(mechOrientation, &unusedvec, &angle);

	d3dObj->mMatrixStack->Push();
	D3DXMATRIX *identityMatrix = d3dObj->mMatrixStack->GetTop();
	D3DXMatrixIdentity(identityMatrix);
	d3dObj->mMatrixStack->Translate(0.55, -0.35, 1.5);
	d3dObj->mMatrixStack->RotateAxisLocal(&D3DXVECTOR3(1, 0, 0), 3.1415f/2.0f);
	mRadarAnim->draw(aTime);
	d3dObj->mMatrixStack->Pop();

	d3dObj->mMatrixStack->Push();
	identityMatrix = d3dObj->mMatrixStack->GetTop();
	D3DXMatrixIdentity(identityMatrix);
	d3dObj->mMatrixStack->TranslateLocal(0.1, 0, 0);
	d3dObj->mMatrixStack->RotateAxis(&D3DXVECTOR3(0, 0, 1), angle);
	d3dObj->mMatrixStack->Translate(0.55, -0.35, 1.5);
	d3dObj->mMatrixStack->Push();
	d3dObj->mMatrixStack->TranslateLocal(0.1, 0, 0);
	d3dObj->mMatrixStack->Push();
	d3dObj->mMatrixStack->TranslateLocal(0.1, 0, 0);
	mMinimapPingAnim->draw(aTime);
	d3dObj->mMatrixStack->Pop();
	mMinimapPingAnim->draw(aTime);
	d3dObj->mMatrixStack->Pop();
	mMinimapPingAnim->draw(aTime);
	d3dObj->mMatrixStack->Pop();
	return;
}

void CFPSModeUI::activate(CGameObjPtr aTarget)
{
	mActive = true;
	mMech = aTarget;
	game->mCam->setFPSMode(mMech);
}
