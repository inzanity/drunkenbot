#include "../inc/botai.h"

#define KExclusiveActions	(EActionShoot | EActionBunker | EActionPickWeapon | EActionDropWeapon)
#define KAllActions			(EActionMove | EActionTurn | EActionShoot | EActionBunker | EActionPickWeapon | EActionDropWeapon)

CBotAI::CBotAI() : mTilemap(NULL), mData(NULL), mAction(EActionNone), mMovingDir(EMoveForward),
				   mTurningDir(ETurnLeft), mShootingDir(0)
{
}

CBotAI::~CBotAI()
{
	if (mTilemap)
		delete mTilemap;
	if (mData)
		delete mData;
}

void CBotAI::init(int &aSpeed, int &aArmour, int &aAiming, int &aSize)
{
	aSpeed = aArmour = aAiming = aSize = 100;
}

void CBotAI::resetAction()
{
	mAction = EActionNone;
}

bool CBotAI::move(TBotMovingDir aDir)
{
	if (aDir != EMoveForward && aDir != EMoveBackwards && aDir != EMoveLeft && aDir != EMoveRight)
		return false;
	if (setAction(EActionMove, EActionBunker | EActionPickWeapon | EActionDropWeapon))
	{
		mMovingDir = aDir;
		return true;
	}
	return false;
}

bool CBotAI::turn(TBotTurningDir aDir)
{
	if (aDir != ETurnLeft && aDir != ETurnRight)
		return false;
	if (setAction(EActionTurn, EActionBunker | EActionPickWeapon | EActionDropWeapon))
	{
		mTurningDir = aDir;
		return true;
	}
	return false;
}

bool CBotAI::shoot(float aDir)
{
	if (aDir > PI/8.f || aDir < -PI/8.f)
		return false;
	if (setAction(EActionShoot, KExclusiveActions))
	{
		mShootingDir = aDir;
		return true;
	}
	return false;
}

bool CBotAI::bunker()
{
	return setAction(EActionBunker, KAllActions);
}

bool CBotAI::pickWeapon()
{
	return setAction(EActionPickWeapon, KAllActions);
}

bool CBotAI::dropWeapon()
{
	return setAction(EActionDropWeapon, KAllActions);
}

void CBotAI::cancelAction(TBotAction aAction)
{
	mAction &= aAction ^ 0xffffffff;
}

int CBotAI::action() const
{
	return mAction;
}

bool CBotAI::checkAction() const
{
	if ((mData->actionDelay() && mAction > EActionTurn) ||
		(mData->bunkered() && (mAction & EActionMove)))
		return false;
	int action = mAction & KExclusiveActions;
	if (action != 0 && action != EActionShoot && action != EActionBunker &&
		action != EActionPickWeapon && action != EActionDropWeapon)
		return false;
	return true;
}

CBotAI::TBotMovingDir CBotAI::movingDir()
{
	return mMovingDir;
}

CBotAI::TBotTurningDir CBotAI::turningDir()
{
	return mTurningDir;
}

float CBotAI::shootingDir()
{
	return mShootingDir;
}

bool CBotAI::setAction(TBotAction aAction, int aExclusiveActions)
{
	int old = mAction;
	mAction &= aExclusiveActions ^ 0xffffffff;
	mAction |= aAction;
	if (checkAction())
		return true;
	mAction = old;
	return false;
}
