
#include "../include/buildingdata.h"

CBuildingData::CBuildingData(MAnimation *aAnimation, MAnimation *aConstructAnimation, int aHitpoints, int aPrice, int aTechnologyMask) :
	mAnimation(aAnimation), mConstructAnimation(aConstructAnimation), mHitpoints(aHitpoints), mPrice(aPrice), mTechnologyMask(aTechnologyMask)
{
}

CBuildingData::~CBuildingData()
{
	// AnimationStorage does the deleting, we don't need to.
}

MAnimation *CBuildingData::getAnimation()
{
	return mAnimation;
}

MAnimation *CBuildingData::getConstructAnimation()
{
	return mConstructAnimation;
}

uint32 CBuildingData::getConstructionTime() const
{
	return mConstructionTime;
}

int CBuildingData::getHitpoints() const
{
	return mHitpoints;
}

int CBuildingData::getPrice() const
{
	return mPrice;
}

int CBuildingData::getTechnologyMask() const
{
	return mTechnologyMask;
}
