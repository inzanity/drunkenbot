#include <cmath>
#include <list>
#include "test.h"

using namespace std;

float ff(float foo)
{
	while (foo > PI)
		foo -= 2 * PI;
	while (foo < -PI)
		foo += 2 * PI;
	return foo;
}

float pow2(float bar)
{
	return bar * bar;
}

extern "C" CBotAI *getBotAI()
{
	return new CTestAI();
}


CTestAI::CTestAI()
{
}

CTestAI::~CTestAI()
{
}

void CTestAI::think()
{
	float angle = mData->weapon()->weaponType();
	float plop;
	bool weapon = 0;
	CGameObj *target = NULL;
	list<CVisibleBotInfo *>::iterator b;
	list<CWeaponInfo *>::iterator w;

	for (w = mWeapons.begin(); w != mWeapons.end(); w++)
		if ((plop = (*w)->weaponType()) > angle)
		{
			angle = plop;
			target = *w;
			weapon = 1;
		}
	if (!target)
	{
		angle = 100000;
		for (b = mBots.begin(); b != mBots.end(); b++)
			if ((plop = pow2((*b)->yPos() - mData->yPos()) + pow2((*b)->xPos() - mData->xPos())) < angle)
			{
				angle = plop;
				target = *b;
			}
	}
	if (target)
	{
		angle = ff(atan2(target->yPos() - mData->yPos(), target->xPos() - mData->xPos()) - mData->orientation());
		if (fabs(angle) > PI / 20 && action() == EActionNone)
		{
			if  (angle < 0)
				turn(ETurnLeft);
			else
				turn(ETurnRight);
		}
		move(EMoveForward);
		if (!weapon)
		{
			if (pow2(target->yPos() - mData->yPos()) + pow2(target->xPos() - mData->xPos()) < 49)
				shoot(angle);
		}
		else
			if ((angle = pow2(target->yPos() - mData->yPos()) + pow2(target->xPos() - mData->xPos())) < 1)
				pickWeapon();
	}
	else
	{
		turn(ETurnRight);
	}
}
