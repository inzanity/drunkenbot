#include <cstring>
#include <cstdlib>
#include "../inc/bot.h"

#include <cmath>

CBot::CBot(const char *aDllName, int aTeamNumber) : CBotInfo(aTeamNumber << KObjectTeamShift), mBotAI(NULL), mDllName(NULL), mDllHandle(NULL), mFrags(0)
{
	mDllName = new char[strlen(aDllName) + 1];
	strcpy(mDllName, aDllName);
}

void CBot::spawn(const char ** aTilemap, int aWidth, int aHeight, const CGameObj **aGameObjects, const CWeapon *aWeapon)
{
	loadAI();
	int speed, armour, aiming, size;
	mBotAI->init(speed, armour, aiming, size);
	float sum = speed + armour + aiming + size;
	if (speed >= 75 && speed <= 125 && armour >= 75 && armour <= 125 &&
		aiming >= 75 && aiming <= 125 && size >= 75 && size <= 125 && sum == 400)
	{
		mSpeedFactor = speed * .01f;
		mArmourFactor = 2.f - armour * .01f;
		mAimingFactor = aiming * .01f;
		mRadius = 1.f - size * .005f;
	}
	else
	{
		mSpeedFactor = mArmourFactor = mAimingFactor = 1.f;
		mRadius = .5f;
	}

	int tile;
	do {
		mPos.mX = 1 + rand() % (aWidth-2);
		mPos.mY = 1 + rand() % (aHeight-2);
		tile = aTilemap[int(mPos.mY - mRadius)][int(mPos.mX + mRadius)] &
				   aTilemap[int(mPos.mY - mRadius)][int(mPos.mX - mRadius)] &
				   aTilemap[int(mPos.mY + mRadius)][int(mPos.mX + mRadius)] &
				   aTilemap[int(mPos.mY + mRadius)][int(mPos.mX - mRadius)];
	} while ((tile & KTileTypeMask) != CTilemap::ETileEmpty);
	mSpawningPos = mPos;
	mBotAI->mTilemap = new CTilemap(aWidth, aHeight);
	mHealth = 100;
	mWeapon = new CWeapon(aWeapon);
}

void CBot::think(const char **aTilemap, int aWidth, int aHeight, CVisibleBotInfo **aBots,
				 list<CBulletInfo *> *aBulletList, list<CWeaponInfo *> *aWeaponList, list<TVector> *aVoices)
{
	mBotAI->resetAction();
	if (mBotAI->mData)
		delete mBotAI->mData;
	mBotAI->mData = new CBotInfo(this, mSpawningPos.mX, mSpawningPos.mY);
	std::list<CBulletInfo *>::iterator bulIter;
	std::list<CWeaponInfo *>::iterator weaIter;
	std::list<TVector>::iterator voiIter;
	if (!mBotAI)
		return;
	mBotAI->mTilemap->resetFogOfWar();
	float dAngle = KFov / (2 * sqrt(aWidth * aWidth + aHeight * aHeight) * tan(KFov / 2.f));
	scanTilemap(aTilemap, dAngle);
	for (int i = 0; aBots[i]; i++)
	{
		if (aBots[i]->type() != mType && !(mBotAI->mTilemap->getTile(floorf(aBots[i]->xPos()) - mSpawningPos.mX, floorf(aBots[i]->yPos()) - mSpawningPos.mY) & KTileFowMask))
		{
			mBotAI->mBots.push_front(new CVisibleBotInfo(aBots[i], mSpawningPos.mX, mSpawningPos.mY, mType));
		}
	}
	for (weaIter = aWeaponList->begin(); weaIter != aWeaponList->end(); weaIter++)
		if (!(aTilemap[int((*weaIter)->yPos())][int((*weaIter)->xPos())] & 0x80))
			mBotAI->mWeapons.push_front(new CVisibleWeaponInfo(*weaIter, mSpawningPos.mX, mSpawningPos.mY));

	for (bulIter = aBulletList->begin(); bulIter != aBulletList->end(); bulIter++)
		if (!(aTilemap[int((*bulIter)->yPos())][int((*bulIter)->xPos())] & 0x80))
			mBotAI->mBullets.push_front(new CVisibleBulletInfo(*bulIter, mSpawningPos.mX, mSpawningPos.mY));

	for (voiIter = aVoices->begin(); voiIter != aVoices->end(); voiIter++)
		if (!(aTilemap[int((*voiIter).mY)][int((*voiIter).mX)] & 0x80))
			mBotAI->mSourcesOfNoise.push_front(atan2((*voiIter).mY - mPos.mY, (*voiIter).mX - mPos.mX));

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
			else if (mBotAction == EActionPickWeapon)
				mWeapon = mWeapon; // TODO
			else if (mBotAction == EActionDropWeapon)
				mWeapon = mWeapon; // TODO
			mBotAction = EActionNone;
		}

	if (!mBotAI->checkAction() ||
		(mActionDelay && mBotAI->action() > EActionTurn) ||
		(mBunkered && (mBotAI->action() & EActionMove)))
		mBotAI->resetAction();
	if (mBotAI->action() & EActionTurn)
	{
		if (mBotAI->turningDir() == CBotAI::ETurnLeft)
			mOrientation -= KTurnSpeed;
		else if (mBotAI->turningDir() == CBotAI::ETurnRight)
			mOrientation += KTurnSpeed;
		if (mOrientation < 0)
			mOrientation += 2 * PI;
		else if (mOrientation >= 2 * PI)
			mOrientation -= 2 * PI;
	}
	if (mBotAI->action() & EActionMove)
	{
		switch (mBotAI->movingDir())
		{
		case CBotAI::EMoveForward:
			mMovingDirection = mOrientation;
			mVelocity = mSpeedFactor * KMoveSpeedForward;
			break;
		case CBotAI::EMoveBackwards:
			mMovingDirection = mOrientation + PI;
			mVelocity = mSpeedFactor * KMoveSpeedBackwards;
			break;
		case CBotAI::EMoveLeft:
			mMovingDirection = mOrientation + PI / 2.f;
			mVelocity = mSpeedFactor * KMoveSpeedBackwards;
			break;
		case CBotAI::EMoveRight:
			mMovingDirection = mOrientation + 3 * PI / 2.f;
			mVelocity = mSpeedFactor * KMoveSpeedBackwards;
			break;
		}
		if (mMovingDirection >= 2 * PI)
			mMovingDirection -= 2 * PI;
	}
	else
		mVelocity = 0;

	if (!mActionDelay)
	{
		mBotAction = (TBotAction)(mBotAI->action() & (EActionShoot | EActionBunker | EActionPickWeapon | EActionDropWeapon));
		if (mBotAction == EActionShoot && mWeapon->shoot())
		{
			TVector pos = { mPos.mX + (mRadius + 0.3f) * cos(mOrientation), mPos.mY + (mRadius + 0.3f) * sin(mOrientation) };
			aBulletList->push_back(new CBulletInfo(/*mWeapon->type()*/0,
												   mPos.mX + (mRadius + 0.5f) * cos(mOrientation),
												   mPos.mY + (mRadius + 0.5f) * sin(mOrientation),
												   mOrientation + mBotAI->shootingDir(), mWeapon->bulletSpeed(), this));
			aVoices->push_back(pos);
			mActionDelay = (char)mWeapon->reloadTime();
		}
		else if (mBotAction == EActionBunker)
			mActionDelay = 4;
		else if (mBotAction == EActionPickWeapon)
			mActionDelay = 2;
		else if (mBotAction == EActionDropWeapon)
			mActionDelay = 1;
	}
}

bool CBot::update()
{
	CMovingGameObj::update();
	if (mHealth <= 0)
		return false;
	return true;
}

const CBotAI *CBot::botAI() const
{
	return mBotAI;
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

float CBot::spawningXPos() const
{
	return mSpawningPos.mX;
}

float CBot::spawningYPos() const
{
	return mSpawningPos.mY;
}

void CBot::scanTilemap(const char ** aTilemap, float aDAngle) const
{
	float time;
	char tile;
	for (float angle = mOrientation - KFov / 2.f; angle < mOrientation + KFov / 2.f; angle += aDAngle)
	{
		TVector pos = mPos;
		TVector speed = {cos(angle), sin(angle)};
		time = 0;
		for (tile = aTilemap[(int)floorf(pos.mY)][(int)floorf(pos.mX)];
			 (CTilemap::TTileType)(tile & KTileTypeMask) != CTilemap::ETileWall;
			 tile = aTilemap[(int)floorf(pos.mY)][(int)floorf(pos.mX)])
		{
			mBotAI->mTilemap->setTile((int)(floorf(pos.mX) - mSpawningPos.mX), (int)(floorf(pos.mY) - mSpawningPos.mY), tile);
			time += getNextEdge(pos, speed);
			pos.mX = mPos.mX + time * speed.mX;
			pos.mY = mPos.mY + time * speed.mY;
		}
		mBotAI->mTilemap->setTile((int)(floorf(pos.mX) - mSpawningPos.mX), (int)(floorf(pos.mY) - mSpawningPos.mY), tile);
	}
}
