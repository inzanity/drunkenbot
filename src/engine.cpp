#ifndef WIN32
# include <cstdlib>
# include <cstring>
# include <dirent.h>
#endif
#include "../inc/engine.h"

#ifndef WIN32
int fileselector(const struct dirent *file)
{
	if (strncmp(".so", file->d_name + strlen(file->d_name) - 3, 3))
		return 0;
	return 1;
}
#endif

CGameEngine::CGameEngine(istream *aWeapons, istream *aMap, istream *aTeamInfo) : 
						 mGfxEngine(NULL), mWeaponTypes(NULL), mBots(NULL), mTilemap(NULL),
						 mBotNum(0), mWeaponNum(0), mMapWidth(0), mMapHeight(0),
						 mFragLimit(0), mTeamNum(0), mTeamNames(NULL), mFriendlyFire(true)
{
	int i;
	int teamTemp;

	*aWeapons >> mWeaponNum;
	mWeaponTypes = new CWeapon*[mWeaponNum];
	for (i = 0; i < mWeaponNum; i++)
		mWeaponTypes[i] = new CWeapon(aWeapons);
	char *fileName = new char[64];

	if (aTeamInfo)
	{
		*aTeamInfo >> mBotNum;
		mBots = new CBot *[mBotNum];
		
		for (i = 0; i < mBotNum; i++)
		{
			*aTeamInfo >> teamTemp >> fileName;
			mBots[i] = new CBot(fileName, teamTemp);
		}
	}
	else
	{
#ifdef WIN32
		WIN32_FIND_DATA findFileData;
		HANDLE handle = FindFirstFile("bots/*.dll", &findFileData);
		if (handle == INVALID_HANDLE_VALUE)
			return;

		for (mBotNum = 1; FindNextFile(handle, &findFileData); mBotNum++);
		mBots = new CBot *[mBotNum + 1];

		handle = FindFirstFile("bots/*.dll", &findFileData);
		for (i = 0; i < mBotNum; i++)
		{
			sprintf(fileName, "bots/%s", findFileData.cFileName);
			mBots[i] = new CBot(fileName, i);
			FindNextFile(handle, &findFileData);
		}
#else
		struct dirent **filelist;
		int i;
		
		mBotNum = scandir("bots/", &filelist, fileselector, NULL);

		mBots = new CBot *[mBotNum + 1];

		for (i = 0; i < mBotNum; i++)
		{
			snprintf(fileName, 63, "bots/%s", filelist[i]->d_name);
			mBots[i] = new CBot(fileName, i);
			free(filelist[i]);
		}
		free(filelist);
#endif
		mBots[mBotNum] = NULL;
	}
	delete [] fileName;

	restart(aMap);
}

void CGameEngine::setGraphicsEngine(CGraphicsEngine *aGraphicsEngine)
{
	mGfxEngine = aGraphicsEngine;
}

bool CGameEngine::loop()
{
	int i;
	for (i = 0; i < mBotNum; i++)
		if (!mBots[i]->update())
			mBots[i]->spawn((const char **)mTilemap, mMapWidth, mMapHeight, (const CGameObj **)mBots);
	for (i = 0; i < mBotNum; i++)
		mBots[i]->think((const char **)mTilemap, mMapWidth, mMapHeight, (CVisibleBotInfo **)mBots, mBotNum,
						&mBulletList, &mWeaponList, &mVoiceList);
	for (i = 0; i < mBotNum; i++)
		mBots[i]->performActions(&mBulletList, &mVoiceList);
	for (i = 0; i < mBotNum; i++)
		mBots[i]->chkCollision((const char **)mTilemap, (CBotInfo **)&mBots[i + 1], true);
	return true;
}

void CGameEngine::draw(float aTimeInterval, int aBotIndex)
{
	if (!mGfxEngine)
		return;
	mGfxEngine->setActiveBot(aBotIndex >= 0 && aBotIndex < mBotNum ? mBots[aBotIndex] : NULL);
	mGfxEngine->drawTilemap(mTilemap, mMapWidth, mMapHeight);
	for (int i = 0; i < mBotNum; i++)
	{
		mBots[i]->move(aTimeInterval);
		mGfxEngine->drawGameObj(mBots[i]);
	}
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
	}

	delete [] sTemp;
	delete [] lookup;

	for (i = 0; i < mBotNum; i++)
		mBots[i]->spawn((const char **)mTilemap, mMapWidth, mMapHeight, (const CGameObj**)mBots);
}

void CGameEngine::setFragLimit(int aFragLimit)
{
	mFragLimit = aFragLimit;
}

