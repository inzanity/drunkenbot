#ifndef CAMERA_H
#define CAMERA_H

#include "gameObj.h"

enum TGameMode{EModeFPS, EModeRTS};

class CCamera : public MGameObj
{
public:
					CCamera			(CGameObjPtr aObjPtr, float aHMin, float aHMax, float aVMin, float aVMax, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation);
					CCamera			(istream &aStream);
	virtual			~CCamera		();
	virtual void	externalize		(ostream &aStream);
	virtual void	handleMessage	(CMessage *aMsg);
	virtual void	transform		(uint32 aTimeFactor);
	void			setRTSMode		();
	void			setFPSMode		(CGameObjPtr aFPSTargetObj);
	void			setPos			(const D3DXVECTOR3 *aPos);
	void			setPos			(const D3DXVECTOR2 *aPos);
	void			setDest			(uint32 aTime, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation);
	void			scroll			(float aRight, float aUp);
	CGameObjPtr		targetObj		() const;
	TGameMode		gameMode		() const;
	const D3DXMATRIX *transformation() const;
protected:
	TGameMode		mMode;

	D3DXMATRIX		mTransformation;
	D3DXVECTOR3		mPos, mSpeed;
	D3DXQUATERNION	mOrientation;//, mRotSpeed;

	uint32			mTimeToDest;
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
