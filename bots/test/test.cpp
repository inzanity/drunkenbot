#include <cmath>
#include "test.h"

float ff(float foo)
{
	while (foo > PI)
		foo -= 2 * PI;
	while (foo < -PI)
		foo += 2 * PI;
	return foo;
}

float sqr(float bar)
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
	float angle;
	if (mBots.size())
	{
		angle = ff(atan2((*mBots.begin())->yPos() - mData->yPos(), (*mBots.begin())->xPos() - mData->xPos()) - mData->orientation());
		if (fabs(angle) > PI / 20 && action() == EActionNone)
		{
			if  (angle < 0)
				turn(ETurnRight);
			else
				turn(ETurnLeft);
		}
		else
			if (sqr((*mBots.begin())->yPos() - mData->yPos()) + sqr((*mBots.begin())->xPos() - mData->xPos()) < 49)
				shoot(angle);
		move(EMoveForward);
	}
	else
	{
		turn(ETurnRight);
	}
}
