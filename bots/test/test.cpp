#include <cstdlib>
#include "test.h"

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
	move(EMoveForward);
	int r = rand() % 5;
	if (r == 0)
		turn(ETurnLeft);
	else if (r == 1)
		turn(ETurnRight);
}
