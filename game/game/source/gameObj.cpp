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

	float width1, height1, length1;
	float width2, height2, length2;

	height1 = fabs(obj1.mMin.y - obj1.mMax.y);
	height2 = fabs(obj2.mMin.y - obj2.mMax.y);

	width1 = fabs(obj1.mMin.x - obj1.mMax.x);
	width2 = fabs(obj2.mMin.x - obj2.mMax.x);

	length1 = fabs(obj1.mMin.z - obj1.mMax.z);
	length2 = fabs(obj2.mMin.z - obj2.mMax.z);

	D3DXVECTOR3 pos1 = D3DXVECTOR3((obj1.mMax.x - width1) / 2, (obj1.mMax.y - height1) / 2, (obj1.mMax.z - length1) / 2); 
	D3DXVECTOR3 pos2 = D3DXVECTOR3((obj2.mMax.x - width2) / 2, (obj2.mMax.y - height2) / 2, (obj2.mMax.z - length1) / 2); 

	pos1 += *speed() * aTimeFactor + *pos();
	pos2 += *aObj->speed() * aTimeFactor + *aObj->pos();

/*	obj1.mMax += *speed() * aTimeFactor + *pos();
	obj1.mMin += *speed() * aTimeFactor + *pos();
	obj2.mMax += *aObj->speed() * aTimeFactor + *aObj->pos();
	obj2.mMin += *aObj->speed() * aTimeFactor + *aObj->pos();
*/
	if (fabs(pos1.x - pos2.x) < (width1+width2)/2 && fabs(pos1.y - pos2.y) < (height1+height2)/2 &&
		fabs(pos1.z - pos2.z) < (length1+length2)/2 )
		handleCollision(NULL);

/*	if ((obj1.mMax.x < obj2.mMax.x && obj1.mMax.x > obj2.mMin.x &&
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
	}*/
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
	TBox mBoundingBox = *mAnimation->getBoundingBox();
	float vb[5*4];
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3dObj->mD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	d3dObj->mD3DDevice->SetTransform(D3DTS_VIEW, d3dObj->mMatrixStack->GetTop());
	vb[0] = mBoundingBox.mMax.x; vb[1] = mBoundingBox.mMin.y; vb[2] = mBoundingBox.mMax.z;
	((D3DCOLOR *)vb)[3] = D3DCOLOR_XRGB(255, 0, 0);
	vb[4] = mBoundingBox.mMin.x; vb[5] = mBoundingBox.mMin.y; vb[6] = mBoundingBox.mMax.z;
	((D3DCOLOR *)vb)[7] = D3DCOLOR_XRGB(255, 0, 0);
	vb[8] = mBoundingBox.mMin.x; vb[9] = mBoundingBox.mMin.y; vb[10] = mBoundingBox.mMin.z;
	((D3DCOLOR *)vb)[11] = D3DCOLOR_XRGB(255, 0, 0);
	vb[12] = mBoundingBox.mMax.x; vb[13] = mBoundingBox.mMin.y; vb[14] = mBoundingBox.mMin.z;
	((D3DCOLOR *)vb)[15] = D3DCOLOR_XRGB(255, 0, 0);
	vb[16] = mBoundingBox.mMax.x; vb[17] = mBoundingBox.mMin.y; vb[18] = mBoundingBox.mMax.z;
	((D3DCOLOR *)vb)[19] = D3DCOLOR_XRGB(255, 0, 0);
	d3dObj->mD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vb, 4 * sizeof(float));
	vb[0] = mBoundingBox.mMax.x; vb[1] = mBoundingBox.mMax.y; vb[2] = mBoundingBox.mMax.z;
	((D3DCOLOR *)vb)[3] = D3DCOLOR_XRGB(255, 0, 0);
	vb[4] = mBoundingBox.mMin.x; vb[5] = mBoundingBox.mMax.y; vb[6] = mBoundingBox.mMax.z;
	((D3DCOLOR *)vb)[7] = D3DCOLOR_XRGB(255, 0, 0);
	vb[8] = mBoundingBox.mMin.x; vb[9] = mBoundingBox.mMax.y; vb[10] = mBoundingBox.mMin.z;
	((D3DCOLOR *)vb)[11] = D3DCOLOR_XRGB(255, 0, 0);
	vb[12] = mBoundingBox.mMax.x; vb[13] = mBoundingBox.mMax.y; vb[14] = mBoundingBox.mMin.z;
	((D3DCOLOR *)vb)[15] = D3DCOLOR_XRGB(255, 0, 0);
	vb[16] = mBoundingBox.mMax.x; vb[17] = mBoundingBox.mMax.y; vb[18] = mBoundingBox.mMax.z;
	((D3DCOLOR *)vb)[19] = D3DCOLOR_XRGB(255, 0, 0);
	d3dObj->mD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vb, 4 * sizeof(float));
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
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
