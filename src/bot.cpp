#include <cstring>
#include <cstdlib>
#include "../inc/bot.h"

CBot::CBot(const char *aDllName) : mBotAI(NULL), mDllName(NULL), mDllHandle(NULL), mFrags(0)
{
	mDllName = new char[strlen(aDllName) + 1];
	strcpy(mDllName, aDllName);
}

void CBot::spawn(const char ** aTilemap, int aWidth, int aHeight, const CGameObj **aGameObjects)
{
	mPos.mX = 1.f + rand() % (aWidth-2);
	mPos.mY = 1.f + rand() % (aHeight-2);
	loadAI();
	mRadius = 1.f;
}

void CBot::think(const char **aTilemap, CVisibleBotInfo **aBots, list<CBulletInfo *> *aBulletList,
				 list<CWeaponInfo *> *aWeaponList, list<TVector> *aVoices)
{
	if (!mBotAI)
		return;
	// TODO: copy data and update visible objects
	mBotAI->think();
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
