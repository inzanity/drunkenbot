#ifdef WIN32
# include <windows.h>
#else
# include <dlfcn.h>
#endif
#include <cstring>
#include <cstdlib>
#include "../inc/bot.h"

	CBotAI *mBotAI;
	char *mDllName;
	HMODULE mDllHandle;
	int mFrags;

CBot::CBot(const char *aDllName) : mBotAI(NULL), mDllName(NULL), mDllHandle(NULL), mFrags(0)
{
	mDllName = new char[strlen(aDllName) + 1];
	strcpy(mDllName, aDllName);
}

void CBot::spawn(const char ** /*aTilemap*/, int aWidth, int aHeight, const CGameObj * /*aGameObjects*/)
{
	mPos.mX = float(rand() % aWidth);
	mPos.mY = float(rand() % aHeight);
	loadAI();
}

void CBot::think(const char ** /*aTilemap*/, CVisibleBotInfo * /*aBots*/, list<CBulletInfo *> * /*aBulletList*/,
				 list<CWeaponInfo *> * /*aWeaponList*/, list<TPosition> * /*aVoices*/)
{
	if (!mBotAI)
		return;
	// TODO: copy data and update visible objects
	mBotAI->think();
}

void CBot::performActions(list<CWeaponInfo *> * /*aBulletList*/, list<TPosition> * /*aVoices*/)
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

void CBot::chkCollision(const char ** /*aTilemap*/, CBotInfo ** /*aBots*/)
{
	// TODO
	if (!mBotAI)
		return;
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
//		else
//			cout<<"No function found\n";
	}
//	else
//		cout<<"No library found.\n";
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
	// TODO: #ifdef _PURKKA_
}
