#include "../inc/bullet.h"
#include "../inc/botinfo.h"


// CBulletInfo

CBulletInfo::CBulletInfo(float aXPos, float aYPos, float aDirection, CBotInfo *aShooter) :
	CMovingGameObj(EObjectBullet | (aShooter->weapon()->weaponType() << KObjectIndexShift)), mShooter(aShooter),
	mExplosionSpeed(aShooter->weapon()->explosionSpeed()), mExplosionRadius(aShooter->weapon()->explosionRadius()),
	mDamage(aShooter->weapon()->damage())

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
	if (mExplosionRadius > 0)
	{
		mType &= EObjectBullet ^ 0xffffffff;
		mType |= EObjectExplosion;
	}
	return true;
}

char CBulletInfo::getDamage() const
{
	return int(mDamage);
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
		if (mRadius >= mExplosionRadius)
			return false;
		mRadius += mExplosionSpeed;
		return true;
	}
	return (!mCollisionDetected);
}
