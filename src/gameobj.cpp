#include <cmath>
#include <iostream>
#include "../inc/gameobj.h"
#include "../inc/botinfo.h"
#include "../inc/tilemap.h"

// CGameObj

CGameObj::CGameObj(int aType) : mRadius(0), mOrientation(0), mType(aType), mAnimationTimer(0)
{
	mPos.mX = mPos.mY = 0;
}

CGameObj::CGameObj(const CGameObj *aGameObj, float aXPos, float aYPos) : mPos(aGameObj->mPos), mRadius(aGameObj->mRadius),
																		 mOrientation(aGameObj->mOrientation),
																		 mType(aGameObj->mType),
																		 mAnimationTimer(aGameObj->mAnimationTimer)
{
	mPos.mX -= aXPos;
	mPos.mY -= aYPos;
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

float CGameObj::sqr(float aNum)
{
	return aNum * aNum;
}


// CMovingGameObj

const float CMovingGameObj::mEpsilon = 0.0001f;

CMovingGameObj::CMovingGameObj(int aType) : CGameObj(aType), mVelocity(0), mMovingDirection(0), mCollisionDetected(false),
											mMovingTimeFactor(1.f)
{
}

CMovingGameObj::CMovingGameObj(const CMovingGameObj *aGameObj, float aXPos, float aYPos) :
	CGameObj(aGameObj, aXPos, aYPos), mVelocity(aGameObj->mVelocity), mMovingDirection(aGameObj->mVelocity),
	mCollisionDetected(aGameObj->mCollisionDetected), mMovingTimeFactor(aGameObj->mMovingDirection)
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
	float time1, time2, minTime = 1.f;
	float xSpeed = mVelocity * cos(mMovingDirection);
	float ySpeed = mVelocity * sin(mMovingDirection);

	if (mVelocity > 0)
	{
		TVector speed = {xSpeed, ySpeed};
		float totalTime;
		char tile;
		int dir = (int)(mMovingDirection / PI * 4);
		for (int i = dir - 1; i <= dir + 2; i++)
		{
			TVector pos = {mPos.mX + cos(i * PI / 4.f) * mRadius,
						   mPos.mY + sin(i * PI / 4.f) * mRadius};
			for (time1 = totalTime = 0; totalTime < mMovingTimeFactor; totalTime += time1)
			{
				pos.mX += time1 * speed.mX; pos.mY += time1 * speed.mY;
				tile = aTilemap[(int)pos.mY][(int)pos.mX] & KTileTypeMask;
				if (tile == CTilemap::ETileWall || (aCollisionWithObstacles && tile == CTilemap::ETileObstacle))
					break;
				time1 = getNextEdge(pos, speed);
			}
			if (mMovingTimeFactor > totalTime)
				mMovingTimeFactor = totalTime;
		}
		if (mMovingTimeFactor < 1.f)
			handleCollision(15);
	}

	xSpeed += mEpsilon;
	int i;
	for (i = 0; aBots[i]; i++)
	{
		if (sqr(aBots[i]->xPos() - mPos.mX) + sqr(aBots[i]->yPos() - mPos.mY) < sqr(mVelocity + aBots[i]->velocity() + mRadius + aBots[i]->radius()))
		{
			float xSpeed2 = aBots[i]->velocity() * cos(aBots[i]->movingDirection());
			float ySpeed2 = aBots[i]->velocity() * sin(aBots[i]->movingDirection());
			float rooted = -sqr((mPos.mY - aBots[i]->yPos()) * (xSpeed - xSpeed2) - (mPos.mX - aBots[i]->xPos()) * (ySpeed - ySpeed2)) + (sqr(ySpeed - ySpeed2) + sqr(xSpeed - xSpeed2)) * sqr(mRadius + aBots[i]->radius());
			if (rooted > 0)
			{
				float root = sqrt(rooted);
				float plop = (ySpeed - ySpeed2) * (aBots[i]->yPos() - mPos.mY) + (xSpeed - xSpeed2) * (aBots[i]->xPos() - mPos.mX);
				float divider = 1 / (sqr(ySpeed - ySpeed2) + sqr(xSpeed - xSpeed2));
				time1 = (plop + root) * divider;
				time2 = (plop - root) * divider;
				if (time1 > 0 && time2 > 0)
				{
					if (time1 > time2)
						time1 = time2;
					if (minTime > time1)
						minTime = time1;
					if (time1 < 1.f && (aCollisionWithObstacles || time1 < mMovingTimeFactor))
					{	
						if (time1 < 0)
							time1 = time2;
						time1 -= mEpsilon;
						if (((CMovingGameObj *)aBots[i])->mMovingTimeFactor > time1)
							((CMovingGameObj *)aBots[i])->mMovingTimeFactor = time1;
						handleCollision(((const CMovingGameObj *)aBots[i])->getDamage());
						((CMovingGameObj *)aBots[i])->handleCollision(getDamage());
					}
				}
				else if ((time1 > mEpsilon && time2 < -mEpsilon) || (time1 < -mEpsilon && time2 > mEpsilon))
				{
					((CMovingGameObj *)aBots[i])->mMovingTimeFactor = 0.f;
					handleCollision(((const CMovingGameObj *)aBots[i])->getDamage());
					((CMovingGameObj *)aBots[i])->handleCollision(getDamage());
				}
			}
		}
	}
	if (mMovingTimeFactor > minTime)
		mMovingTimeFactor = minTime - mEpsilon;
	mCollisionDetected = (mMovingTimeFactor < 1.f ? true : false);
	return true;
}

bool CMovingGameObj::update()
{
	move(mMovingTimeFactor);
	if (mCollisionDetected)
		mVelocity = 0;
	mMovingTimeFactor = 1.f;
	return true;
}

bool CMovingGameObj::handleCollision(char)
{
	return true;
}

char CMovingGameObj::getDamage() const
{
	return 0;
}

void CMovingGameObj::changeFragNum(bool)
{
}

float CMovingGameObj::getNextEdge(TVector aPos, TVector aSpeed)
{
	float dX = aSpeed.mX < 0 ? floorf(aPos.mX) - aPos.mX : 1.f - aPos.mX + floorf(aPos.mX);
	float dY = aSpeed.mY < 0 ? floorf(aPos.mY) - aPos.mY : 1.f - aPos.mY + floorf(aPos.mY);

	if (dX == 0 && aSpeed.mX >= 0) dX = 1.f;
	if (dY == 0 && aSpeed.mY >= 0) dY = 1.f;

	float xTime = dX / aSpeed.mX, yTime = dY / aSpeed.mY;

	return (xTime < yTime ? xTime : yTime) + mEpsilon;
}
