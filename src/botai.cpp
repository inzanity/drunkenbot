#include "../inc/botai.h"

#define KExclusiveActions (EActionShoot | EActionBunker | EActionPickWeapon | EActionDropWeapon)

CBotAI::CBotAI() : mTilemap(NULL), mAction(EActionNone), mMovingDir(EMoveForward),
				   mTurningDir(ETurnLeft), mShootingDir(0)
{
}

CBotAI::~CBotAI()
{
}

void CBotAI::init(int &aSpeed, int &aArmour, int &aAiming)
{
	aSpeed = aArmour = aAiming = 1;
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
	return setAction(EActionBunker, KExclusiveActions);
}

bool CBotAI::pickWeapon()
{
	return setAction(EActionPickWeapon, KExclusiveActions);
}

bool CBotAI::dropWeapon()
{
	return setAction(EActionDropWeapon, KExclusiveActions);
}

int CBotAI::action() const
{
	return mAction;
}

bool CBotAI::checkAction() const
{
	return true;
}

bool CBotAI::setAction(TBotAction aAction, int aExclusiveActions)
{
	int old = mAction;
	mAction &= !aExclusiveActions;
	mAction |= EActionMove;
	if (checkAction())
		return true;
	mAction = old;
	return false;
}