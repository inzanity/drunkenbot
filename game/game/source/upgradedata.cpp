#include "../include/upgradedata.h"

CUpgradeData::CUpgradeData() :
	mSetValues(NULL),
	mNumSetValues(0),
	mPriceGas(0),
	mPriceMineral(0)
{
}

CUpgradeData::~CUpgradeData()
{
	for (uint32 i = 0; i < mNumSetValues; i++)
		delete [] mSetValues[i].mAttribute;
	delete [] mSetValues;
}

uint32 CUpgradeData::getNumSetValues()
{
	return mNumSetValues;
}

CUpgradeData::CSetValue *CUpgradeData::getSetValues()
{
	return mSetValues;
}

uint32 CUpgradeData::getPriceMineral()
{
	return mPriceMineral;
}

uint32 CUpgradeData::getPriceGas()
{
	return mPriceGas;
}

CTexture *CUpgradeData::getPicture()
{
	return mPicture;
}

void CUpgradeData::setSetValues(std::vector<CUpgradeData::CSetValue *> &aSetValues)
{
	mNumSetValues = (uint32)aSetValues.size();
	if (mSetValues)
		delete [] mSetValues;
	mSetValues = new CSetValue[mNumSetValues];
	for (uint32 i = 0; i < mNumSetValues; i++)
	{
		mSetValues[i].mAttribute = new char[strlen(aSetValues[i]->mAttribute) + 1];
		strcpy(mSetValues[i].mAttribute, aSetValues[i]->mAttribute);
		mSetValues[i].mValue = aSetValues[i]->mValue;
	}
}

void CUpgradeData::setPriceMineral(uint32 aPriceMineral)
{
	mPriceMineral = aPriceMineral;
}

void CUpgradeData::setPriceGas(uint32 aPriceGas)
{
	mPriceGas = aPriceGas;
}

void CUpgradeData::setPicture(CTexture *aPicture)
{
	mPicture = aPicture;
}