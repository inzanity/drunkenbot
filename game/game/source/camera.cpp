#include "..\include\camera.h"

#define BRAKING_RATE 0.05f

CCamera::CCamera(uint16 aId, uint16 aIndex, float aHMin, float aHMax, float aVMin, float aVMax, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
	MGameObj(aId, aIndex), mMode(EModeRTS), mPos(*aPos), mOrientation(*aOrientation),
	mTimeToDest(0.f), mRTSHeight(aPos->y), mRTSOrientation(*aOrientation),
	mHMin(aHMin), mHMax(aHMax), mVMin(aVMin), mVMax(aVMax)
{
	mSpeed = D3DXVECTOR3(0, 0, 0);
	mRotSpeed = D3DXQUATERNION(0, 0, 0, 1);
	D3DXVECTOR3 right(1, 0, 0), up(0, 1, 0);
	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &mOrientation);
	D3DXVECTOR4 out;
	D3DXVec3Transform(&out, &right, &mat);
	mRTSRight.x = out.x; mRTSRight.y = 0; mRTSRight.z = out.z;
	D3DXVec3Normalize(&mRTSRight, &mRTSRight);
	D3DXVec3Transform(&out, &up, &mat);
	mRTSUp.x = out.x; mRTSUp.y = 0; mRTSUp.z = out.z;
	D3DXVec3Normalize(&mRTSUp, &mRTSUp);

}

CCamera::CCamera(istream &aStream) : MGameObj(aStream)
{
}

CCamera::~CCamera()
{
}

void CCamera::externalize(ostream &aStream)
{
}

void CCamera::handleMessage(CMessage *)
{
}

void CCamera::transform()
{
	mPos += mSpeed;
	if (mMode == EModeRTS)
	{
		if (mPos.x < mHMin) mPos.x = mHMin;
		if (mPos.x > mHMax) mPos.x = mHMax;
		if (mPos.z < mVMin) mPos.z = mVMin;
		if (mPos.z > mVMax) mPos.z = mVMax;

		if (mSpeed.x > BRAKING_RATE)
			mSpeed.x -= BRAKING_RATE;
		else if (mSpeed.x < -BRAKING_RATE)
			mSpeed.x += BRAKING_RATE;
		else
			mSpeed.x = 0;
		if (mSpeed.z > BRAKING_RATE)
			mSpeed.z -= BRAKING_RATE;
		else if (mSpeed.z < -BRAKING_RATE)
			mSpeed.z += BRAKING_RATE;
		else
			mSpeed.z = 0;
	}
	mOrientation *= mRotSpeed;
	D3DXVECTOR3 vec;
	float angle;
	D3DXQuaternionToAxisAngle(&mOrientation, &vec, &angle);

	d3dObj->mMatrixStack->LoadIdentity();
	d3dObj->mMatrixStack->Translate(-mPos.x, -mPos.y, -mPos.z);
	d3dObj->mMatrixStack->RotateAxis(&vec, -angle);
}

void CCamera::setRTSMode()
{
	mMode = EModeRTS;
}

void CCamera::setFPSMode(CGameObjPtr aFPSTargetObj)
{
	mFPSTargetObj = aFPSTargetObj;
}

void CCamera::setPos(D3DXVECTOR3 *aPos)
{
	mPos = *aPos;
}

void CCamera::setPos(D3DXVECTOR2 *aPos)
{
	mPos.x = aPos->x; mPos.z = aPos->y;
}

void CCamera::scroll(float aRight, float aUp)
{
	D3DXVECTOR3 temp;
	D3DXVec3Scale(&mSpeed, &mRTSRight, aRight);
	D3DXVec3Scale(&temp, &mRTSUp, aUp);
	D3DXVec3Add(&mSpeed, &mSpeed, &temp);
}
