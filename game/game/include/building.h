#ifndef BUILDING_H
#define BUILDING_H

#include "gameObj.h"
#include "buildingData.h"

enum TBuildingMessage{EMsgBuildingReady, EMsgBuildingLast};

class CBuilding : public CDrawable
{
public:
	CBuilding(CGameObjPtr aObjPtr, CBuildingData *aBuildingData, bool aReady,
		const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation = &D3DXQUATERNION(0, 0, 0, 1));
	CBuilding(istream &aStream);
	virtual ~CBuilding();
	virtual void externalize(ostream &aStream);
	virtual void handleMessage(CMessage *aMsg);
private:
	CBuildingData *mBuildingData;
	int mHitpoints;
};

#endif // BUILDING_H
