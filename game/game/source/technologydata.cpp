#include "../include/technologydata.h"

CTechnologyData::CTechnologyData() :
		mRequires(0),
		mNumRequires(0),
		mId(0),
		mName(""),
		mPriceMineral(0),
		mPriceGas(0),
		mDevelopTime(0),
		mPicture(NULL)
{
}

CTechnologyData::~CTechnologyData()
{
	if (mRequires)
		delete [] mRequires;
}

uint32 *CTechnologyData::getRequires() const
{
	return mRequires;
}

uint32 CTechnologyData::getId() const
{
	return mId;
}

std::string CTechnologyData::getName() const
{
	return mName;
}

uint32 CTechnologyData::getPriceMineral() const
{
	return mPriceMineral;
}

uint32 CTechnologyData::getPriceGas() const
{
	return mPriceGas;
}

uint32 CTechnologyData::getDevelopTime() const
{
	return mDevelopTime;
}

CTexture *CTechnologyData::getPicture() const
{
	return mPicture;
}

void CTechnologyData::setRequires(std::vector<uint32> &aRequires)
{
	mNumRequires = (uint32)aRequires.size();
	if (mNumRequires) {
		mRequires = new uint32[mNumRequires];
		for (uint32 i = 0; i < mNumRequires; i++)
			mRequires[i] = aRequires[i];
	}
}

void CTechnologyData::setId(uint32 aId)
{
	mId = aId;
}

void CTechnologyData::setName(std::string aName)
{
	mName = aName;
}

void CTechnologyData::setPriceMineral(uint32 aPriceMineral)
{
	mPriceMineral = aPriceMineral;
}

void CTechnologyData::setPriceGas(uint32 aPriceGas)
{
	mPriceGas = aPriceGas;
}

void CTechnologyData::setDevelopTime(uint32 aDevelopTime)
{
	mDevelopTime = aDevelopTime;
}

void CTechnologyData::setPicture(CTexture *aPicture)
{
	mPicture = aPicture;
}