#include "../include/player.h"
#include "../include/animationStorage.h"

CPlayer::CPlayer(CBuildingData **aBuildings, int aNumBuildings) :
	mBuildings(aBuildings), mNumBuildings(aNumBuildings), mMinerals(0)
{
	mNumBuildings = 2;
	mBuildings = new CBuildingData *[2];
	mBuildings[0] = new CBuildingData(
		CAnimationStorage::ptr()->getAnimation("data/turret.x"),
		CAnimationStorage::ptr()->getAnimation("data/turret.x"),
		10000, "Turret", NULL, 0, 100, 10, 69, 0, 0, 1, TURRET, 0,
		CAnimationStorage::ptr()->getTexture("data/images/turret_icon.bmp"));
	mBuildings[1] = new CBuildingData(
		CAnimationStorage::ptr()->getAnimation("data/silo.x"),
		CAnimationStorage::ptr()->getAnimation("data/silo.x"),
		10000, "Silo", NULL, 0, 100, 10, 69, 0, 0, 1, GENERAL, 0,
		CAnimationStorage::ptr()->getTexture("data/images/silo_icon.bmp"));
}

const CBuildingData *CPlayer::getBuildingData(int aIndex) const
{
	if (aIndex >= 0 && aIndex < mNumBuildings)
		return mBuildings[aIndex];
	return NULL;
}

int CPlayer::numBuildings() const
{
	return mNumBuildings;
}

int CPlayer::minerals() const
{
	return mMinerals;
}

void CPlayer::setMinerals(int aMinerals)
{
	mMinerals = aMinerals;
}
