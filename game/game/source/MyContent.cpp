// MyContent.cpp: implementation of the MyContent class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/MyContent.h"
#include "../include/animationstorage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyContent::MyContent() :
		mBuildingData(NULL),
		mTechnologyData(NULL),
		mMechData(NULL),
		mWeaponData(NULL),
		mUpgradeData(NULL),
		mUseBuf(0),
		mDataType(TNONE)
{
}

MyContent::~MyContent()
{
}

HRESULT STDMETHODCALLTYPE MyContent::startElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName,
            /* [in] */ ISAXAttributes __RPC_FAR *pAttributes)
{
	char buf[256];
	HRESULT hr = S_OK;
	if (mDataType == TBUILDING) {
		if (!wcsnicmp(pwchLocalName, L"requires", cchLocalName))
			mUseBuf = 1;
		else if (!wcsnicmp(pwchLocalName, L"technology", cchLocalName))
			mUseBuf = 2;
	} else if (mDataType == TMECH) {
	} else if (mDataType == TTECHNOLOGY) {
		if (!wcsnicmp(pwchLocalName, L"requires", cchLocalName))
			mUseBuf = 1;
	} else if (mDataType == TWEAPON) {
		if (!wcsnicmp(pwchLocalName, L"requires", cchLocalName))
			mUseBuf = 1;
	} else if (mDataType == TUPGRADE) {
		if (!wcsnicmp(pwchLocalName, L"setValue", cchLocalName)) {
			int l;
			pAttributes->getLength(&l);
			CUpgradeData::CSetValue *temp;
			temp = new CUpgradeData::CSetValue();
			for (int i = 0; i < l; i++) {
				wchar_t *ln, *vl;
				int lnl, vll;

				pAttributes->getLocalName(i, &ln, &lnl);
				pAttributes->getValue(i, &vl, &vll);
				wcstombs(buf, vl, 256);

				if (!wcsnicmp(ln, L"variable", lnl)) {
					temp->mAttribute = new char[strlen(buf)];
					strcpy(temp->mAttribute, buf);
				} else if (!wcsnicmp(ln, L"value", lnl)) {
					temp->mValue = (float)atof(buf);
				}
			}
			mBuffer3.push_back(temp);
		}
	} else if (!wcsnicmp(pwchLocalName, L"building", cchLocalName)) {
		mBuildingData.push_back(new CBuildingData());
		mDataType = TBUILDING;
		int l;
		pAttributes->getLength(&l);
		for (int i = 0; i < l; i++) {
			wchar_t *ln, *vl;
			int lnl, vll;

			pAttributes->getLocalName(i, &ln, &lnl);
			pAttributes->getValue(i, &vl, &vll);
			wcstombs(buf, vl, 256);

			if (!wcsnicmp(ln, L"buildingType", lnl)) {
				if (!strnicmp(buf, "turret", vll))
					mBuildingData.back()->setBuildingType(TURRET);
				else if (!strnicmp(buf, "general", vll))
					mBuildingData.back()->setBuildingType(GENERAL);
				else if (!strnicmp(buf, "dockyard", vll))
					mBuildingData.back()->setBuildingType(DOCKYARD);
				else if (!strnicmp(buf, "pipe", vll))
					mBuildingData.back()->setBuildingType(PIPE);
			}
			else if (!wcsnicmp(ln, L"name", lnl))
				mBuildingData.back()->setName(buf);
			else if (!wcsnicmp(ln, L"id", lnl))
				mBuildingData.back()->setId(atoi(buf));
			else if (!wcsnicmp(ln, L"priceMineral", lnl))
				mBuildingData.back()->setPriceMineral(atoi(buf));
			else if (!wcsnicmp(ln, L"priceGas", lnl))
				mBuildingData.back()->setPriceGas(atoi(buf));
			else if (!wcsnicmp(ln, L"energyProductionNight", lnl))
				mBuildingData.back()->setEnergyProductionNight(atoi(buf));
			else if (!wcsnicmp(ln, L"energyProductionDay", lnl))
				mBuildingData.back()->setEnergyProductionDay(atoi(buf));
			else if (!wcsnicmp(ln, L"energyConsumption", lnl))
				mBuildingData.back()->setEnergyConsumption(atoi(buf));
			else if (!wcsnicmp(ln, L"hitpoints", lnl))
				mBuildingData.back()->setHitpoints(atoi(buf));
			else if (!wcsnicmp(ln, L"picture", lnl))
				mBuildingData.back()->setPicture(CAnimationStorage::ptr()->getTexture(buf));
			else if (!wcsnicmp(ln, L"animation", lnl))
				mBuildingData.back()->setAnimation(CAnimationStorage::ptr()->getAnimation(buf));
			else if (!wcsnicmp(ln, L"constructAnimation", lnl))
				mBuildingData.back()->setConstructAnimation(CAnimationStorage::ptr()->getAnimation(buf));
			else if (!wcsnicmp(ln, L"constructionTime", lnl))
				mBuildingData.back()->setConstructionTime(atoi(buf));
		}
	} else if (!wcsnicmp(pwchLocalName, L"mech", cchLocalName)) {
		mMechData.push_back(new CMechData());
		mDataType = TMECH;
		int l;
		pAttributes->getLength(&l);
		for (int i = 0; i < l; i++) {
			wchar_t *ln, *vl;
			int lnl, vll;
			pAttributes->getLocalName(i, &ln, &lnl);
			pAttributes->getValue(i, &vl, &vll);
			wcstombs(buf, vl, 256);
			if (!wcsnicmp(ln, L"priceMineral", lnl))
				mMechData.back()->setPriceMineral(atoi(buf));
			else if (!wcsnicmp(ln, L"priceGas", lnl))
				mMechData.back()->setPriceGas(atoi(buf));
			else if (!wcsnicmp(ln, L"hitpoints", lnl))
				mMechData.back()->setHitpoints(atoi(buf));
			else if (!wcsnicmp(ln, L"piercingArmor", lnl))
				mMechData.back()->setPiercingArmor(atoi(buf));
			else if (!wcsnicmp(ln, L"explosiveArmor", lnl))
				mMechData.back()->setExplosiveArmor(atoi(buf));
			else if (!wcsnicmp(ln, L"speed", lnl))
				mMechData.back()->setSpeed(atoi(buf));
			else if (!wcsnicmp(ln, L"animation", lnl))
				mMechData.back()->setAnimation(CAnimationStorage::ptr()->getAnimation(buf));
			else if (!wcsnicmp(ln, L"mass", lnl))
				mMechData.back()->setMass(atoi(buf));
		}
	} else if (!wcsnicmp(pwchLocalName, L"technology", cchLocalName)) {		
		mTechnologyData.push_back(new CTechnologyData());
		mDataType = TTECHNOLOGY;
		int l;
		pAttributes->getLength(&l);
		for (int i = 0; i < l; i++) {
			wchar_t *ln, *vl;
			int lnl, vll;
			pAttributes->getLocalName(i, &ln, &lnl);
			pAttributes->getValue(i, &vl, &vll);
			wcstombs(buf, vl, 256);

			if (!wcsnicmp(ln, L"id", lnl))
				mTechnologyData.back()->setId(atoi(buf));
			else if (!wcsnicmp(ln, L"name", lnl))
				mTechnologyData.back()->setName(buf);
			else if (!wcsnicmp(ln, L"priceMineral", lnl))
				mTechnologyData.back()->setPriceMineral(atoi(buf));
			else if (!wcsnicmp(ln, L"priceGas", lnl))
				mTechnologyData.back()->setPriceGas(atoi(buf));
			else if (!wcsnicmp(ln, L"developTime", lnl))
				mTechnologyData.back()->setDevelopTime(atoi(buf));
			else if (!wcsnicmp(ln, L"picture", lnl))
				mTechnologyData.back()->setPicture(CAnimationStorage::ptr()->getTexture(buf));
		}
	} else if (!wcsnicmp(pwchLocalName, L"weapon", cchLocalName)) {		
		mWeaponData.push_back(new CWeaponData());
		mDataType = TWEAPON;
		int l;
		pAttributes->getLength(&l);
		for (int i = 0; i < l; i++) {
			wchar_t *ln, *vl;
			int lnl, vll;
			pAttributes->getLocalName(i, &ln, &lnl);
			pAttributes->getValue(i, &vl, &vll);
			wcstombs(buf, vl, 256);

			if (!wcsnicmp(ln, L"id", lnl))
				mWeaponData.back()->setId(atoi(buf));
			else if (!wcsnicmp(ln, L"name", lnl))
				mWeaponData.back()->setName(buf);
			else if (!wcsnicmp(ln, L"model", lnl))
				mWeaponData.back()->setModel(CAnimationStorage::ptr()->getAnimation(buf));
			else if (!wcsnicmp(ln, L"bullet", lnl))
				mWeaponData.back()->setBullet(CAnimationStorage::ptr()->getAnimation(buf));
			else if (!wcsnicmp(ln, L"picture", lnl))
				mWeaponData.back()->setPicture(CAnimationStorage::ptr()->getTexture(buf));
			else if (!wcsnicmp(ln, L"priceMineral", lnl))
				mWeaponData.back()->setPriceMineral(atoi(buf));
			else if (!wcsnicmp(ln, L"priceGas", lnl))
				mWeaponData.back()->setPriceGas(atoi(buf));
			else if (!wcsnicmp(ln, L"mass", lnl))
				mWeaponData.back()->setMass(atoi(buf));
			else if (!wcsnicmp(ln, L"range", lnl))
				mWeaponData.back()->setRange(atoi(buf));
			else if (!wcsnicmp(ln, L"time", lnl))
				mWeaponData.back()->setTime(atoi(buf));
			else if (!wcsnicmp(ln, L"type", lnl))
				mWeaponData.back()->setType(atoi(buf));
			else if (!wcsnicmp(ln, L"piercingDamage", lnl))
				mWeaponData.back()->setPiercingDamage(atoi(buf));
			else if (!wcsnicmp(ln, L"explosiveDamage", lnl))
				mWeaponData.back()->setExplosiveDamage(atoi(buf));
			else if (!wcsnicmp(ln, L"accuracy", lnl))
				mWeaponData.back()->setAccuracy(atoi(buf));
		}
	}
    return hr;
}
        
       
HRESULT STDMETHODCALLTYPE MyContent::endElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName)
{
	if (mDataType == TBUILDING && !wcsnicmp(pwchLocalName, L"building", cchLocalName)) {
		mBuildingData.back()->setRequires(mBuffer);
		mBuildingData.back()->setTechnologies(mBuffer2);
		mBuffer.clear();
		mBuffer2.clear();
		mDataType = TNONE;
	} else if ((mDataType == TBUILDING || mDataType == TTECHNOLOGY || mDataType == TWEAPON)
				&& mUseBuf == 1 && !wcsnicmp(pwchLocalName, L"requires", cchLocalName)) {
		mUseBuf = 0;
	} else if (mDataType == TBUILDING && mUseBuf == 2 && !wcsnicmp(pwchLocalName, L"technology", cchLocalName)) {
		mUseBuf = 0;
	} else if (mDataType == TMECH && !wcsnicmp(pwchLocalName, L"mech", cchLocalName)) {
		mDataType = TNONE;
	} else if (mDataType == TTECHNOLOGY && !wcsnicmp(pwchLocalName, L"technology", cchLocalName)) {
		mTechnologyData.back()->setRequires(mBuffer);
		mBuffer.clear();
		mDataType = TNONE;
	} else if (mDataType == TWEAPON && !wcsnicmp(pwchLocalName, L"weapon", cchLocalName)) {
		mWeaponData.back()->setRequires(mBuffer);
		mBuffer.clear();
		mDataType = TNONE;
	} else if (mDataType == TUPGRADE && !wcsnicmp(pwchLocalName, L"upgrade", cchLocalName)) {
		mUpgradeData.back()->setSetValues(mBuffer3);
		while (mBuffer3.size())
		{
			delete [] mBuffer3.back()->mAttribute;
			delete mBuffer3.back();
			mBuffer3.pop_back();
		}
		mDataType = TNONE;
	}
    return S_OK;
}

HRESULT STDMETHODCALLTYPE MyContent::characters( 
            /* [in] */ wchar_t __RPC_FAR *pwchChars,
			/* [in] */ int cchChars)
{
	if ((mDataType == TBUILDING || mDataType == TTECHNOLOGY || mDataType == TWEAPON) && mUseBuf) {
		char buf[256];
		int temp;
		wcstombs(buf, pwchChars, 256);
		temp = atoi(buf);
		if (temp && mUseBuf == 1)
			mBuffer.push_back(temp);
		else if (temp && mUseBuf == 2 && mDataType == TBUILDING)
			mBuffer2.push_back(temp);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MyContent::startDocument()
{
    return S_OK;
}

std::vector<CBuildingData *> &MyContent::getBuildingData()
{
	return mBuildingData;
}

std::vector<CMechData *> &MyContent::getMechData()
{
	return mMechData;
}

std::vector<CTechnologyData *> &MyContent::getTechnologyData()
{
	return mTechnologyData;
}

std::vector<CWeaponData *> &MyContent::getWeaponData()
{
	return mWeaponData;
}

std::vector<CUpgradeData *> &MyContent::getUpgradeData()
{
	return mUpgradeData;
}