#include "../inc/botinfo.h"

// CVisibleBotInfo

CVisibleBotInfo::CVisibleBotInfo(int aTeamInfo) : CMovingGameObj(aTeamInfo | 1), mEnemy(false)
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

CBotInfo::CBotInfo(int aTeamInfo) : CVisibleBotInfo(aTeamInfo), mWeapon(NULL), mBotAction(EActionNone), mActionDelay(0), mBunkered(false), mHealth(0)
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

bool CBotInfo::handleCollision(int aDamage)
{
	mHealth -= aDamage;
	return (mHealth < 0);
}

int CBotInfo::getDamage()
{
	return 0x69;
}

void CBotInfo::addFrag()
{
	mFragNum++;
}
