// #include "StdAfx.h"
#include "../include/buildingdata.h"
#include <iostream>

using std::cout;
using std::endl;

CBuildingData::CBuildingData(MAnimation *aAnimation,
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
							 CTexture *aPicture) : 
								    mAnimation(aAnimation),
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
									mWeapon(aWeapon)
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
									mWeapon(0)
{
}

CBuildingData::~CBuildingData()
{
	// AnimationStorage does the deleting, we don't need to.
}

void CBuildingData::setId(int aId)
{
	mId = aId;
}
void CBuildingData::setAnimation(MAnimation *aAnimation)
{
	mAnimation = aAnimation;
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

/*
void CBuildingData::dumpData() const
{
	if (mId) cout << "Building " << mName << "(" << mId << ")" << endl;
	if (mAnimation) cout << "\tAnimation: " << mAnimation << endl;
	if (mConstructAnimation) cout << "\tConstructAnimation: " << mConstructAnimation << endl;
	if (mConstructionTime) cout << "\tConstructionTime: " << mConstructionTime << endl;
	if (mName.length()) cout << "\tName: " << mName << endl;
	if (mRequires) cout << "\tRequires: " << mRequires << endl;
	if (mNumRequires) cout << "\tNumRequires: " << mNumRequires << endl;
	if (mNumTechnologies) {
		cout << "\tTechnologies:" << endl;
		for (int i = 0; i < mNumTechnologies; i++)
			cout << "\t\t" << mTechnologies[i] << endl;
	}
	if (mHitpoints) cout << "\tHitpoints: " << mHitpoints << endl;
	if (mPriceMineral) cout << "\tPriceMineral: " << mPriceMineral << endl;
	if (mPriceGas) cout << "\tPriceGas: " << mPriceGas << endl;
	if (mEnergyProductionNight) cout << "\tEnergyProductionNight: " << mEnergyProductionNight << endl;
	if (mEnergyProductionDay) cout << "\tEnergyProductionDay: " << mEnergyProductionDay << endl;
	if (mEnergyConsumption) cout << "\tEnergyConsumption: " << mEnergyConsumption << endl;
	if (mBuildingType) cout << "\tBuildingType: " << mBuildingType << endl;
	if (mWeapon) cout << "\tWeapon: " << mWeapon << endl;
}*/

uint32 CBuildingData::getId() const
{
	return mId;
}

MAnimation *CBuildingData::getAnimation() const
{
	return mAnimation;
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

uint32 *CBuildingData::getTechnologies() const
{
	return mTechnologies;
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

