// #include "StdAfx.h"
#include "../include/buildingdata.h"
#include <iostream>

using std::cout;
using std::endl;

CBuildingData::CBuildingData(MAnimation *aAnimation,
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

void CBuildingData::setRequires(const std::vector<int> &aRequires)
{
	mNumRequires = aRequires.size();
	if (mNumRequires)
	{
		mRequires = new int[mNumRequires];
		for (int i = 0; i < mNumRequires; i++)
			mRequires[i] = aRequires[i];
	}
}
void CBuildingData::setTechnologies(const std::vector<int> &aTechnologies)
{
	mNumTechnologies = aTechnologies.size();
	if (mNumTechnologies) {
		mTechnologies = new int[mNumTechnologies];
		for (int i = 0; i < mNumTechnologies; i++)
			mTechnologies[i] = aTechnologies[i];
	}
}
void CBuildingData::setHitpoints(int aHitpoints)
{
	mHitpoints = aHitpoints;
}
void CBuildingData::setPriceMineral(int aPriceMineral)
{
	mPriceMineral = aPriceMineral;
}
void CBuildingData::setPriceGas(int aPriceGas)
{
	mPriceGas = aPriceGas;
}
void CBuildingData::setEnergyProductionNight(int aEnergyProductionNight)
{
	mEnergyProductionNight = aEnergyProductionNight;
}
void CBuildingData::setEnergyProductionDay(int aEnergyProductionDay)
{
	mEnergyProductionDay = aEnergyProductionDay;
}
void CBuildingData::setEnergyConsumption(int aEnergyConsumption)
{
	mEnergyConsumption = aEnergyConsumption;
}
void CBuildingData::setBuildingType(enum BuildingType aBuildingType)
{
	mBuildingType = aBuildingType;
}
void CBuildingData::setWeapon(int aWeapon)
{
	mWeapon = aWeapon;
}
void CBuildingData::setPicture(CTexture *aPicture)
{
	mPicture = aPicture;
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

int CBuildingData::getPriceGas() const
{
	return mPriceGas;
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