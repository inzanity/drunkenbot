#include "../include/fpsModeUI.h"
#include "../include/directInput.h"
#include "../include/animationStorage.h"
#include "../include/game.h"

CFPSModeUI::CFPSModeUI() : mActive(false), mCounter(0), mMech()
{
}

CFPSModeUI::~CFPSModeUI()
{
}

void CFPSModeUI::handleInput()
{
	if (!mActive) return; /* Active UI will handle input */
	int mouseX = directInput->getMouseX();
	CMech *mech = (CMech *)mMech.ptr();
	if (directInput->checkKey(KEY_1))
	{
		mActive = false;
		game->mRTSModeUI->activate(mMech);
		mech->setSpeed(&D3DXVECTOR3(0, 0, 0));
		return;
	}
	if (!mCounter || !mech) return; /* Controlling requires 100% FPS mode */
	if (directInput->checkMouseButton(0))
	{
	}
//reset stuff before new button check
	{
		mech->setSpeed(&D3DXVECTOR3(0, 0, 0));
		mech->setRotSpeed(&D3DXQUATERNION(0, 0, 0, 1));
	}

	if (directInput->isPressed(MOVE_UP))
	{
		D3DXMATRIX mat;
		D3DXVECTOR4 out;
		D3DXMatrixRotationQuaternion(&mat, mech->orientation());
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, .005f), &mat);
		mech->setSpeed((const D3DXVECTOR3 *)&out);
	}
	else if (directInput->isPressed(MOVE_DOWN))
	{
		D3DXMATRIX mat;
		D3DXVECTOR4 out;
		D3DXMatrixRotationQuaternion(&mat, mech->orientation());
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, -.005f), &mat);
		mech->setSpeed((const D3DXVECTOR3 *)&out);
	}
	else if (directInput->isPressed(MOVE_LEFT))
	{
		D3DXQUATERNION quaternion(0, -.001f, 0, 1);
		mech->setRotSpeed((const D3DXQUATERNION *)&quaternion);
	}
	else if (directInput->isPressed(MOVE_RIGHT))
	{
		D3DXQUATERNION quaternion(0, .001f, 0, 1);
		mech->setRotSpeed((const D3DXQUATERNION *)&quaternion);
	}
}

void CFPSModeUI::draw(uint32 aTime)
{
	if (mActive && mCounter < 1000) mCounter += 50;
	else if (!mActive && mCounter > 0) mCounter -= 50;
	else if (mCounter == 0) return;
	CMech *ptr = (CMech *)mMech.ptr();
	if (ptr)
	{
	}
	return;
}

void CFPSModeUI::activate(CGameObjPtr aTarget)
{
	mActive = true;
	mMech = aTarget;
	game->mCam->setFPSMode(mMech);
}
