#include "..\include\camera.h"
#include "..\include\mech.h"

#define BRAKING_RATE 0.05f

CCamera::CCamera(CGameObjPtr aObjPtr, float aHMin, float aHMax, float aVMin, float aVMax, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation) :
	MGameObj(aObjPtr), mMode(EModeRTS), mPos(*aPos), mOrientation(*aOrientation),
	mTimeToDest(0), mRTSHeight(aPos->y), mRTSOrientation(*aOrientation),
	mHMin(aHMin), mHMax(aHMax), mVMin(aVMin), mVMax(aVMax)
{
	mSpeed = D3DXVECTOR3(0, 0, 0);
//	D3DXQuaternionIdentity(&mRotSpeed);
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

void CCamera::transform(uint32 aTimeFactor)
{
	D3DXVECTOR3 vec;
	float angle;
	// Move to destination
	if (mTimeToDest > 0)
	{
		if (mMode == EModeFPS) // If we're moving to FPS-mode, update destination
		{
			CMech *ptr = (CMech *)mFPSTargetObj.ptr();
			if (!ptr)
				setRTSMode(); // Mech not available, move back to RTS-mode
			else
			{
				mDestPos = ptr->getEyePos();
				mDestOrientation = *ptr->orientation();
			}
		}
		if (aTimeFactor >= mTimeToDest)
			aTimeFactor = mTimeToDest;
		float t = aTimeFactor / (float)mTimeToDest;
		mTimeToDest -= aTimeFactor;
		mPos += (mDestPos - mPos) * t;
		D3DXQuaternionSlerp(&mOrientation, &mOrientation, &mDestOrientation, t);
	}
	// Use mech position and orientation
	else if (mMode == EModeFPS)
	{
			CMech *ptr = (CMech *)mFPSTargetObj.ptr();
			if (!ptr)
				setRTSMode(); // Mech not available, move back to RTS-mode
			else
			{
				mPos = ptr->getEyePos();
				mOrientation = *ptr->orientation();
			}
	}
	// Update using current speed
	else
	{
		mPos += mSpeed * (float)aTimeFactor;
/*		if (!D3DXQuaternionIsIdentity(&mRotSpeed))
		{
			D3DXQuaternionToAxisAngle(&mRotSpeed, &vec, &angle);
			angle *= aTimeFactor;
			D3DXQUATERNION temp;
			D3DXQuaternionRotationAxis(&temp, &vec, angle);
			mOrientation *= temp;
		}
*/
		// Scrolling outside the map is not allowed
		if (mPos.x < mHMin) mPos.x = mHMin;
		if (mPos.x > mHMax) mPos.x = mHMax;
		if (mPos.z < mVMin) mPos.z = mVMin;
		if (mPos.z > mVMax) mPos.z = mVMax;

		// Slowdown the scrolling speed
		float brakingRate = BRAKING_RATE * aTimeFactor;
		if (mSpeed.x > brakingRate)
			mSpeed.x -= brakingRate;
		else if (mSpeed.x < -brakingRate)
			mSpeed.x += brakingRate;
		else
			mSpeed.x = 0;
		if (mSpeed.z > brakingRate)
			mSpeed.z -= brakingRate;
		else if (mSpeed.z < -brakingRate)
			mSpeed.z += brakingRate;
		else
			mSpeed.z = 0;
	}
	// Transformation
	D3DXQuaternionToAxisAngle(&mOrientation, &vec, &angle);
	d3dObj->mMatrixStack->LoadIdentity();
	d3dObj->mMatrixStack->Translate(-mPos.x, -mPos.y, -mPos.z);
	d3dObj->mMatrixStack->RotateAxis(&vec, -angle);
}

void CCamera::setRTSMode()
{
	if (mMode == EModeRTS) return;
	mTimeToDest = 400;
	D3DXMATRIX mat;
	D3DXVECTOR4 out;
	D3DXMatrixRotationQuaternion(&mat, &mRTSOrientation);
	D3DXVec3Transform(&out, &D3DXVECTOR3(0, 0, -(mRTSHeight - mPos.y)), &mat);
	mDestPos = mPos + (D3DXVECTOR3)out;
	if (mDestPos.x < mHMin) mDestPos.x = mHMin;
	if (mDestPos.x > mHMax) mDestPos.x = mHMax;
	if (mDestPos.z < mVMin) mDestPos.z = mVMin;
	if (mDestPos.z > mVMax) mDestPos.z = mVMax;
	mDestOrientation = mRTSOrientation;
	mMode = EModeRTS;
	mSpeed = D3DXVECTOR3(0, 0, 0);
}

void CCamera::setFPSMode(CGameObjPtr aFPSTargetObj)
{
	if (mMode == EModeFPS)
	{
		mFPSTargetObj = aFPSTargetObj; // Teleport to next destination
		return;
	}
	mTimeToDest = 400;
	mFPSTargetObj = aFPSTargetObj;
	mMode = EModeFPS;
	mSpeed = D3DXVECTOR3(0, 0, 0);
}

void CCamera::setPos(const D3DXVECTOR3 *aPos)
{
	mPos = *aPos;
}

void CCamera::setPos(const D3DXVECTOR2 *aPos)
{
	mPos.x = aPos->x; mPos.z = aPos->y;
}

void CCamera::setDest(uint32 aTime, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation)
{
	mTimeToDest = aTime;
	mDestPos = aPos ? *aPos : mPos;
	mDestOrientation = aOrientation ? *aOrientation : mOrientation;
	mSpeed = D3DXVECTOR3(0, 0, 0);
}

void CCamera::scroll(float aRight, float aUp)
{
	D3DXVECTOR3 temp;
	D3DXVec3Scale(&mSpeed, &mRTSRight, aRight);
	D3DXVec3Scale(&temp, &mRTSUp, aUp);
	D3DXVec3Add(&mSpeed, &mSpeed, &temp);
}

CGameObjPtr CCamera::targetObj() const
{
	if (mMode == EModeFPS && mTimeToDest == 0)
		return mFPSTargetObj;
	else
		return CGameObjPtr();
}

TGameMode CCamera::gameMode() const
{
	return mMode;
}
