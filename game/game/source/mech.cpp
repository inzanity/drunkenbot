#include "../include/game.h"
#include "../include/animationStorage.h"


#define GRAVITY 0.05f

CMech::CMech(CGameObjPtr aObjPtr, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
	CDrawable(aObjPtr, CAnimationStorage::ptr()->getAnimation("data/mech.x"), 1.f, aPos, aOrientation),
	mUpperBody(CAnimationStorage::ptr()->getAnimation("data/mechTop.x")), mUpperBodyAngleX(0),
	mUpperBodyAngleY(0), mUpperBodyAngleXSpeed(0), mUpperBodyAngleYSpeed(0), mFPSMode(false),
	mOperationMode(EMechManualMode), mRadarRange(40), mRadarDelay(true),
	mMaxUpperBodyAngleX(.5f), mMaxUpperBodyAngleY(.5f),
	mMaxUpperBodyAngleXSpeed(0.08f), mMaxUpperBodyAngleYSpeed(0.08f)
{
	const TBox *box = mAnimation->getBoundingBox();
	mUBPos = D3DXVECTOR3(0, box->mMax.y, 0);

	D3DXQuaternionRotationYawPitchRoll(&mOrientation, 0.5f, 0, 0);
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

void CMech::handleMessage(CMessage *aMsg)
{
	D3DXMATRIX mat;
	D3DXVECTOR4 out;
	switch (aMsg->mMsg)
	{
	case EMsgMechMove:
		D3DXMatrixRotationQuaternion(&mat, &mOrientation);
		D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, .002f), &mat);
		D3DXQuaternionIdentity(&mRotSpeed);
		mSpeed = (D3DXVECTOR3)out;
		game->sendMessage(EMsgMechTurnAround, this, 0, 0, 20000);
		break;
	case EMsgMechTurnAround:
		mSpeed = D3DXVECTOR3(0, 0, 0);
		D3DXQuaternionRotationYawPitchRoll(&mRotSpeed, 3.1415f/2000.f, 0, 0);
		game->sendMessage(EMsgMechMove, this, 0, 0, 2000);
		break;
	}
}

void CMech::update(uint32 aTimeFactor)
{
	CDrawable::update(aTimeFactor);
	addUpperBodyAngle(aTimeFactor * mUpperBodyAngleXSpeed / 100.f, aTimeFactor * mUpperBodyAngleYSpeed / 100.f);
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

	D3DXVECTOR3 vec;
	float angle;
	d3dObj->mMatrixStack->TranslateLocal(mPos.x, mPos.y, mPos.z);
	D3DXQuaternionToAxisAngle(&mOrientation, &vec, &angle);
	d3dObj->mMatrixStack->RotateAxisLocal(&vec, angle);

	mAnimTime += (uint32)(aTimeFactor * mAnimSpeed);
	if (mAnimation)
		mAnimation->draw(mAnimTime);
	d3dObj->mMatrixStack->TranslateLocal(mUBPos.x, mUBPos.y, mUBPos.z);
	d3dObj->mMatrixStack->RotateAxisLocal(&D3DXVECTOR3(0, 1, 0), mUpperBodyAngleX);
	d3dObj->mMatrixStack->RotateAxisLocal(&D3DXVECTOR3(1, 0, 0), mUpperBodyAngleY);
	if (mUpperBody)
		mUpperBody->draw(mAnimTime);
	d3dObj->mMatrixStack->Pop();
}