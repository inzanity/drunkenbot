#include "../include/gameObj.h"
#include "../include/game.h"

// CGameObjPtr

CGameObjPtr::CGameObjPtr() : mIndex(KNullIndex), mId(0xFFFF)
{
}

CGameObjPtr::CGameObjPtr(uint16 aId, uint16 aIndex) : mIndex(aIndex), mId(aId)
{
}


void CGameObjPtr::setPtr(uint16 aId, uint16 aIndex)
{
	mId = aId;
	mIndex = aIndex;
}

void CGameObjPtr::setPtrToNULL()
{
	mId = 0;
	mIndex = KNullIndex;
}

MGameObj *CGameObjPtr::ptr()
{
	return game->getGameObj(mIndex, mId);
}

uint16 CGameObjPtr::index() const
{
	return mIndex;
}

uint16 CGameObjPtr::id() const
{
	return mId;
}

// MGameObj

MGameObj::MGameObj(CGameObjPtr aObjPtr) : mObjectPtr(aObjPtr)
{
}

MGameObj::MGameObj(istream &aStream)
{
}

MGameObj::~MGameObj()
{
}

void MGameObj::externalize(ostream &aStream)
{
}

CGameObjPtr MGameObj::objectPtr()
{
	return mObjectPtr;
}

// MColliding

MColliding::~MColliding()
{
}

void MColliding::checkCollision(MColliding *aObj)
{
}

// CDrawable

CDrawable::CDrawable(CGameObjPtr aObjPtr, MAnimation *aAnim,
					 float aAnimSpeed, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
	MGameObj(aObjPtr), mPos(*aPos), mSpeed(0, 0, 0), mOrientation(*aOrientation), mRotSpeed(0, 0, 0, 1)
{
	setAnimation(aAnim, aAnimSpeed);
}

CDrawable::CDrawable(CGameObjPtr aObjPtr) : MGameObj(aObjPtr)
{
}

CDrawable::CDrawable(istream &aStream) : MGameObj(aStream)
{
}

CDrawable::~CDrawable()
{
}

void CDrawable::externalize(ostream &aStream)
{
}

void CDrawable::draw(uint32 aTimeFactor)
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
	d3dObj->mMatrixStack->Pop();
}

void CDrawable::setAnimation(MAnimation *aAnim, float aAnimSpeed)
{
	mAnimation = aAnim;
	mAnimSpeed = aAnimSpeed;
	mAnimTime = 0;
}

float CDrawable::animSpeed()
{
	return mAnimSpeed;
}

void CDrawable::setAnimSpeed(float aAnimSpeed)
{
	mAnimSpeed = aAnimSpeed;
}

void CDrawable::handleMessage(CMessage *)
{
}

void CDrawable::handleCollision(const MGameObj *aObj)
{
}

void CDrawable::setPos(const D3DXVECTOR3 *aPos)
{
	mPos = *aPos;
}

void CDrawable::setSpeed(const D3DXVECTOR3 *aSpeed)
{
	mSpeed = *aSpeed;
}

void CDrawable::setOrientation(const D3DXQUATERNION *aOrientation)
{
	mOrientation = *aOrientation;
}

void CDrawable::setRotSpeed(const D3DXQUATERNION *aRotSpeed)
{
	mRotSpeed = *aRotSpeed;
}

const D3DXVECTOR3 *CDrawable::pos() const
{
	return &mPos;
}

const D3DXVECTOR3 *CDrawable::speed() const
{
	return &mSpeed;
}

const D3DXQUATERNION *CDrawable::orientation() const
{
	return &mOrientation;
}

const D3DXQUATERNION *CDrawable::rotSpeed() const
{
	return &mRotSpeed;
}

void CDrawable::update(uint32 aTimeFactor)
{
	mPos += mSpeed * (float)aTimeFactor;
	if (!D3DXQuaternionIsIdentity(&mRotSpeed))
	{
		D3DXVECTOR3 vec;
		float angle;
		D3DXQuaternionToAxisAngle(&mRotSpeed, &vec, &angle);
		angle *= aTimeFactor;
		D3DXQUATERNION temp;
		D3DXQuaternionRotationAxis(&temp, &vec, angle);
		mOrientation *= temp;
	}
}

float CDrawable::radiusSqr() const
{
	return mAnimation->getRadiusSqr();
}

const TBox *CDrawable::boundingBox() const
{
	return mAnimation->getBoundingBox();
}
