#include <math.h>
#include "../inc/gameobj.h"
#include "../inc/botinfo.h"
#include "../inc/tilemap.h"

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

CMovingGameObj::CMovingGameObj(int aType) : CGameObj(aType), mVelocity(0), mMovingDirection(0), mCollisionDetected(false),
											mMovingTimeFactor(0.f)
{
}

CMovingGameObj::CMovingGameObj(const CMovingGameObj *aGameObj) : CGameObj(aGameObj), mVelocity(aGameObj->mVelocity),
																 mMovingDirection(aGameObj->mVelocity),
																 mCollisionDetected(aGameObj->mCollisionDetected),
																 mMovingTimeFactor(aGameObj->mMovingDirection)
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
	float time, minTime = 1.f;
	float xSpeed = mVelocity * cos(mMovingDirection);
	float ySpeed = mVelocity * sin(mMovingDirection);
	mMovingTimeFactor = 1.f;

	if (mVelocity > 0)
	{
		TVector speed = {xSpeed, ySpeed};
		float t;
		char tile;
		int dir = (int)(mMovingDirection / PI * 4);
		for (int i = dir - 1; i <= dir + 2; i++)
		{
			TVector pos = {mPos.mX + cos(i * PI / 4.f) * mRadius,
						   mPos.mY + sin(i * PI / 4.f) * mRadius};
			for (t = 0; t < mMovingTimeFactor; t += getNextEdge(pos, speed))
			{
				pos.mX += t * speed.mX; pos.mY += t * speed.mY;
				tile = aTilemap[(int)pos.mY][(int)pos.mX] & 3;
				if (tile == CTilemap::ETileWall || (aCollisionWithObstacles && tile == CTilemap::ETileObstacle))
					break;
			}
			if (mMovingTimeFactor > t)
				mMovingTimeFactor = t;
		}
		if (mMovingTimeFactor < 1.f)
			handleCollision(15);
	}

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
					if (foo < minTime) minTime = foo;
					if (foo < 1.f && (aCollisionWithObstacles || foo < mMovingTimeFactor))
					{	
						if (((CMovingGameObj *)aBots[i])->mMovingTimeFactor > foo)
							((CMovingGameObj *)aBots[i])->mMovingTimeFactor = foo;
						handleCollision(((const CMovingGameObj *)aBots[i])->getDamage());
						((CMovingGameObj *)aBots[i])->handleCollision(getDamage());
					}
				}
			}
		}
	}
	if (mMovingTimeFactor > minTime)
		mMovingTimeFactor = minTime;
	mCollisionDetected = (mMovingTimeFactor < 1.f ? true : false);
	return true;
}

bool CMovingGameObj::update()
{
	move(mMovingTimeFactor);
	if (mCollisionDetected)
		mVelocity = 0;
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
