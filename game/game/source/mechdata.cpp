#include "../include/mechdata.h"

CMechData::CMechData() :
			mPriceMineral(0),
			mPriceGas(0),
			mHitpoints(0),
			mPiercingArmor(0),
			mExplosiveArmor(0),
			mSpeed(0),
			mAnimation(0),
			mMass(0)
{
}

CMechData::~CMechData()
{
}

void CMechData::setPriceMineral(uint32 aPriceMineral)
{
	mPriceMineral = aPriceMineral;
}

void CMechData::setPriceGas(uint32 aPriceGas)
{
	mPriceGas = aPriceGas;
}

void CMechData::setHitpoints(uint32 aHitpoints)
{
	mHitpoints = aHitpoints;
}

void CMechData::setPiercingArmor(uint32 aPiercingArmor)
{
	mPiercingArmor = aPiercingArmor;
}

void CMechData::setExplosiveArmor(uint32 aExplosiveArmor)
{
	mExplosiveArmor = aExplosiveArmor;
}

void CMechData::setSpeed(uint32 aSpeed)
{
	mSpeed = aSpeed;
}

void CMechData::setAnimation(MAnimation *aAnimation)
{
	mAnimation = aAnimation;
}

void CMechData::setMass(uint32 aMass)
{
	mMass = aMass;
}

uint32 CMechData::getPriceMineral() const
{
	return mPriceMineral;
}

uint32 CMechData::getPriceGas() const
{
	return mPriceGas;
}

uint32 CMechData::getHitpoints() const
{
	return mHitpoints;
}

uint32 CMechData::getPiercingArmor() const
{
	return mPiercingArmor;
}

uint32 CMechData::getExplosiveArmor() const
{
	return mExplosiveArmor;
}

uint32 CMechData::getSpeed() const
{
	return mSpeed;
}

MAnimation *CMechData::getAnimation() const
{
	return mAnimation;
}

uint32 CMechData::getMass() const
{
	return mMass;
}