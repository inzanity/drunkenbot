#include "../include/buildingdata.h"

CBuildingData::CBuildingData(MAnimation *aAnimation,
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
							 CTexture *aPicture,
							 int aShortcut) : 
								    mAnimation(aAnimation),
									mUpperBodyAnimation(aUpperBodyAnimation),
									mConstructAnimation(aConstructAnimation),
									mConstructionTime(aConstructionTime),
									mName(aName),
									mRequires(aRequires),
									mNumRequires(aNumRequires),
									mHitpoints(aHitpoints),
									mPriceMineral(aPriceMineral),
									mPriceGas(aPriceGas),
									mEnergyProductionNight(aEnergyProductionNight),
									mEnergyProductionDay(aEnergyProductionDay),
									mEnergyConsumption(aEnergyConsumption),
									mBuildingType(aBuildingType),
									mWeapon(aWeapon),
									mPicture(aPicture),
									mShortcut(aShortcut)
{
}

CBuildingData::CBuildingData() :
									mId(0),
								    mAnimation(0),
									mConstructAnimation(0),
									mConstructionTime(0),
									mName(""),
									mRequires(0),
									mNumRequires(0),
									mHitpoints(0),
									mPriceMineral(0),
									mPriceGas(0),
									mEnergyProductionNight(0),
									mEnergyProductionDay(0),
									mEnergyConsumption(0),
									mBuildingType(GENERAL),
									mWeapon(0),
									mPicture(0)
{
}

CBuildingData::~CBuildingData()
{
	// AnimationStorage does the deleting, we don't need to.
	if (mRequires)
		delete [] mRequires;
	if (mTechnologies)
		delete [] mTechnologies;
}

void CBuildingData::setId(int aId)
{
	mId = aId;
}
void CBuildingData::setAnimation(MAnimation *aAnimation)
{
	mAnimation = aAnimation;
}
void CBuildingData::setUpperBodyAnimation(MAnimation *aUpperBodyAnimation)
{
	mUpperBodyAnimation = aUpperBodyAnimation;
}
void CBuildingData::setConstructAnimation(MAnimation *aConstructAnimation)
{
	mConstructAnimation = aConstructAnimation;
}
void CBuildingData::setConstructionTime(uint32 aConstructionTime)
{
	mConstructionTime = aConstructionTime;
}
void CBuildingData::setName(char *aName)
{
	mName = aName;
}

void CBuildingData::setRequires(const std::vector<uint32> &aRequires)
{
	mNumRequires = (uint32)aRequires.size();
	if (mNumRequires)
	{
		mRequires = new uint32[mNumRequires];
		for (uint32 i = 0; i < mNumRequires; i++)
			mRequires[i] = aRequires[i];
	}
}
void CBuildingData::setTechnologies(const std::vector<uint32> &aTechnologies)
{
	mNumTechnologies = (uint32)aTechnologies.size();
	if (mNumTechnologies) {
		mTechnologies = new uint32[mNumTechnologies];
		for (uint32 i = 0; i < mNumTechnologies; i++)
			mTechnologies[i] = aTechnologies[i];
	}
}
void CBuildingData::setHitpoints(uint32 aHitpoints)
{
	mHitpoints = aHitpoints;
}
void CBuildingData::setPriceMineral(uint32 aPriceMineral)
{
	mPriceMineral = aPriceMineral;
}
void CBuildingData::setPriceGas(uint32 aPriceGas)
{
	mPriceGas = aPriceGas;
}
void CBuildingData::setEnergyProductionNight(uint32 aEnergyProductionNight)
{
	mEnergyProductionNight = aEnergyProductionNight;
}
void CBuildingData::setEnergyProductionDay(uint32 aEnergyProductionDay)
{
	mEnergyProductionDay = aEnergyProductionDay;
}
void CBuildingData::setEnergyConsumption(uint32 aEnergyConsumption)
{
	mEnergyConsumption = aEnergyConsumption;
}
void CBuildingData::setBuildingType(enum BuildingType aBuildingType)
{
	mBuildingType = aBuildingType;
}
void CBuildingData::setWeapon(uint32 aWeapon)
{
	mWeapon = aWeapon;
}
void CBuildingData::setPicture(CTexture *aPicture)
{
	mPicture = aPicture;
}
void CBuildingData::setShortcut(int aShortcut)
{
	mShortcut = aShortcut;
}

uint32 CBuildingData::getId() const
{
	return mId;
}

MAnimation *CBuildingData::getAnimation() const
{
	return mAnimation;
}

MAnimation *CBuildingData::getUpperBodyAnimation() const
{
	return mUpperBodyAnimation;
}

MAnimation *CBuildingData::getConstructAnimation() const
{
	return mConstructAnimation;
}

uint32 CBuildingData::getConstructionTime() const
{
	return mConstructionTime;
}


std::string CBuildingData::getName() const
{
	return mName;
}

uint32 *CBuildingData::getRequires() const
{
	return mRequires;
}

uint32 CBuildingData::getNumRequires() const
{
	return mNumRequires;
}

uint32 CBuildingData::getTechnology(int aInd) const
{
	return mTechnologies[aInd];
}

uint32 CBuildingData::getNumTechnologies() const
{
	return mNumTechnologies;
}

uint32 CBuildingData::getHitpoints() const
{
	return mHitpoints;
}

uint32 CBuildingData::getPriceMineral() const
{
	return mPriceMineral;
}

uint32 CBuildingData::getPriceGas() const
{
	return mPriceGas;
}

uint32 CBuildingData::getEnergyProductionNight() const
{
	return mEnergyProductionNight;
}

uint32 CBuildingData::getEnergyProductionDay() const
{
	return mEnergyProductionDay;
}

uint32 CBuildingData::getEnergyConsumption() const
{
	return mEnergyConsumption;
}

enum BuildingType CBuildingData::getBuildingType() const
{
	return mBuildingType;
}

uint32 CBuildingData::getWeapon() const
{
	return mWeapon;
}

CTexture *CBuildingData::getPicture() const
{
	return mPicture;
}

int CBuildingData::getShortcut() const
{
	return mShortcut;
}