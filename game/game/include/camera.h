#ifndef CAMERA_H
#define CAMERA_H

#include "gameObj.h"

class CCamera : public MGameObj
{
public:
					CCamera			(uint16 aId, uint16 aIndex, D3DXVECTOR3 aPos = D3DXVECTOR3(0, 0, 0),
									D3DXQUATERNION aRot = D3DXQUATERNION(0, 0, 0, 1),
									D3DXVECTOR3 aSpeed = D3DXVECTOR3(0, 0, 0),
									D3DXQUATERNION aRotSpeed = D3DXQUATERNION(0, 0, 0, 1));
					CCamera			(istream &aStream);
	virtual			~CCamera		();
	virtual void	externalize		(ostream &aStream);
	virtual void	handleMessage	(CMessage *aMsg);
	virtual void	transform		();
protected:
	D3DXVECTOR3		mPos, mSpeed;
	D3DXQUATERNION	mRot, mRotSpeed;
};

#endif