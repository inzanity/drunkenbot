#include "../inc/graphicsengine.h"

CGraphicsEngine::CGraphicsEngine() : mActiveBot(NULL)
{
}

CGraphicsEngine::~CGraphicsEngine()
{
}

void CGraphicsEngine::setActiveBot(const CBot *aBot)
{
	mActiveBot = aBot;
}
