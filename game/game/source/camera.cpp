#include "..\include\camera.h"

CCamera::CCamera(uint16 aId, uint16 aIndex, D3DXVECTOR3 aPos, D3DXQUATERNION aRot,
				 D3DXVECTOR3 aSpeed, D3DXQUATERNION aRotSpeed)
				: MGameObj(aId, aIndex), mPos(aPos), mSpeed(aSpeed),
				mRot(aRot), mRotSpeed(aRotSpeed)
{
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
	mRot *= mRotSpeed;
	D3DXVECTOR3 vec;
	float angle;
	D3DXQuaternionToAxisAngle(&mRot, &vec, &angle);

	d3dObj->mMatrixStack->LoadIdentity();
	d3dObj->mMatrixStack->RotateAxis(&vec, -angle);
	d3dObj->mMatrixStack->Translate(-mPos.x, -mPos.y, -mPos.z);
}
