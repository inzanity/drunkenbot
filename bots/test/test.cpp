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
		if (fabs(angle) > PI / 8 && action() == EActionNone)
		{
			if  (angle < 0)
				turn(ETurnRight);
			else
				turn(ETurnLeft);
		}
		move(EMoveForward);
	}
	else
	{
		turn(ETurnRight);
	}
}
