#include <cstring>
#include <cstdlib>
#include "../inc/bot.h"

#include <cmath>

CBot::CBot(const char *aDllName, int aTeamNumber) : CBotInfo(aTeamNumber << 8), mBotAI(NULL), mDllName(NULL), mDllHandle(NULL), mFrags(0)
{
	mDllName = new char[strlen(aDllName) + 1];
	strcpy(mDllName, aDllName);
}

void CBot::spawn(const char ** aTilemap, int aWidth, int aHeight, const CGameObj **aGameObjects)
{
	do {
		mPos.mX = 1.f + rand() % (aWidth-2);
		mPos.mY = 1.f + rand() % (aHeight-2);
	} while ((aTilemap[int(mPos.mY)][int((mPos.mX))] & 3) != 3);
	loadAI();
	mRadius = 1.f;
}

void CBot::think(const char **aTilemap, CVisibleBotInfo **aBots, int aBotNum, list<CBulletInfo *> *aBulletList,
				 list<CWeaponInfo *> *aWeaponList, list<TVector> *aVoices)
{
	std::list<CBulletInfo *>::iterator bulIter;
	std::list<CWeaponInfo *>::iterator weaIter;
	std::list<TVector>::iterator voiIter;
	if (!mBotAI)
		return;
	for (int i = 0; i < aBotNum; i++)
		if (aBots[i]->type() != mType && !(aTilemap[int(aBots[i]->yPos())][int(aBots[i]->yPos())] & 0x80))
			mBotAI->mBots.push_front(new CVisibleBotInfo(aBots[i], mPos.mX, mPos.mY, mType));
	for (weaIter = aWeaponList->begin(); weaIter != aWeaponList->end(); weaIter++)
		if (!(aTilemap[int((*weaIter)->yPos())][int((*weaIter)->xPos())] & 0x80))
			mBotAI->mWeapons.push_front(new CVisibleWeaponInfo(*weaIter, mPos.mX, mPos.mY));

	for (bulIter = aBulletList->begin(); bulIter != aBulletList->end(); bulIter++)
		if (!(aTilemap[int((*bulIter)->yPos())][int((*bulIter)->xPos())] & 0x80))
			mBotAI->mBullets.push_front(new CVisibleBulletInfo(*bulIter, mPos.mX, mPos.mY));

	for (voiIter = aVoices->begin(); voiIter != aVoices->end(); voiIter++)
		if (!(aTilemap[int((*voiIter).mY)][int((*voiIter).mX)] & 0x80))
			mBotAI->mSourcesOfNoise.push_front(atan2((*voiIter).mY - mBotAI->mData.yPos(), (*voiIter).mX - mBotAI->mData.xPos()));

	mBotAI->think();

	for (; !mBotAI->mBots.empty(); mBotAI->mBots.pop_back())
		delete mBotAI->mBots.back();
	for (; !mBotAI->mBullets.empty(); mBotAI->mBullets.pop_back())
		delete mBotAI->mBullets.back();
	for (; !mBotAI->mWeapons.empty(); mBotAI->mWeapons.pop_back())
		delete mBotAI->mWeapons.back();
	mBotAI->mSourcesOfNoise.clear();
}

void CBot::performActions(list<CBulletInfo *> * aBulletList, list<TVector> * aVoices)
{
	if (!mBotAI)
		return;
	if (mActionDelay)
		if (--mActionDelay == 0)
		{
			if (mBotAction == EActionBunker)
				mBunkered = !mBunkered;
			mBotAction = EActionNone;
		}

	if (!mBotAI->checkAction() ||
		(mActionDelay && mBotAI->action() > EActionMove) ||
		(mBunkered && (mBotAI->action() & EActionMove)))
		mBotAI->resetAction();
	if (mBotAI->action() & EActionTurn)
	{
		if (mBotAI->turningDir() == CBotAI::ETurnLeft)
			mOrientation += .1f * PI;
		else if (mBotAI->turningDir() == CBotAI::ETurnRight)
			mOrientation += 1.9f * PI;
		if (mOrientation >= 2 * PI)
			mOrientation -= 2 * PI;
	}
	if (mBotAI->action() & EActionMove)
	{
		switch (mBotAI->movingDir())
		{
		case CBotAI::EMoveForward:
			mMovingDirection = mOrientation;
			mVelocity = .25f;
			break;
		case CBotAI::EMoveBackwards:
			mMovingDirection = mOrientation + PI;
			mVelocity = .1f;
			break;
		case CBotAI::EMoveLeft:
			mMovingDirection = mOrientation + PI / 2.f;
			mVelocity = .1f;
			break;
		case CBotAI::EMoveRight:
			mMovingDirection = mOrientation + 3 * PI / 2.f;
			mVelocity = .1f;
			break;
		}
		if (mMovingDirection >= 2 * PI)
			mMovingDirection -= 2 * PI;
	}
	mBotAction = (TBotAction)(mBotAI->action() & (EActionShoot | EActionBunker | EActionPickWeapon | EActionDropWeapon));
	if (mBotAction == EActionShoot && mWeapon->shoot())
	{
		// TODO: Add bullet and voice.
		mActionDelay = (char)mWeapon->reloadTime();
	}
	else if (mBotAction == EActionBunker)
		mActionDelay = 4;
	else if (mBotAction == EActionPickWeapon)
		mActionDelay = 2;
	else if (mBotAction == EActionDropWeapon)
		mActionDelay = 1;
}

void CBot::update(float aTimeFactor)
{
	if (!mBotAI)
		return;
	move(aTimeFactor);
	// TODO: Set mAnimationTimer.
}

void CBot::loadAI()
{
	if (mBotAI)
		delete mBotAI;

#ifdef _WINDOWS_
	if (mDllHandle)
		FreeLibrary(mDllHandle);
	mDllHandle = LoadLibrary(mDllName);
	if (mDllHandle)
	{
		TBotAIGetter getter = (TBotAIGetter)GetProcAddress(mDllHandle, "getBotAI");
		if (getter)
			mBotAI = getter();
	}
#else
	if (mDllHandle)
		dlclose(mDllHandle);
	mDllHandle = dlopen(mDllName, RTLD_LAZY);
	if (mDllHandle)
	{
		TBotAIGetter getter = (TBotAIGetter)dlsym(mDllHandle, "getBotAI");
		if (getter)
			mBotAI = getter();
	}
#endif
}
