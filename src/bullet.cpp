#include "../inc/bullet.h"
#include "../inc/botinfo.h"

// CVisibleBulletInfo

CVisibleBulletInfo::CVisibleBulletInfo(int aBulletType) : CMovingGameObj(EObjectBullet | (aBulletType << 8)), 
														  mBulletType(aBulletType)
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
						 CVisibleBulletInfo(aBulletType), mShooter(aShooter)
{
	mPos.mX = aXPos;
	mPos.mY = aYPos;
	mOrientation = mMovingDirection = aDirection;
	mVelocity = aVelocity;
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
	if ((mType & 0xF) == EObjectExplosion)
	{
		if (mRadius >= mShooter->weapon()->explosionRadius())
			return false;
		mRadius += mShooter->weapon()->explosionSpeed();
		return true;
	}
	return (!mCollisionDetected);
}
