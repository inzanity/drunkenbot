#ifndef MECH_H
#define MECH_H

#include "gameObj.h"

enum TMechMessage{EMsgMechMove, EMsgMechTurnAround, EMsgMechLast};
enum TMechOperationMode{EMechManualMode, EMechAutoAimMode, EMechAutoFireMode, EMechScoutMode, EMechAttackMode, EMechDefenceMode};
class CMech : public CDrawable
{
public:
	CMech(CGameObjPtr aObjPtr, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation);
	CMech(istream &aStream);
	virtual ~CMech();
	virtual void externalize(ostream &aStream);
	virtual void handleMessage(CMessage *aMsg);
	virtual void update(uint32 aTimeFactor);
	virtual void draw(uint32 aTimeFactor);
	D3DXVECTOR3 getEyePos() const;
	float radarRange() const;
	bool radarDelay() const;
	void upperBodyAngle(float *aAngleX, float *aAngleY);
	void setUpperBodyAngle(float aAngleX, float aAngleY);
	void setUpperBodyAngleXSpeed(float angle);
	void setUpperBodyAngleYSpeed(float angle);
	float maxUBAngleX();
	float maxUBAngleY();
	float maxUBAngleXSpeed();
	float maxUBAngleYSpeed();
private:
	void addUpperBodyAngle(float aAngleX, float aAngleY);
	MAnimation *mUpperBody;
	D3DXVECTOR3 mUBPos;
	float mUpperBodyAngleX, mUpperBodyAngleY; 
	float mUpperBodyAngleXSpeed, mUpperBodyAngleYSpeed; 
	float mMaxUpperBodyAngleX, mMaxUpperBodyAngleY; 
	float mMaxUpperBodyAngleXSpeed, mMaxUpperBodyAngleYSpeed; 
	float mRadarRange;
	bool mRadarDelay;
	float mSize;
	bool mFPSMode;
	TMechOperationMode mOperationMode;
};

#endif // MECH_H
