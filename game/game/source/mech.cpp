#include "../include/mech.h"
#include "../include/game.h"
#include "../include/animationStorage.h"


#define GRAVITY 0.05f

CMech::CMech(CGameObjPtr aObjPtr, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
	CDrawable(aObjPtr, CAnimationStorage::ptr()->getAnimation("data/mech.x"), 1.f, aPos, aXAngle, aYAngle),
	mUpperBody(CAnimationStorage::ptr()->getAnimation("data/mechTop.x")), mUpperBodyAngleX(0),
	mUpperBodyAngleY(0), mUpperBodyAngleXSpeed(0), mUpperBodyAngleYSpeed(0), mFPSMode(false),
	mOperationMode(EMechManualMode), mRadarRange(40), mRadarDelay(true),
	mMaxUpperBodyAngleX(.5f), mMaxUpperBodyAngleY(.5f),
	mMaxUpperBodyAngleXSpeed(0.0008f), mMaxUpperBodyAngleYSpeed(0.0008f),
	mMaxSpeed(.005f), mMaxRotSpeed(.003f),
	mMoveToDest(false)
{
	const TBox *box = mAnimation->getBoundingBox();
	mUBPos = D3DXVECTOR3(0, box->mMax.y, 0);

//	game->sendMessage(EMsgMechMove, this, 0, 0, 0);
	mSize = 2;
}

CMech::CMech(istream &aStream) :
	CDrawable(aStream)
{
}

CMech::~CMech()
{
}

void CMech::externalize(ostream &aStream)
{
}

void CMech::handleCollision(const MGameObj *aObj)
{
	setSpeed(&D3DXVECTOR3(0, 0, 0));
}

void CMech::checkMapCollision(uint32 aTimeFactor)
{
	D3DXVECTOR3 dest = (mSpeed * aTimeFactor + mPos);
	if(	dest.y + 0.05f/*+ maxrise*/< game->mHeightMap->height(dest.x, dest.z)) 
		handleCollision(NULL);
}

void CMech::handleMessage(CMessage *aMsg)
{
	switch (aMsg->mMsg)
	{
	case EMsgMechMove:
		mDestPos.x = *(float *)&aMsg->mParam1; mDestPos.z = *(float *)&aMsg->mParam2;
		mDestPos.y = game->mHeightMap->height(mDestPos.x, mDestPos.z);
		mMoveToDest = true;
		break;
	}
}

void CMech::update(uint32 aTimeFactor)
{
	if (mMoveToDest)
	{
		float destAngle = atan2f(mDestPos.x - mPos.x, mDestPos.z - mPos.z);
		mYRotSpeed = destAngle - mYAngle;
		if (mYRotSpeed > D3DX_PI) mYRotSpeed = D3DX_PI - mYRotSpeed;
		else if (mYRotSpeed < -D3DX_PI) mYRotSpeed += 2 * D3DX_PI;
		mSpeed = D3DXVECTOR3(sin(mYAngle)*mMaxSpeed, 0, cos(mYAngle)*mMaxSpeed);
		D3DXVECTOR3 dir = mDestPos - mPos;
		D3DXVECTOR3 newDir = dir - mSpeed;
		float len = D3DXVec3LengthSq(&dir);
		if (len < mMaxSpeed)
		{
			mYRotSpeed = 0;
			mSpeed = D3DXVECTOR3(0, 0, 0);
			mMoveToDest = false;
		}
		else if (fabs(mYRotSpeed) > D3DX_PI / 4.f ||
			(dir.x > 0 && newDir.x > dir.x) || (dir.x < 0 && newDir.x < dir.x) ||
			(dir.z > 0 && newDir.z > dir.z) || (dir.z < 0 && newDir.z < dir.z))
			mSpeed = D3DXVECTOR3(0, 0, 0);
		if (mYRotSpeed > mMaxRotSpeed) mYRotSpeed = mMaxRotSpeed;
		else if (mYRotSpeed < -mMaxRotSpeed) mYRotSpeed = -mMaxRotSpeed;
	}
	CDrawable::update(aTimeFactor);
	addUpperBodyAngle(aTimeFactor * mUpperBodyAngleXSpeed, aTimeFactor * mUpperBodyAngleYSpeed);
	float h = game->mHeightMap->height(mPos.x, mPos.z);
	if (mPos.y > h + GRAVITY)
		mPos.y -= GRAVITY;
	else
		mPos.y = h;
}

// TODO
D3DXVECTOR3 CMech::getEyePos() const
{
	D3DXVECTOR3 eye = mPos;
	eye.y += mSize * 0.9f;

	return eye;
}

float CMech::radarRange() const
{
	return mRadarRange;
}

bool CMech::radarDelay() const
{
	return mRadarDelay;
}

void CMech::upperBodyAngle(float *aAngleX, float *aAngleY)
{
	if(aAngleX != NULL)	*aAngleX = mUpperBodyAngleX;
	if(aAngleX != NULL)	*aAngleY = mUpperBodyAngleY;
}

void CMech::setUpperBodyAngle(float aAngleX, float aAngleY)
{
	mUpperBodyAngleX = aAngleX;
	mUpperBodyAngleY = aAngleY;
}

void CMech::addUpperBodyAngle(float aAngleX, float aAngleY)
{
	mUpperBodyAngleX += aAngleX;
	mUpperBodyAngleY += aAngleY;

	if(mUpperBodyAngleX > mMaxUpperBodyAngleX) mUpperBodyAngleX = mMaxUpperBodyAngleX;
	if(mUpperBodyAngleY > mMaxUpperBodyAngleY) mUpperBodyAngleY = mMaxUpperBodyAngleY;
	if(mUpperBodyAngleX < -mMaxUpperBodyAngleX) mUpperBodyAngleX = -mMaxUpperBodyAngleX;
	if(mUpperBodyAngleY < -mMaxUpperBodyAngleY) mUpperBodyAngleY = -mMaxUpperBodyAngleY;
}

void CMech::setUpperBodyAngleXSpeed(float angle) 
{
	mUpperBodyAngleXSpeed = angle;
}

void CMech::setUpperBodyAngleYSpeed(float angle) 
{
	mUpperBodyAngleYSpeed = angle;
}

float CMech::maxRotSpeed()
{
	return mMaxRotSpeed;
}

float CMech::maxSpeed()
{
	return mMaxSpeed;
}

float CMech::maxUBAngleX()
{
	return mMaxUpperBodyAngleX;
}

float CMech::maxUBAngleY()
{
	return mMaxUpperBodyAngleY;
}

float CMech::maxUBAngleXSpeed()
{
	return mMaxUpperBodyAngleXSpeed;
}

float CMech::maxUBAngleYSpeed()
{
	return mMaxUpperBodyAngleYSpeed;
}


void CMech::draw(uint32 aTimeFactor)
{
	d3dObj->mMatrixStack->Push();

	d3dObj->mMatrixStack->TranslateLocal(mPos.x, mPos.y, mPos.z);
	d3dObj->mMatrixStack->RotateYawPitchRollLocal(mYAngle, mXAngle, 0);

	mAnimTime += (uint32)(aTimeFactor * mAnimSpeed);
	if (mAnimation)
		mAnimation->draw(mAnimTime);
	d3dObj->mMatrixStack->TranslateLocal(mUBPos.x, mUBPos.y, mUBPos.z);
	d3dObj->mMatrixStack->RotateYawPitchRollLocal(mUpperBodyAngleY, mUpperBodyAngleX, 0);
	if (mUpperBody)
		mUpperBody->draw(mAnimTime);
	d3dObj->mMatrixStack->Pop();
}