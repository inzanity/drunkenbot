#include <math.h>
#include "../inc/gameobj.h"

// CGameObj

CGameObj::CGameObj(int aType) : mRadius(0), mOrientation(0), mType(aType), mAnimationTimer(0)
{
	mPos.mX = mPos.mY = 0;
}

CGameObj::CGameObj(const CGameObj *aGameObj) : mPos(aGameObj->mPos), mRadius(aGameObj->mRadius),
											   mOrientation(aGameObj->mOrientation), mType(aGameObj->mType),
											   mAnimationTimer(aGameObj->mAnimationTimer)
{
}

CGameObj::~CGameObj()
{
}

float CGameObj::xPos() const
{
	return mPos.mX;
}
	
float CGameObj::yPos() const
{
	return mPos.mY;
}

float CGameObj::radius() const
{
	return mRadius;
}

float CGameObj::orientation() const
{
	return mOrientation;
}

int CGameObj::type() const
{
	return mType;
}

float CGameObj::animationTimer() const
{
	return mAnimationTimer;
}



// CMovingGameObj

const float CMovingGameObj::mEpsilon = 0.00001f;

CMovingGameObj::CMovingGameObj(int aType) : CGameObj(aType), mVelocity(0), mMovingDirection(0)
{
}

CMovingGameObj::CMovingGameObj(const CMovingGameObj *aGameObj) : CGameObj(aGameObj), mVelocity(aGameObj->mVelocity),
																 mMovingDirection(aGameObj->mVelocity)
{
}

CMovingGameObj::~CMovingGameObj()
{
}

float CMovingGameObj::velocity() const
{
	return mVelocity;
}

float CMovingGameObj::movingDirection() const
{
	return mMovingDirection;
}

void CMovingGameObj::move(float aTimeFactor)
{
	if (mMovingTimeFactor > 0)
	{
		if (mMovingTimeFactor < aTimeFactor)
			aTimeFactor = mMovingTimeFactor;
		mMovingTimeFactor -= aTimeFactor;
		mPos.mX += aTimeFactor * mVelocity * (float)cos(mMovingDirection);
		mPos.mY += aTimeFactor * mVelocity * (float)sin(mMovingDirection);
	}
}

bool CMovingGameObj::chkCollision(const char ** aTilemap, CBotInfo ** aBots, bool aCollisionWithObstacles)
{
	mMovingTimeFactor = 1.f;
	return true;
}

bool CMovingGameObj::handleCollision(int)
{
	return true;
}

int CMovingGameObj::getDamage() const
{
	return 0;
}

void CMovingGameObj::addFrag()
{
}

float CMovingGameObj::getNextEdge(TVector aPos, TVector aSpeed)
{
	float dX = aSpeed.mX < 0 ? floorf(aPos.mX) - aPos.mX : 1.f - aPos.mX + floorf(aPos.mX);
	float dY = aSpeed.mY < 0 ? floorf(aPos.mY) - aPos.mY : 1.f - aPos.mY + floorf(aPos.mY);

	if (dX == 0) dX = (aSpeed.mX < 0 ? -1.f : 1.f);
	if (dY == 0) dY = (aSpeed.mY < 0 ? -1.f : 1.f);

	float xTime = dX / aSpeed.mX, yTime = dY / aSpeed.mY;

	return (xTime < yTime ? xTime : yTime) + mEpsilon;
}
