#ifndef PLAYER_H
#define PLAYER_H

#include "buildingData.h"
#include "technologyData.h"
#include "weaponData.h"
#include "upgradeData.h"

class CPlayer
{
public:
	CPlayer(const char *aDataFile);
	CPlayer(CBuildingData **aBuildings, int aNumBuildings);
	CBuildingData *getBuildingData(int aIndex) const;
	CTechnologyData *getTechnologyData(int aIndex) const;
	CWeaponData *getWeaponData(int aIndex) const;
	CUpgradeData *getUpgradeData(int aIndex) const;
	int numBuildings() const;
	int numTechnologies() const;
	int numWeapons() const;
	int numUpgrades() const;
	int minerals() const;
	void setMinerals(int aMinerals);

	void setBuildingData(std::vector<CBuildingData *> &aBuildingData);
	void setTechnologyData(std::vector<CTechnologyData *> &aTechnologyData);
	void setWeaponData(std::vector<CWeaponData *> &aWeaponData);
	void setUpgradeData(std::vector<CUpgradeData *> &aUpgradeData);
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