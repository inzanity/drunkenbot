#include "../include/weapondata.h"

CWeaponData::CWeaponData() :
			mRequires(NULL),
			mNumRequires(0),
			mId(0),
			mName(""),
			mModel(NULL),
			mBullet(NULL),
			mPicture(NULL),
			mPriceMineral(0),
			mPriceGas(0),
			mMass(0),
			mRange(0),
			mTime(0),
			mType(0),
			mPiercingDamage(0),
			mExplosiveDamage(0),
			mAccuracy(0)
{
}

CWeaponData::~CWeaponData()
{
	if (mRequires)
		delete [] mRequires;
}

uint32 *CWeaponData::getRequires() const
{
	return mRequires;
}

uint32 CWeaponData::getNumRequires() const
{
	return mNumRequires;
}

uint32 CWeaponData::getId() const
{
	return mId;
}

std::string CWeaponData::getName() const
{
	return mName;
}

MAnimation *CWeaponData::getModel() const
{
	return mModel;
}

MAnimation *CWeaponData::getBullet() const
{
	return mBullet;
}

CTexture *CWeaponData::getPicture() const
{
	return mPicture;
}

uint32 CWeaponData::getPriceMineral() const
{
	return mPriceMineral;
}

uint32 CWeaponData::getPriceGas() const
{
	return mPriceGas;
}

uint32 CWeaponData::getMass() const
{
	return mMass;
}

uint32 CWeaponData::getRange() const
{
	return mRange;
}

uint32 CWeaponData::getTime() const
{
	return mTime;
}

uint32 CWeaponData::getType() const
{
	return mType;
}

uint32 CWeaponData::getPiercingDamage() const
{
	return mPiercingDamage;
}

uint32 CWeaponData::getExplosiveDamage() const
{
	return mExplosiveDamage;
}

uint32 CWeaponData::getAccuracy() const
{
	return mAccuracy;
}

void CWeaponData::setRequires(std::vector<uint32> &aRequires)
{
	mNumRequires = (uint32)aRequires.size();
	if (mNumRequires) {
		mRequires = new uint32[mNumRequires];
		for (uint32 i = 0; i < mNumRequires; i++)
			mRequires[i] = aRequires[i];
	}
}

void CWeaponData::setId(uint32 aId)
{
	mId = aId;
}

void CWeaponData::setName(std::string aName)
{
	mName = aName;
}

void CWeaponData::setModel(MAnimation *aModel)
{
	mModel = aModel;
}

void CWeaponData::setBullet(MAnimation *aBullet)
{
	mBullet = aBullet;
}

void CWeaponData::setPicture(CTexture *aPicture)
{
	mPicture = aPicture;
}

void CWeaponData::setPriceMineral(uint32 aPriceMineral)
{
	mPriceMineral = aPriceMineral;
}

void CWeaponData::setPriceGas(uint32 aPriceGas)
{
	mPriceGas = aPriceGas;
}

void CWeaponData::setMass(uint32 aMass)
{
	mMass = aMass;
}

void CWeaponData::setRange(uint32 aRange)
{
	mRange = aRange;
}

void CWeaponData::setTime(uint32 aTime)
{
	mTime = aTime;
}

void CWeaponData::setType(uint32 aType)
{
	mType = aType;
}

void CWeaponData::setPiercingDamage(uint32 aPiercingDamage)
{
	mPiercingDamage = aPiercingDamage;
}

void CWeaponData::setExplosiveDamage(uint32 aExplosiveDamage)
{
	mExplosiveDamage = aExplosiveDamage;
}

void CWeaponData::setAccuracy(uint32 aAccuracy)
{
	mAccuracy = aAccuracy;
}