#include "../inc/engine.h"

CGameEngine::CGameEngine(istream *aWeapons, istream *aMap, istream *aTeamInfo) : 
						 mGfxEngine(NULL), mWeaponTypes(NULL), mBots(NULL), mTilemap(NULL),
						 mBotNum(0), mWeaponNum(0), mMapWidth(0), mMapHeight(0),
						 mFragLimit(0), mTeamNum(0), mTeamNames(NULL), mFriendlyFire(true)
{
	int i;

	*aWeapons >> mWeaponNum;
	mWeaponTypes = new CWeapon*[mWeaponNum];
	for (i = 0; i < mWeaponNum; i++)
		mWeaponTypes[i] = new CWeapon(aWeapons);

	if (aTeamInfo)
	{
		// ...
	}

	restart(aMap);
}

void CGameEngine::setGraphicsEngine(IGraphicsEngine *aGraphicsEngine)
{
	mGfxEngine = aGraphicsEngine;
}

bool CGameEngine::loop()
{
	if (mGfxEngine)
		mGfxEngine->drawTilemap(mTilemap, mMapWidth, mMapHeight);
	return true;
}

char **CGameEngine::getResults(bool aTeamResults) const
{
	if (aTeamResults)
	{
		return NULL;
	}
	return NULL;

}

void CGameEngine::restart(istream *aMap)
{
	int ltSize, iTemp;
	char *lookup, cTemp;
	char *sTemp;
	int i;

	if (mTilemap)
	{
		for (i = 0; i < mMapHeight; i++)
			delete mTilemap[i];
		delete [] mTilemap;
		mTilemap = NULL;
	}

	*aMap >> mMapHeight >> mMapWidth >> ltSize;

	lookup = new char[256];
	
	for (i = 0; i < ltSize; i += 1)
	{
		*aMap >> iTemp >> cTemp;
		lookup[cTemp] = iTemp;
	}

	mTilemap = new char*[mMapHeight];

	sTemp = new char[mMapWidth + 1];

	for (i = 0; i < mMapHeight; i += 1)
	{
		mTilemap[i] = new char[mMapWidth];
		*aMap >> sTemp;
		for (int j = 0; j < mMapWidth; j += 1)
			mTilemap[i][j] = lookup[sTemp[j]];
		mTilemap[mMapWidth] = '\0';
	}

	delete [] sTemp;
	delete [] lookup;
}

void CGameEngine::setFragLimit(int aFragLimit)
{
	mFragLimit = aFragLimit;
}

