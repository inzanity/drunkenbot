#include "../inc/bullet.h"
#include "../inc/botinfo.h"

// CVisibleBulletInfo

CVisibleBulletInfo::CVisibleBulletInfo(int aBulletType) : CMovingGameObj(2), mBulletType(aBulletType)
{
}

CVisibleBulletInfo::CVisibleBulletInfo(const CVisibleBulletInfo *aBullet, float aXPos, float aYPos) :
									   CMovingGameObj(aBullet), mBulletType(aBullet->mBulletType)
{
	mPos.mX = aBullet->xPos() - aXPos;
	mPos.mY = aBullet->yPos() - aYPos;
}

CVisibleBulletInfo::~CVisibleBulletInfo()
{
}
	
int CVisibleBulletInfo::bulletType() const
{
	return mBulletType;
}



// CBulletInfo

CBulletInfo::CBulletInfo(int aBulletType, float aXPos, float aYPos, float aDirection, float aVelocity, CBotInfo *aShooter) :
						 CVisibleBulletInfo(aBulletType), mShooter(aShooter), mAlive(true)
{
	mPos.mX = aXPos;
	mPos.mY = aYPos;
	mOrientation = mMovingDirection = aDirection;
	mVelocity = aVelocity;
}

CBulletInfo::~CBulletInfo()
{
}

bool CBulletInfo::handleCollision(int)
{
	mVelocity = .0001f;
	if (mShooter->weapon()->explosionRadius() == 0)
		mAlive = false;
	return true;
}

int CBulletInfo::getDamage() const
{
	return int(mShooter->weapon()->damage());
}

void CBulletInfo::changeFragNum(bool aAddFrag)
{
	mShooter->changeFragNum(aAddFrag);
}

bool CBulletInfo::alive()
{
	if (!mAlive)
		return false;
	if (mVelocity == .0001f)
	{
		mRadius += mShooter->weapon()->explosionSpeed();
		if (mRadius >= mShooter->weapon()->explosionRadius())
			mAlive = false;
	}
	return true;
}