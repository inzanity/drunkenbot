#ifndef TEST
#define TEST

#include "../../inc/botai.h"

class CTestAI : public CBotAI
{
public:
	CTestAI();

	virtual ~CTestAI();

	void think();
};

#endif // TEST