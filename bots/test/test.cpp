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
	float angle = 100000.f;
	float plop;
	CVisibleBotInfo *target;
	list<CVisibleBotInfo *>::iterator i;
	for (i = mBots.begin(); i != mBots.end(); i++)
		if ((plop = pow2((*i)->yPos() - mData->yPos()) + pow2((*i)->xPos() - mData->xPos())) < angle)
		{
			angle = plop;
			target = *i;
		}
	if (mBots.size())
	{
		angle = ff(atan2(target->yPos() - mData->yPos(), target->xPos() - mData->xPos()) - mData->orientation());
		if (fabs(angle) > PI / 20 && action() == EActionNone)
		{
			if  (angle < 0)
				turn(ETurnLeft);
			else
				turn(ETurnRight);
		}
		else
			if (pow2(target->yPos() - mData->yPos()) + pow2(target->xPos() - mData->xPos()) < 49)
				shoot(angle);
		move(EMoveForward);
	}
	else
	{
		turn(ETurnRight);
	}
}
