#include "../include/game.h"
#include "../include/dock.h"
#include "../include/animationStorage.h"

CDock::CDock(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
CBuilding(aObjPtr, (mDock = new CBuildingData(CAnimationStorage::ptr()->getAnimation("data/dock.x"), CAnimationStorage::ptr()->getAnimation("data/dock.x"), 1, 1, 1)), aReady, aPos, aOrientation)
{
	mSize					= 1.5f;

	D3DXQuaternionRotationYawPitchRoll(&mOrientation, 0.0f, 0, 0);
	game->sendMessage(EMsgBuildingReady, this, 0, 0, 0);
}

CDock::CDock(istream &aStream) :
	CBuilding(aStream)
{
}

CDock::~CDock()
{
}

void CDock::externalize(ostream &aStream)
{
}

void CDock::handleMessage(CMessage *aMsg)
{
	D3DXVECTOR3		temp;
	int				i;

	switch (aMsg->mMsg)
	{
	case EMsgBuildingReady:
		i = 1;
	}
}

void CDock::update(uint32 aTimeFactor)
{
	D3DXVECTOR3	vec;
	float		angle;
}

void CDock::draw(uint32 aTimeFactor)
{
	d3dObj->mMatrixStack->Push();

	D3DXVECTOR3 vec;
	float angle;
	d3dObj->mMatrixStack->TranslateLocal(mPos.x, mPos.y, mPos.z);
	d3dObj->mMatrixStack->ScaleLocal(mSize, mSize, mSize);
	D3DXQuaternionToAxisAngle(&mOrientation, &vec, &angle);
	d3dObj->mMatrixStack->RotateAxisLocal(&vec, angle);

	mAnimTime += (uint32)(aTimeFactor * mAnimSpeed);
	if (mAnimation)
		mAnimation->draw(mAnimTime);

	d3dObj->mMatrixStack->Pop();
}