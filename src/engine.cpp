#include "../inc/engine.h"

CGameEngine::CGameEngine(istream *aWeapons, istream *aMap, istream *aTeamInfo) : 
						 mGfxEngine(NULL), mWeaponTypes(NULL), mBots(NULL), mTilemap(NULL),
						 mBotNum(0), mWeaponNum(0), mMapWidth(0), mMapHeight(0),
						 mFragLimit(0), mTeamNum(0), mTeamNames(NULL), mFriendlyFire(true)
{
	*aWeapons >> mWeaponNum;
	// ...

	restart(aMap);

	if (aTeamInfo)
	{
		// ...
	}
}

void CGameEngine::setGraphicsEngine(IGraphicsEngine *aGraphicsEngine)
{
	mGfxEngine = aGraphicsEngine;
}

bool CGameEngine::loop()
{
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
	int mapWidth, mapHeight;
	*aMap >> mapWidth >> mapHeight;
	// ...
}

void CGameEngine::setFragLimit(int aFragLimit)
{
	mFragLimit = aFragLimit;
}

