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
	mMinimapPingAnim = CAnimationStorage::ptr()->getAnimation("../tools/particleEditor/particleEffects/minimapenemmy.lua");
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
		mech->setRotSpeed(0, 0);
		return;
	}
	if (!mCounter || !mech) return; /* Controlling requires 100% FPS mode */
//reset stuff before new button check
	{
		mech->setSpeed(&D3DXVECTOR3(0, 0, 0));
		mech->setRotSpeed(0, 0);
	}

	float mechRotSpeed = mech->maxRotSpeed();
	float mechSpeed = mech->maxSpeed();

	if (directInput->isPressed(MOVE_LEFT))
	{
		mech->setRotSpeed(0, -mechRotSpeed);
	}
	else if (directInput->isPressed(MOVE_RIGHT))
	{
		mech->setRotSpeed(0, mechRotSpeed);
	}

	if (directInput->isPressed(MOVE_UP))
	{
		mech->setSpeed(&D3DXVECTOR3(sin(mech->yRot()) * mechSpeed, 0, cos(mech->yRot()) * mechSpeed));
	}
	if (directInput->isPressed(MOVE_DOWN))
	{
		mech->setSpeed(&D3DXVECTOR3(sin(mech->yRot()) * mechSpeed, 0, cos(mech->yRot()) * -mechSpeed));
	}

	float maxXAngleSpeed = mech->maxUBAngleXSpeed();
	float maxYAngleSpeed = mech->maxUBAngleYSpeed();

	if (mouseX < 200) mech->setUpperBodyAngleYSpeed(-maxYAngleSpeed);
	else if (mouseX > d3dObj->width() - 200) mech->setUpperBodyAngleYSpeed(maxYAngleSpeed);
	else mech->setUpperBodyAngleYSpeed(.0f);

	if (mouseY < 100) mech->setUpperBodyAngleXSpeed(-maxXAngleSpeed);
	else if (mouseY > d3dObj->height() - 100) mech->setUpperBodyAngleXSpeed(maxXAngleSpeed);
	else mech->setUpperBodyAngleXSpeed(.0f);
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
	float a = aTime/(float)mRadarAnim->getDuration();
	a -=(int)a;	
	float angle = mech->yRot();

	d3dObj->mMatrixStack->Push();
	D3DXMATRIX *identityMatrix = d3dObj->mMatrixStack->GetTop();
	D3DXMatrixIdentity(identityMatrix);
	d3dObj->mMatrixStack->Translate(0.55f, -0.35f, 1.5f);
	d3dObj->mMatrixStack->RotateAxisLocal(&D3DXVECTOR3(1, 0, 0), 3.1415f/2.0f);
	mRadarAnim->draw(aTime);
	d3dObj->mMatrixStack->Pop();

	d3dObj->mMatrixStack->Push();
	identityMatrix = d3dObj->mMatrixStack->GetTop();
	D3DXMatrixIdentity(identityMatrix);
	d3dObj->mMatrixStack->RotateAxis(&D3DXVECTOR3(0, 0, 1), angle);
	d3dObj->mMatrixStack->Translate(0.55f, -0.35f, 1.5f);
	float radarRange2 = mech->radarRange() * mech->radarRange();
	float scale = sqrt(mRadarAnim->getRadiusSqr()) / mech->radarRange();

	if (mech->radarDelay() == true) 
	{
		for (int i = game->mBuildings.first(); i != game->mBuildings.end(); i = game->mBuildings.mTable[i].mNext)
		{
			D3DXVECTOR3 position = *game->mBuildings.mTable[i].mObj->pos() - *mech->pos();
			//if the position is out of range of the radar then skip the object
			if(position.x * position.x + position.z * position.z > radarRange2) continue;

			float f = atan2f(position.x, position.z) - angle;
			f = 2 * D3DX_PI * a + D3DX_PI/2.f- f;
			if (f < 0) f += 2 * D3DX_PI;
			if (f > 2 * D3DX_PI) f -= 2 * D3DX_PI;
			int time = (int)(mMinimapPingAnim->getDuration() * f / (2 * D3DX_PI));
			d3dObj->mMatrixStack->Push();
			d3dObj->mMatrixStack->TranslateLocal(position.x*scale, position.z*scale, 0);
			mMinimapPingAnim->draw(time);
			d3dObj->mMatrixStack->Pop();
		}
	}
	d3dObj->mMatrixStack->Pop();
	return;
}

void CFPSModeUI::activate(CGameObjPtr aTarget)
{
	mActive = true;
	mMech = aTarget;
	game->mCam->setFPSMode(mMech);
}
