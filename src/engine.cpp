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
	std::list<CBulletInfo *>::iterator bulIter;
	int i;
	for (i = 0; i < mBotNum; i++)
		if (!mBots[i]->update())
			mBots[i]->spawn((const char **)mTilemap, mMapWidth, mMapHeight, (const CGameObj **)mBots, (const CWeapon *)mWeaponTypes[0]);
	while (mWeaponList.size() < (unsigned int)mWeaponNum - 1)
	{
		float x, y;
		char tile;
		do {
			x = 1 + rand() % (mMapWidth-2);
			y = 1 + rand() % (mMapHeight-2);
			tile = mTilemap[int(y - .5f)][int(x + .5f)] &
				mTilemap[int(y - .5f)][int(x - .5f)] &
				mTilemap[int(y + .5f)][int(x + .5f)] &
				mTilemap[int(y + .5f)][int(x - .5f)];
		} while ((tile & KTileTypeMask) != CTilemap::ETileEmpty);
		mWeaponList.push_back(new CWeaponInfo(mWeaponTypes[rand() % (mWeaponNum - 1) + 1], x, y));
	}

	for (bulIter = mBulletList.begin(); bulIter != mBulletList.end(); bulIter++)
		if (!(*bulIter)->update())
		{
			delete (*bulIter);
			mBulletList.erase(bulIter--);
		}
	for (i = 0; i < mBotNum; i++)
		mBots[i]->think((const char **)mTilemap, mMapWidth, mMapHeight, (CVisibleBotInfo **)mBots,
						&mBulletList, &mWeaponList, &mVoiceList);
	mVoiceList.clear();
	for (i = 0; i < mBotNum; i++)
		mBots[i]->performActions(&mBulletList, &mVoiceList, &mWeaponList, (const CWeapon *)mWeaponTypes[0]);
	for (bulIter = mBulletList.begin(); bulIter != mBulletList.end(); bulIter++)
		(*bulIter)->chkCollision((const char **)mTilemap, (CBotInfo **)mBots, false);
	for (i = 0; i < mBotNum; i++)
		mBots[i]->chkCollision((const char **)mTilemap, (CBotInfo **)&mBots[i + 1], true);
	return true;
}

void CGameEngine::draw(float aTimeInterval, int aBotIndex)
{
	std::list<CBulletInfo *>::iterator bulIter;
	std::list<CWeaponInfo *>::iterator weaIter;
	if (!mGfxEngine)
		return;
	mGfxEngine->setActiveBot(aBotIndex >= 0 && aBotIndex < mBotNum ? mBots[aBotIndex] : NULL);
	mGfxEngine->drawTilemap(mTilemap, mMapWidth, mMapHeight);
	for (bulIter = mBulletList.begin(); bulIter != mBulletList.end(); bulIter++)
	{
		(*bulIter)->move(aTimeInterval);
		mGfxEngine->drawGameObj(*bulIter);
	}
	for (weaIter = mWeaponList.begin(); weaIter != mWeaponList.end(); weaIter++)
		mGfxEngine->drawGameObj(*weaIter);
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
	char **results = new char *[mBotNum + 1];
	for (int i = 0; i < mBotNum; i++)
	{
		char *name = mBots[i]->name();
		results[i] = new char[strlen(name) + 6];
		sprintf(results[i], "%-5d%s", mBots[i]->fragNum(), name);
	}
	results[mBotNum] = NULL;
	return results;

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

	mTilemap = new char*[mMapHeight + 1];

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
		mBots[i]->spawn((const char **)mTilemap, mMapWidth, mMapHeight, (const CGameObj**)mBots, mWeaponTypes[0]);
}

void CGameEngine::setFragLimit(int aFragLimit)
{
	mFragLimit = aFragLimit;
}

