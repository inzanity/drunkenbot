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
	return true;
}

TVector CMovingGameObj::scanTilemap(const char ** aSrcTilemap, float aAngle, CTilemap * aDstTilemap) const
{
	TVector pos = mPos;
	return pos;
}

bool CMovingGameObj::handleCollision(int aDamage)
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

float CMovingGameObj::getNextEdge(TVector aPos, TVector aSpeed) const
{
	float x = aPos.mX - int(aPos.mX);
	float y = aPos.mY - int(aPos.mY);

	float xTime = 100000.f, yTime = 100000.f;

	if (aSpeed.mX > 0)		xTime = (1.f - x) / aSpeed.mX;
	else if (aSpeed.mX < 0)	xTime = x / -aSpeed.mX;

	if (aSpeed.mY > 0)		yTime = (1.f - y) / aSpeed.mY;
	else if (aSpeed.mY < 0)	yTime = y / -aSpeed.mY;

	return (xTime < yTime ? xTime : yTime) + mEpsilon;
}
