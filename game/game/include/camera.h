#ifndef CAMERA_H
#define CAMERA_H

#include "gameObj.h"

enum TGameMode{EModeFPS, EModeRTS};

class CCamera : public MGameObj
{
public:
					CCamera			(uint16 aId, uint16 aIndex, float aHMin, float aHMax, float aVMin, float aVMax, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation);
					CCamera			(istream &aStream);
	virtual			~CCamera		();
	virtual void	externalize		(ostream &aStream);
	virtual void	handleMessage	(CMessage *aMsg);
	virtual void	transform		();
	void			setRTSMode		();
	void			setFPSMode		(CGameObjPtr aFPSTargetObj);
	void			setPos			(D3DXVECTOR3 *aPos);
	void			setPos			(D3DXVECTOR2 *aPos);
	void			scroll			(float aRight, float aUp);
protected:
	TGameMode		mMode;

	D3DXVECTOR3		mPos, mSpeed;
	D3DXQUATERNION	mOrientation, mRotSpeed;

	float			mTimeToDest;
	D3DXVECTOR3		mDestPos;
	D3DXQUATERNION	mDestOrientation;

	// RTS mode
	float			mRTSHeight;
	D3DXQUATERNION	mRTSOrientation;
	float			mHMin, mHMax, mVMin, mVMax;
	D3DXVECTOR3		mRTSRight, mRTSUp;

	// FPS mode
	CGameObjPtr		mFPSTargetObj;
};

#endif
