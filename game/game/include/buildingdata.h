#ifndef BUILDINGDATA_H
#define BUILDINGDATA_H


#include <string>
#include <vector>
#include "animation.h"

/*#define MAnimation char
#define CTexture char
#define uint32 unsigned int */
enum BuildingType {
	GENERAL = 1, TURRET, PIPE, DOCKYARD
};

// using namespace std;

class CBuildingData
{
public:
	CBuildingData();
	CBuildingData(MAnimation *aAnimation,
				  MAnimation *aConstructAnimation,
				  uint32 aConstructionTime,
				  char *aName,
				  int *aRequires,
				  int aNumRequires,
				  int aHitpoints,
				  int aPriceMineral,
				  int aPriceGas,
				  int aEnergyProductionNight,
				  int aEnergyProductionDay,
				  int aEnergyConsumption,
				  enum BuildingType aBuildingType,
				  int aWeapon,
				  CTexture *aPicture);
	/**
	 * Destructor.
	 */
	~CBuildingData();
	void setId(int aId);
	void setAnimation(MAnimation *aAnimation);
	void setConstructAnimation(MAnimation *aConstructAnimation);
	void setConstructionTime(uint32 aConstructionTime);
	void setName(char *aName);
	void setRequires(const std::vector<int> &aRequires);
	void setHitpoints(int aHitpoints);
	void setPriceMineral(int aPriceMineral);
	void setPriceGas(int aPriceGas);
	void setEnergyProductionNight(int aEnergyProductionNight);
	void setEnergyProductionDay(int aEnergyProductionDay);
	void setEnergyConsumption(int aEnergyConsumption);
	void setBuildingType(enum BuildingType aBuildingType);
	void setWeapon(int aWeapon);
	void setPicture(CTexture *aPicture);
	void setTechnologies(const std::vector<int> &aTechnologies);
	/**
	 * Getter for the animation file path.
	 */
	MAnimation *getAnimation();
	/**
	 * Getter for the constructing animation file path.
	 */
	MAnimation *getConstructAnimation();
	/**
	 * Getter for construction time.
	 */
	uint32 getConstructionTime() const;
	/**
	 * Getter for hitpoints.
	 */
	int getHitpoints() const;
	/**
	 * Getter for price.
	 */
	int getPriceGas() const;

	void dumpData() const;
private:
	uint32 mID;
	MAnimation *mAnimation;
	MAnimation *mConstructAnimation;
	uint32 mConstructionTime;

	std::string mName;
	int mId;
	int *mRequires;
	int mNumRequires;
	int *mTechnologies;
	int mNumTechnologies;
	int mHitpoints;
	int mPriceMineral;
	int mPriceGas;
	int mEnergyProductionNight;
	int mEnergyProductionDay;
	int mEnergyConsumption;
	enum BuildingType mBuildingType;
	int mWeapon;
	CTexture *mPicture;
};

#endif // BUILDINGDATA_H