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
				  MAnimation *aUpperBodyAnimation,
				  MAnimation *aConstructAnimation,
				  uint32 aConstructionTime,
				  char *aName,
				  uint32 *aRequires,
				  uint32 aNumRequires,
				  uint32 aHitpoints,
				  uint32 aPriceMineral,
				  uint32 aPriceGas,
				  uint32 aEnergyProductionNight,
				  uint32 aEnergyProductionDay,
				  uint32 aEnergyConsumption,
				  enum BuildingType aBuildingType,
				  uint32 aWeapon,
				  CTexture *aPicture);
	/**
	 * Destructor.
	 */
	~CBuildingData();
	void setId(int aId);
	void setAnimation(MAnimation *aAnimation);
	void setUpperBodyAnimation(MAnimation *aUpperBodyAnimation);
	void setConstructAnimation(MAnimation *aConstructAnimation);
	void setConstructionTime(uint32 aConstructionTime);
	void setName(char *aName);
	void setRequires(const std::vector<uint32> &aRequires);
	void setHitpoints(uint32 aHitpoints);
	void setPriceMineral(uint32 aPriceMineral);
	void setPriceGas(uint32 aPriceGas);
	void setEnergyProductionNight(uint32 aEnergyProductionNight);
	void setEnergyProductionDay(uint32 aEnergyProductionDay);
	void setEnergyConsumption(uint32 aEnergyConsumption);
	void setBuildingType(enum BuildingType aBuildingType);
	void setWeapon(uint32 aWeapon);
	void setPicture(CTexture *aPicture);
	void setTechnologies(const std::vector<uint32> &aTechnologies);

	uint32 getId() const;
	MAnimation *getAnimation() const;
	MAnimation *getUpperBodyAnimation() const;
	MAnimation *getConstructAnimation() const;
	uint32 getConstructionTime() const;
	std::string getName() const;
	uint32 *getRequires() const;
	uint32 getNumRequires() const;
	uint32 getTechnology(int aInd) const;
	uint32 getNumTechnologies() const;
	uint32 getHitpoints() const;
	uint32 getPriceMineral() const;
	uint32 getPriceGas() const;
	uint32 getEnergyProductionNight() const;
	uint32 getEnergyProductionDay() const;
	uint32 getEnergyConsumption() const;
	enum BuildingType getBuildingType() const;
	uint32 getWeapon() const;
	CTexture *getPicture() const;

	void dumpData() const;
private:
	uint32 mId;
	MAnimation *mAnimation;
	MAnimation *mUpperBodyAnimation;
	MAnimation *mConstructAnimation;
	uint32 mConstructionTime;

	std::string mName;
	uint32 *mRequires;
	uint32 mNumRequires;
	uint32 *mTechnologies;
	uint32 mNumTechnologies;
	uint32 mHitpoints;
	uint32 mPriceMineral;
	uint32 mPriceGas;
	uint32 mEnergyProductionNight;
	uint32 mEnergyProductionDay;
	uint32 mEnergyConsumption;
	enum BuildingType mBuildingType;
	uint32 mWeapon;
	CTexture *mPicture;
};

#endif // BUILDINGDATA_H
