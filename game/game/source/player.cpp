#include "../include/player.h"
#include "../include/animationStorage.h"

#include "../include/MyContent.h"
#include "../include/SAXErrorHandlerImpl.h"

CPlayer::CPlayer(const char *aDataFile)
{
	CoInitialize(NULL); 
	ISAXXMLReader* pRdr = NULL;

	HRESULT hr = CoCreateInstance(
								__uuidof(SAXXMLReader), 
								NULL, 
								CLSCTX_ALL, 
								__uuidof(ISAXXMLReader), 
								(void **)&pRdr);

	if(!FAILED(hr)) 
	{
		MyContent * pMc = new MyContent();

		hr = pRdr->putContentHandler(pMc);

		// No sense to do so in this example, just an illustration how to set other handlers
		//===================================================================================
		 SAXErrorHandlerImpl * pEc = new SAXErrorHandlerImpl();
		 hr = pRdr->putErrorHandler(pEc);
		// SAXDTDHandlerImpl * pDc = new SAXDTDHandlerImpl();
		// hr = pRdr->putDTDHandler(pDc);

		 static wchar_t URL[1000];
		 mbstowcs( URL, aDataFile, 999 );
		hr = pRdr->parseURL(URL);
		pRdr->Release();

		mNumBuildings = pMc->getBuildingData().size();
		mNumUpgrades = pMc->getUpgradeData().size();
		mNumTechs = pMc->getTechnologyData().size();
		mNumWeapons = pMc->getWeaponData().size();

		mBuildings = new CBuildingData *[mNumBuildings];
		mUpgrades = new CUpgradeData *[mNumUpgrades];
		mTechs = new CTechnologyData *[mNumTechs];
		mWeapons = new CWeaponData *[mNumWeapons];

		int i;
		for (i = 0; i < mNumBuildings; i++)
			mBuildings[i] = pMc->getBuildingData()[i];
		for (i = 0; i < mNumUpgrades; i++)
			mUpgrades[i] = pMc->getUpgradeData()[i];
		for (i = 0; i < mNumTechs; i++)
			mTechs[i] = pMc->getTechnologyData()[i];
		for (i = 0; i < mNumWeapons; i++)
			mWeapons[i] = pMc->getWeaponData()[i];
	}
	CoUninitialize();
}

CPlayer::CPlayer(CBuildingData **aBuildings, int aNumBuildings) :
	mBuildings(aBuildings), mNumBuildings(aNumBuildings), mMinerals(0)
{
	mNumBuildings = 2;
	mBuildings = new CBuildingData *[2];
	mBuildings[0] = new CBuildingData(
		CAnimationStorage::ptr()->getAnimation("data/turret.x"),
		CAnimationStorage::ptr()->getAnimation("data/turretTop.x"),
		CAnimationStorage::ptr()->getAnimation("data/turret.x"),
		10000, "Turret", NULL, 0, 100, 10, 69, 0, 0, 1, TURRET, 0,
		CAnimationStorage::ptr()->getTexture("data/images/turret_icon.bmp"), 0);
	mBuildings[1] = new CBuildingData(
		CAnimationStorage::ptr()->getAnimation("data/silo.x"),
		NULL,
		CAnimationStorage::ptr()->getAnimation("data/silo.x"),
		10000, "Silo", NULL, 0, 100, 10, 69, 0, 0, 1, GENERAL, 0,
		CAnimationStorage::ptr()->getTexture("data/images/silo_icon.bmp"), 0);

	mNumTechs = 1;
	mTechs = new CTechnologyData *[mNumTechs];
	mTechs[0] = new CTechnologyData();
	mTechs[0]->setPicture(CAnimationStorage::ptr()->getTexture("data/images/tech1_icon.bmp"));

	mNumWeapons = 1;
	mWeapons = new CWeaponData *[mNumWeapons];
	mWeapons[0] = new CWeaponData();
	mWeapons[0]->setPicture(CAnimationStorage::ptr()->getTexture("data/images/weapon1_icon.bmp"));

	mNumUpgrades = 1;
	mUpgrades = new CUpgradeData *[mNumUpgrades];
	mUpgrades[0] = new CUpgradeData();
	mUpgrades[0]->setPicture(CAnimationStorage::ptr()->getTexture("data/images/upgrade1_icon.bmp"));

	std::vector<uint32> techs;
	techs.push_back(0);
	mBuildings[1]->setTechnologies(techs);
}

CBuildingData *CPlayer::getBuildingData(int aIndex) const
{
	if (aIndex >= 0 && aIndex < mNumBuildings)
		return mBuildings[aIndex];
	return NULL;
}

CTechnologyData *CPlayer::getTechnologyData(int aIndex) const
{
	if (aIndex >= 0 && aIndex < mNumTechs)
		return mTechs[aIndex];
	return NULL;
}

CWeaponData *CPlayer::getWeaponData(int aIndex) const
{
	if (aIndex >= 0 && aIndex < mNumWeapons)
		return mWeapons[aIndex];
	return NULL;
}

CUpgradeData *CPlayer::getUpgradeData(int aIndex) const
{
	if (aIndex >= 0 && aIndex < mNumUpgrades)
		return mUpgrades[aIndex];
	return NULL;
}

int CPlayer::numBuildings() const
{
	return mNumBuildings;
}

int CPlayer::numTechnologies() const
{
	return mNumTechs;
}

int CPlayer::numWeapons() const
{
	return mNumWeapons;
}

int CPlayer::numUpgrades() const
{
	return mNumUpgrades;
}

int CPlayer::minerals() const
{
	return mMinerals;
}

void CPlayer::setMinerals(int aMinerals)
{
	mMinerals = aMinerals;
}
