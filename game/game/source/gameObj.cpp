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

MColliding::MColliding(CGameObjPtr aObjPtr) : MGameObj(aObjPtr)
{
}

MColliding::MColliding(istream &aStream) : MGameObj(aStream)
{
}

MColliding::~MColliding()
{
}

void MColliding::checkCollision(MColliding *aObj, uint32 aTimeFactor)
{
	TBox obj1 = *boundingBox();
	TBox obj2 = *aObj->boundingBox();

	obj1.mMax += *speed() * aTimeFactor + *pos();
	obj1.mMin += *speed() * aTimeFactor + *pos();
	obj2.mMax += *aObj->speed() * aTimeFactor + *aObj->pos();
	obj2.mMin += *aObj->speed() * aTimeFactor + *aObj->pos();


	if ((obj1.mMax.x < obj2.mMax.x && obj1.mMax.x > obj2.mMin.x &&
		 obj1.mMax.y < obj2.mMax.y && obj1.mMax.y > obj2.mMin.y &&
		 obj1.mMax.z < obj2.mMax.z && obj1.mMax.z > obj2.mMin.z) ||

		 (obj1.mMin.x < obj2.mMax.x && obj1.mMin.x > obj2.mMin.x &&
		 obj1.mMax.y < obj2.mMax.y && obj1.mMax.y > obj2.mMin.y &&
		 obj1.mMax.z < obj2.mMax.z && obj1.mMax.z > obj2.mMin.z) ||

		 (obj1.mMax.x < obj2.mMax.x && obj1.mMax.x > obj2.mMin.x &&
		 obj1.mMin.y < obj2.mMax.y && obj1.mMin.y > obj2.mMin.y &&
		 obj1.mMax.z < obj2.mMax.z && obj1.mMax.z > obj2.mMin.z) ||

		 (obj1.mMax.x < obj2.mMax.x && obj1.mMax.x > obj2.mMin.x &&
		 obj1.mMax.y < obj2.mMax.y && obj1.mMax.y > obj2.mMin.y &&
		 obj1.mMin.z < obj2.mMax.z && obj1.mMin.z > obj2.mMin.z) ||

		 (obj1.mMin.x < obj2.mMax.x && obj1.mMin.x > obj2.mMin.x &&
		 obj1.mMin.y < obj2.mMax.y && obj1.mMin.y > obj2.mMin.y &&
		 obj1.mMax.z < obj2.mMax.z && obj1.mMax.z > obj2.mMin.z) ||

		 (obj1.mMin.x < obj2.mMax.x && obj1.mMin.x > obj2.mMin.x &&
		 obj1.mMax.y < obj2.mMax.y && obj1.mMax.y > obj2.mMin.y &&
		 obj1.mMin.z < obj2.mMax.z && obj1.mMin.z > obj2.mMin.z) ||

		 (obj1.mMax.x < obj2.mMax.x && obj1.mMax.x > obj2.mMin.x &&
		 obj1.mMin.y < obj2.mMax.y && obj1.mMin.y > obj2.mMin.y &&
		 obj1.mMin.z < obj2.mMax.z && obj1.mMin.z > obj2.mMin.z) ||

		 (obj1.mMin.x < obj2.mMax.x && obj1.mMin.x > obj2.mMin.x &&
		 obj1.mMin.y < obj2.mMax.y && obj1.mMin.y > obj2.mMin.y &&
		 obj1.mMin.z < obj2.mMax.z && obj1.mMin.z > obj2.mMin.z) ||
		 
		 (obj2.mMax.x < obj1.mMax.x && obj2.mMax.x > obj1.mMin.x &&
		 obj2.mMax.y < obj1.mMax.y && obj2.mMax.y > obj1.mMin.y &&
		 obj2.mMax.z < obj1.mMax.z && obj2.mMax.z > obj1.mMin.z) ||

		 (obj2.mMin.x < obj1.mMax.x && obj2.mMin.x > obj1.mMin.x &&
		 obj2.mMax.y < obj1.mMax.y && obj2.mMax.y > obj1.mMin.y &&
		 obj2.mMax.z < obj1.mMax.z && obj2.mMax.z > obj1.mMin.z) ||

		 (obj2.mMax.x < obj1.mMax.x && obj2.mMax.x > obj1.mMin.x &&
		 obj2.mMin.y < obj1.mMax.y && obj2.mMin.y > obj1.mMin.y &&
		 obj2.mMax.z < obj1.mMax.z && obj2.mMax.z > obj1.mMin.z) ||

		 (obj2.mMax.x < obj1.mMax.x && obj2.mMax.x > obj1.mMin.x &&
		 obj2.mMax.y < obj1.mMax.y && obj2.mMax.y > obj1.mMin.y &&
		 obj2.mMin.z < obj1.mMax.z && obj2.mMin.z > obj1.mMin.z) ||

		 (obj2.mMin.x < obj1.mMax.x && obj2.mMin.x > obj1.mMin.x &&
		 obj2.mMin.y < obj1.mMax.y && obj2.mMin.y > obj1.mMin.y &&
		 obj2.mMax.z < obj1.mMax.z && obj2.mMax.z > obj1.mMin.z) ||

		 (obj2.mMin.x < obj1.mMax.x && obj2.mMin.x > obj1.mMin.x &&
		 obj2.mMax.y < obj1.mMax.y && obj2.mMax.y > obj1.mMin.y &&
		 obj2.mMin.z < obj1.mMax.z && obj2.mMin.z > obj1.mMin.z) ||

		 (obj2.mMax.x < obj1.mMax.x && obj2.mMax.x > obj1.mMin.x &&
		 obj2.mMin.y < obj1.mMax.y && obj2.mMin.y > obj1.mMin.y &&
		 obj2.mMin.z < obj1.mMax.z && obj2.mMin.z > obj1.mMin.z) ||

		 (obj2.mMin.x < obj1.mMax.x && obj2.mMin.x > obj1.mMin.x &&
		 obj2.mMin.y < obj1.mMax.y && obj2.mMin.y > obj1.mMin.y &&
		 obj2.mMin.z < obj1.mMax.z && obj2.mMin.z > obj1.mMin.z))
	{
		handleCollision(aObj);
		aObj->handleCollision(this);
	}
}

// CDrawable

CDrawable::CDrawable(CGameObjPtr aObjPtr, MAnimation *aAnim,
					 float aAnimSpeed, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
	MColliding(aObjPtr), mPos(*aPos), mSpeed(0, 0, 0), mXAngle(aXAngle), mYAngle(aYAngle), mXRotSpeed(0), mYRotSpeed(0)
{
	setAnimation(aAnim, aAnimSpeed);
}

CDrawable::CDrawable(CGameObjPtr aObjPtr) : MColliding(aObjPtr)
{
}

CDrawable::CDrawable(istream &aStream) : MColliding(aStream)
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

	d3dObj->mMatrixStack->TranslateLocal(mPos.x, mPos.y, mPos.z);
	d3dObj->mMatrixStack->RotateYawPitchRollLocal(mYAngle, mXAngle, 0);

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

void CDrawable::setOrientation(float aXAngle, float aYAngle)
{
	mXAngle = aXAngle; mYAngle = aYAngle;
}

void CDrawable::setRotSpeed(float aXRotSpeed, float aYRotSpeed)
{
	mXRotSpeed = aXRotSpeed; mYRotSpeed = aYRotSpeed;
}

const D3DXVECTOR3 *CDrawable::pos() const
{
	return &mPos;
}

const D3DXVECTOR3 *CDrawable::speed() const
{
	return &mSpeed;
}

float CDrawable::xRot() const
{
	return mXAngle;
}

float CDrawable::yRot() const
{
	return mYAngle;
}

float CDrawable::xRotSpeed() const
{
	return mXRotSpeed;
}

float CDrawable::yRotSpeed() const
{
	return mYRotSpeed;
}

void CDrawable::update(uint32 aTimeFactor)
{
	mPos += mSpeed * (float)aTimeFactor;
	mXAngle += mXRotSpeed * aTimeFactor;
	if (mXAngle > 2 * D3DX_PI) mXAngle -= 2 * D3DX_PI;
	else if (mXAngle < 0) mXAngle += 2 * D3DX_PI;
	mYAngle += mYRotSpeed * aTimeFactor;
	if (mYAngle > 2 * D3DX_PI) mYAngle -= 2 * D3DX_PI;
	else if (mYAngle < 0) mYAngle += 2 * D3DX_PI;
}

float CDrawable::radiusSqr() const
{
	return mAnimation->getRadiusSqr();
}

const TBox *CDrawable::boundingBox() const
{
	return mAnimation->getBoundingBox();
}
