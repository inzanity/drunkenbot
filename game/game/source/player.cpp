#include "../include/player.h"
#include "../include/animationStorage.h"

CPlayer::CPlayer(CBuildingData **aBuildings, int aNumBuildings) :
	mBuildings(aBuildings), mNumBuildings(aNumBuildings), mMinerals(0)
{
	mNumBuildings = 2;
	mBuildings = new CBuildingData *[2];
	mBuildings[0] = new CBuildingData(
		CAnimationStorage::ptr()->getAnimation("data/turret.x"),
		CAnimationStorage::ptr()->getAnimation("data/turretTop.x"),
		CAnimationStorage::ptr()->getAnimation("data/turret.x"),
		10000, "Turret", NULL, 0, 100, 10, 69, 0, 0, 1, TURRET, 0,
		CAnimationStorage::ptr()->getTexture("data/images/turret_icon.bmp"));
	mBuildings[1] = new CBuildingData(
		CAnimationStorage::ptr()->getAnimation("data/silo.x"),
		NULL,
		CAnimationStorage::ptr()->getAnimation("data/silo.x"),
		10000, "Silo", NULL, 0, 100, 10, 69, 0, 0, 1, GENERAL, 0,
		CAnimationStorage::ptr()->getTexture("data/images/silo_icon.bmp"));

	mNumTechs = 1;
	mTechs = new CTechnologyData *[mNumTechs];
	mTechs[0] = new CTechnologyData();
	mTechs[0]->setPicture(CAnimationStorage::ptr()->getTexture("data/images/technology1_icon.bmp"));

	mNumWeapons = 1;
	mWeapons = new CWeaponData *[mNumWeapons];
	mWeapons[0] = new CWeaponData();
	mWeapons[0]->setPicture(CAnimationStorage::ptr()->getTexture("data/images/weapon1_icon.bmp"));

	mNumUpgrades = 1;
	mUpgrades = new CUpgradeData *[mNumUpgrades];
	mUpgrades[0] = new CUpgradeData();
	mUpgrades[0]->setPicture(CAnimationStorage::ptr()->getTexture("data/images/upgrade1_icon.bmp"));

	std::vector<uint32> techs;
	techs.push_back(0);
	mBuildings[1]->setTechnologies(techs);
}

CBuildingData *CPlayer::getBuildingData(int aIndex) const
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
