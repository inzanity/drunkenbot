#ifndef PLAYER_H
#define PLAYER_H

#include "buildingData.h"
#include "technologyData.h"
#include "weaponData.h"
#include "upgradeData.h"

class CPlayer
{
public:
	CPlayer(CBuildingData **aBuildings, int aNumBuildings);
	CBuildingData *getBuildingData(int aIndex) const;
	int numBuildings() const;
	int minerals() const;
	void setMinerals(int aMinerals);
private:
	CBuildingData **mBuildings;
	int mNumBuildings;
	CTechnologyData **mTechs;
	int mNumTechs;
	CWeaponData **mWeapons;
	int mNumWeapons;
	CUpgradeData **mUpgrades;
	int mNumUpgrades;
	int mMinerals;
	int mGas;
};

#endif // PLAYER_H