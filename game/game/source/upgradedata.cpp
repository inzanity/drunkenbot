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

uint32 CUpgradeData::getId() const
{
	return mId;
}

std::string CUpgradeData::getName() const
{
	return mName;
}

uint32 CUpgradeData::getNumSetValues() const
{
	return mNumSetValues;
}

CUpgradeData::CSetValue *CUpgradeData::getSetValues() const
{
	return mSetValues;
}

uint32 CUpgradeData::getPriceMineral() const
{
	return mPriceMineral;
}

uint32 CUpgradeData::getPriceGas() const
{
	return mPriceGas;
}

CTexture *CUpgradeData::getPicture() const
{
	return mPicture;
}

int CUpgradeData::getShortcut() const
{
	return mShortcut;
}

void CUpgradeData::setId(uint32 aId)
{
	mId = aId;
}

void CUpgradeData::setName(const char *aName)
{
	mName = aName;
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
void CUpgradeData::setShortcut(int aShortcut)
{
	mShortcut = aShortcut;
}