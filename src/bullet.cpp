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
						 CVisibleBulletInfo(aBulletType), mShooter(aShooter)
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
	return true;
}

int CBulletInfo::getDamage() const
{
	return int(mShooter->weapon()->damage());
}

void CBulletInfo::addFrag()
{
	mShooter->addFrag();
}
