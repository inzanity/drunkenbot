#include <math.h>
#include "../inc/gameobj.h"
#include "../inc/botinfo.h"

float sqr(float aNum)
{
	return aNum * aNum;
}

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
	float time = 5.f, minTime = 1.f, xSpeed = mVelocity * cos(mMovingDirection);
	float ySpeed = mVelocity * sin(mMovingDirection);

	for (int i = 0; aBots[i]; i++)
	{
		if (sqr(aBots[i]->xPos() - mPos.mX) + sqr(aBots[i]->yPos() - mPos.mY) < sqr(mVelocity + aBots[i]->velocity() + mRadius + aBots[i]->radius()))
		{
			float foo;
			float xSpeed2 = aBots[i]->velocity() * cos(aBots[i]->movingDirection());
			float ySpeed2 = aBots[i]->velocity() * sin(aBots[i]->movingDirection());
			float rooted = -sqr((mPos.mY - aBots[i]->yPos()) * (xSpeed - xSpeed2) - (mPos.mX - aBots[i]->xPos()) * (ySpeed - ySpeed2)) + (sqr(ySpeed - ySpeed2) + sqr(xSpeed - xSpeed2)) * sqr(mRadius + aBots[i]->radius());
			if (rooted > 0)
			{
				float root = sqrt(rooted);
				float plop = (ySpeed - ySpeed2) * (aBots[i]->yPos() - mPos.mY) + (xSpeed - xSpeed2) * (aBots[i]->xPos() - mPos.mX);
				float divider = 1 / (sqr(ySpeed - ySpeed2) + sqr(xSpeed - xSpeed2));
				foo = time = (plop + root) * divider;
				if (time > 0)
				{
					time = (plop - root) * divider;
					if (time < foo) foo = time;
					if (foo < minTime) minTime = time;
					if (foo < 1.f)
					{	
						handleCollision(3);
//						aBots[i]->handleCollision(5);
					}
				}
			}
		}
	}
	mMovingTimeFactor = minTime;
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

void CMovingGameObj::changeFragNum(bool aAddFrag)
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
