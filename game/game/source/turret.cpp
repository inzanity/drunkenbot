#include <math.h>
#include "../include/game.h"
#include "../include/turret.h"
#include "../include/animationStorage.h"

CTurret::CTurret(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
	CBuilding(aObjPtr, new CBuildingData(CAnimationStorage::ptr()->getAnimation("data/turret.x"), CAnimationStorage::ptr()->getAnimation("data/turretTop.x"), CAnimationStorage::ptr()->getAnimation("data/turret.x"), 1, "Foobar", NULL, 0, 1, 1, 1, 1, 1, 1, TURRET, 1, NULL), aReady, aPos, aXAngle, aYAngle)
{
	mTurretAngle			= 0.f;
	mTargetReached			= false;

	game->sendMessage(EMsgActivate, this, 0, 0, 0);

	time(&(mTime));
}

CTurret::CTurret(CGameObjPtr aObjPtr, CBuildingData *aBuildingData, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
	CBuilding(aObjPtr, aBuildingData, aReady, aPos, aXAngle, aYAngle)
{
	mTurretAngle			= 0.f;
	mTargetReached			= false;

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
			temp -= mPos;
			if (sqrt(D3DXVec3Dot(&temp, &temp)) < 10)
			{
				mTargetAngle	= (float)atan2f(temp.z, temp.x) + D3DX_PI;
				game->sendMessage(EMsgActivate, this, 0, 0, 0);
				return;
			}
		}
		// if mechs not found
	default:
		CBuilding::handleMessage(aMsg);
	}
}

void CTurret::update(uint32 aTimeFactor)
{
	D3DXVECTOR3	vec;
	float		angle;
	float		accuracy		= 0.05f;
	float		turnAccuracy	= 0.015f;

	if (time(NULL) - mTime > aTimeFactor)
	{
		game->sendMessage(EMsgActivate, this, 0, 0, 0);
		time(&mTime);
	}

	if (fabs(mTurretAngle - mTargetAngle) > accuracy)
	{
		if (mTargetAngle - mTurretAngle < D3DX_PI && mTargetAngle > mTurretAngle)		// difference < pi
		{
			mTurretAngle += turnAccuracy;
			if (mTurretAngle > 2*D3DX_PI)
				mTurretAngle -= 2*D3DX_PI;
		}
		else
		{
			mTurretAngle -= turnAccuracy;
			if (mTurretAngle < 0)
				mTurretAngle += 2*D3DX_PI;
		}
		mTargetReached	= false;
	}
	else
	{
		mTurretAngle = mTargetAngle;
		mTargetReached	= true;
	}

/*
if (mTurretAngle < targetAngle - accuracy)
{
	mTurretAngle += targetAngle / 10.f;
}
else if (mTurretAngle > targetAngle + accuracy)
{
	mTurretAngle -= targetAngle / 10.f;
}
*/

}

float CTurret::angle()
{
	return mTurretAngle;
}

bool CTurret::active()
{
	return mTargetReached;
}

void CTurret::draw(uint32 aTimeFactor)
{
	d3dObj->mMatrixStack->Push();

	d3dObj->mMatrixStack->TranslateLocal(mPos.x, mPos.y, mPos.z);
	d3dObj->mMatrixStack->RotateYawPitchRollLocal(mYAngle, mXAngle, 0);

	mAnimTime += (uint32)(aTimeFactor * mAnimSpeed);
	if (mAnimation)
		mAnimation->draw(mAnimTime);

	d3dObj->mMatrixStack->TranslateLocal(0, 0.2f, 0);
	d3dObj->mMatrixStack->RotateYawPitchRollLocal(-mTurretAngle, 0, 0);
	if (mBuildingData->getUpperBodyAnimation())
		mBuildingData->getUpperBodyAnimation()->draw(mAnimTime);

	d3dObj->mMatrixStack->Pop();
}