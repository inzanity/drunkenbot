#include "../include/building.h"
#include "../include/message.h"

CBuilding::CBuilding(CGameObjPtr aObjPtr, CBuildingData *aBuildingData, bool aReady,
					 const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
	CDrawable(aObjPtr, NULL, 1.f, aPos, aXAngle, aYAngle), 
	mBuildingData(aBuildingData), mHitpoints(aBuildingData->getHitpoints())
{
	if (aReady)
		setAnimation(mBuildingData->getAnimation());
	else
	{
		MAnimation *anim = mBuildingData->getConstructAnimation();
		setAnimation(anim, anim->getDuration() / (float)mBuildingData->getConstructionTime());
	}
}

CBuilding::CBuilding(istream &aStream) : CDrawable(aStream)
{
}

CBuilding::~CBuilding()
{
}

void CBuilding::externalize(ostream &aStream)
{
}

void CBuilding::handleMessage(CMessage *aMsg)
{
	switch (aMsg->mMsg)
	{
	case EMsgBuildingReady:
		setAnimation(mBuildingData->getAnimation());
		break;
	}
}

