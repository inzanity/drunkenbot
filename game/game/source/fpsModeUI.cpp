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
	CMech *ptr = (CMech *)mMech.ptr();
	if (directInput->checkKey(KEY_1))
	{
		mActive = false;
		game->mRTSModeUI->activate(mMech);
		ptr->setSpeed(&D3DXVECTOR3(0, 0, 0));
		return;
	}
	if (!mCounter || !ptr) return; /* Controlling requires 100% FPS mode */
	if (directInput->checkMouseButton(0))
	{
	}
	if (directInput->isPressed(MOVE_FORWARD))
	{
		D3DXMATRIX mat;
		D3DXVECTOR4 out;
		D3DXMatrixRotationQuaternion(&mat, ptr->orientation());
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, .002f), &mat);
		ptr->setSpeed((const D3DXVECTOR3 *)&out);
	}
	else
		ptr->setSpeed(&D3DXVECTOR3(0, 0, 0));
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
