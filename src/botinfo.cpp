#include "../inc/botinfo.h"

// CVisibleBotInfo

CVisibleBotInfo::CVisibleBotInfo(int aTeamInfo) : CMovingGameObj(aTeamInfo | EObjectBot), mEnemy(false)
{
}

CVisibleBotInfo::CVisibleBotInfo(CVisibleBotInfo *aBotInfo, float aXPos, float aYPos, int aType) :
								CMovingGameObj(aBotInfo), mEnemy((aType & KTeamMask) == (mType & KTeamMask))
{
	mPos.mX = aBotInfo->xPos() - aXPos;
	mPos.mY = aBotInfo->yPos() - aYPos;
}

CVisibleBotInfo::~CVisibleBotInfo()
{
}

bool CVisibleBotInfo::enemy()
{
	return mEnemy;
}



// CBotInfo

CBotInfo::CBotInfo(int aTeamInfo) : CVisibleBotInfo(aTeamInfo), mWeapon(NULL), mBotAction(EActionNone),
									mActionDelay(0), mBunkered(false), mHealth(0), mSpeedFactor(0),
									mArmourFactor(0), mAimingFactor(0), mFragNum(0)
{
}

CBotInfo::CBotInfo(CBotInfo *aBotInfo, float aXPos, float aYPos) : CVisibleBotInfo(aBotInfo, aXPos, aYPos, aBotInfo->type()),
																   mWeapon(aBotInfo->mWeapon), mBunkered(aBotInfo->mBunkered),
																   mHealth(aBotInfo->mHealth),
																   mSpeedFactor(aBotInfo->mSpeedFactor),
																   mArmourFactor(aBotInfo->mArmourFactor),
																   mAimingFactor(aBotInfo->mAimingFactor),
																   mFragNum(aBotInfo->mFragNum)
{
}

CBotInfo::~CBotInfo()
{
}

const CWeapon *CBotInfo::weapon()
{
	return mWeapon;
}

TBotAction CBotInfo::currentAction()
{
	return mBotAction;
}

char CBotInfo::actionDelay()
{
	return mActionDelay;
}

bool CBotInfo::bunkered()
{
	return mBunkered;
}

int CBotInfo::fragNum()
{
	return mFragNum;
}

bool CBotInfo::handleCollision(char aDamage)
{
	mHealth -= char(aDamage * mArmourFactor);
	return (mHealth < 0);
}

char CBotInfo::getDamage() const
{
	return 15;
}

void CBotInfo::changeFragNum(bool aAddFrag)
{
	if (aAddFrag)
		mFragNum++;
	else
		mFragNum--;
}

char CBotInfo::health() const
{
	return mHealth;
}

float CBotInfo::speedFactor() const
{
	return mSpeedFactor;
}
