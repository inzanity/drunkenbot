
#include "buildingdata.h"

CBuildingData::CBuildingData(const MAnimation *aAnimation, const MAnimation *aConstructAnimation, int aHitpoints, int aPrice, int aTechnologyMask) :
	mAnimation(aAnimation), mConstructAnimation(aConstructAnimation), mHitpoints(aHitpoints), mPrice(aPrice), mTechnologyMask(aTechnologyMask)
{
}

~CBuildingData()
{
	// AnimationStorage does the deleting, we don't need to.
}

const MAnimation *CBuildingData::getAnimation() const
{
	return mAnimation;
}

const MAnimation *CBuildingData::getConstructAnimation() const
{
	return mConstructAnimation;
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
