#include "../include/game.h"
#include "../include/turret.h"
#include "../include/animationStorage.h"

CTurret::CTurret(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
CBuilding(aObjPtr, (mTurret = new CBuildingData(CAnimationStorage::ptr()->getAnimation("data/turret.x"), CAnimationStorage::ptr()->getAnimation("data/turret.x"), 1, 1, 1)), aReady, aPos, aOrientation)
{
	mTurretTop		= CAnimationStorage::ptr()->getAnimation("data/turretTop.x");
	mDestination	= *aOrientation;

	D3DXQuaternionRotationYawPitchRoll(&mOrientation, 0.0f, 0, 0);
	game->sendMessage(EMsgActivate, this, 0, 0, 0);

	time(&(mTime));
}

CTurret::CTurret(istream &aStream) :
	CBuilding(aStream)
{
}

CTurret::~CTurret()
{
}

void CTurret::externalize(ostream &aStream)
{
}

void CTurret::handleMessage(CMessage *aMsg)
{
	D3DXVECTOR3		temp;
	int				i;
	switch (aMsg->mMsg)
	{
	case EMsgActivate:
		for (i = 0; i < game->mMechs.size(); i++)
		{
			temp	= *game->mMechs.object(i)->pos();
			temp.x	-= mPos.x; temp.y	-= mPos.y; temp.z	-= mPos.z;
			if (sqrt((temp.x*mPos.x)+(temp.y*mPos.y)+(temp.z*mPos.z)) < 10)
			{
				mDestination.x = temp.x; mDestination.y = temp.y; mDestination.z = temp.z;
				game->sendMessage(EMsgActivate, this, 0, 0, 0);
			}
		}
	}
}

void CTurret::update(uint32 aTimeFactor)
{
	D3DXVECTOR3 vec;
	float angle;

	if (time(NULL) - mTime > aTimeFactor)
	{
		game->sendMessage(EMsgActivate, this, 0, 0, 0);
		time(&mTime);
	}
}

float CTurret::angle()
{
	return mTurretAngle;
}

void CTurret::draw(uint32 aTimeFactor)
{
	d3dObj->mMatrixStack->Push();

	D3DXVECTOR3 vec;
	float angle;
	d3dObj->mMatrixStack->TranslateLocal(mPos.x, mPos.y, mPos.z);
	D3DXQuaternionToAxisAngle(&mOrientation, &vec, &angle);
	d3dObj->mMatrixStack->RotateAxisLocal(&vec, angle);

	mAnimTime += (uint32)(aTimeFactor * mAnimSpeed);
	if (mAnimation)
		mAnimation->draw(mAnimTime);

	d3dObj->mMatrixStack->TranslateLocal(0, 0.25f, 0);
	mTurretAngle	= (float)atan2(-mDestination.z, mDestination.x);
	d3dObj->mMatrixStack->RotateYawPitchRollLocal(mTurretAngle + 3.1415f, 0, 0);
	if (mTurretTop)
		mTurretTop->draw(mAnimTime);

	d3dObj->mMatrixStack->Pop();
}