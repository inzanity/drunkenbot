#ifndef MECH_H
#define MECH_H

#include "gameObj.h"

enum TMechMessage{EMsgMechMove, EMsgMechTurnAround, EMsgMechLast};

class CMech : public CDrawable
{
public:
	CMech(CGameObjPtr aObjPtr, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation);
	CMech(istream &aStream);
	virtual ~CMech();
	virtual void externalize(ostream &aStream);
	virtual void handleMessage(CMessage *aMsg);
	virtual void update(uint32 aTimeFactor);
//	virtual void draw(uint32 aTimeFactor);
	D3DXVECTOR3 getEyePos() const;
private:
	MAnimation *mUpperBody;
	D3DXQUATERNION mUBOrientation;
	float mSize;
};

#endif // MECH_H
