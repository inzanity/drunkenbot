#include "../inc/bullet.h"
#include "../inc/botinfo.h"


// CBulletInfo

CBulletInfo::CBulletInfo(float aXPos, float aYPos, float aDirection, CBotInfo *aShooter) :
	CMovingGameObj(EObjectBullet | (aShooter->weapon()->weaponType() << KObjectIndexShift)), mShooter(aShooter)
{
	mPos.mX = aXPos;
	mPos.mY = aYPos;
	mOrientation = mMovingDirection = aDirection;
	mVelocity = aShooter->weapon()->bulletSpeed();
	mRadius = .2f;
}

CBulletInfo::~CBulletInfo()
{
}

bool CBulletInfo::handleCollision(char)
{
	if (mShooter->weapon()->explosionRadius() > 0)
	{
		mType &= EObjectBullet ^ 0xffffffff;
		mType |= EObjectExplosion;
	}
	return true;
}

char CBulletInfo::getDamage() const
{
	return int(mShooter->weapon()->damage());
}

void CBulletInfo::changeFragNum(bool aAddFrag)
{
	mShooter->changeFragNum(aAddFrag);
}

bool CBulletInfo::update()
{
	CMovingGameObj::update();
	if ((mType & KObjectTypeMask) == EObjectExplosion)
	{
		if (mRadius >= mShooter->weapon()->explosionRadius())
			return false;
		mRadius += mShooter->weapon()->explosionSpeed();
		return true;
	}
	return (!mCollisionDetected);
}
