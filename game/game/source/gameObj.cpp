#include "..\include\gameObj.h"

// MGameObj

MGameObj::MGameObj(uint16 aId, uint16 aIndex) : mIndex(aIndex), mId(aId)
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

uint16 MGameObj::index()
{
	return mIndex;
}

uint16 MGameObj::id()
{
	return mId;
}

// MColliding

MColliding::~MColliding()
{
}

void MColliding::checkCollision(MColliding *aObj)
{
}

// CDrawable

CDrawable::CDrawable(uint16 aId, uint16 aIndex, MAnimation *aAnim,
					 float aAnimSpeed, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
	MGameObj(aId, aIndex), mPos(*aPos), mSpeed(0, 0, 0), mOrientation(*aOrientation), mRotSpeed(0, 0, 0, 1)
{
	setAnimation(aAnim, aAnimSpeed);
}

CDrawable::CDrawable(uint16 aId, uint16 aIndex) : MGameObj(aId, aIndex)
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
	d3dObj->mD3DDevice->SetTransform(D3DTS_VIEW, d3dObj->mMatrixStack->GetTop());

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

void CDrawable::update(uint32 aTime)
{
	mPos += mSpeed * (float)aTime;
	mOrientation *= mRotSpeed * (float)aTime;
}

float CDrawable::radiusSqr() const
{
	return 0.f;
}

TBox *CDrawable::getCollisionRect() const
{
	return NULL;
}
