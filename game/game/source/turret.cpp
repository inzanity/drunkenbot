#include "../include/game.h"
#include "../include/turret.h"
#include "../include/animationStorage.h"

#define	PI		atan2(0, -1)

CTurret::CTurret(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
CBuilding(aObjPtr, (mTurret = new CBuildingData(CAnimationStorage::ptr()->getAnimation("data/turret.x"), CAnimationStorage::ptr()->getAnimation("data/turret.x"), 1, 1, 1)), aReady, aPos, aOrientation)
{
	mTurretTop				= CAnimationStorage::ptr()->getAnimation("data/turretTop.x");
	mTurretAngle			= 0.f;
	mSize					= 1.5f;
	mTargetReached			= false;

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
			if (sqrt((temp.x*temp.x)+(temp.y*temp.y)+(temp.z*temp.z)) < 10)
			{
				mTargetAngle	= (float)atan2(temp.z, temp.x) + PI;
				game->sendMessage(EMsgActivate, this, 0, 0, 0);
				return;
			}
		}
		// if mechs not found
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
		if (mTargetAngle - mTurretAngle < PI && mTargetAngle > mTurretAngle)		// difference < pi
		{
			mTurretAngle += turnAccuracy;
			if (mTurretAngle > 2*PI)
				mTurretAngle -= 2*PI;
		}
		else
		{
			mTurretAngle -= turnAccuracy;
			if (mTurretAngle < 0)
				mTurretAngle += 2*PI;
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

	D3DXVECTOR3 vec;
	float angle;
	d3dObj->mMatrixStack->TranslateLocal(mPos.x, mPos.y, mPos.z);
	d3dObj->mMatrixStack->ScaleLocal(mSize, mSize, mSize);
	D3DXQuaternionToAxisAngle(&mOrientation, &vec, &angle);
	d3dObj->mMatrixStack->RotateAxisLocal(&vec, angle);

	mAnimTime += (uint32)(aTimeFactor * mAnimSpeed);
	if (mAnimation)
		mAnimation->draw(mAnimTime);

	d3dObj->mMatrixStack->TranslateLocal(0, 0.2f, 0);
	d3dObj->mMatrixStack->RotateYawPitchRollLocal(-mTurretAngle, 0, 0);
	if (mTurretTop)
		mTurretTop->draw(mAnimTime);

	d3dObj->mMatrixStack->Pop();
}