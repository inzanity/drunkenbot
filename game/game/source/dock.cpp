#include "../include/game.h"
#include "../include/dock.h"
#include "../include/animationStorage.h"

CDock::CDock(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
CBuilding(aObjPtr, (mDock = new CBuildingData(CAnimationStorage::ptr()->getAnimation("data/dock.x"), CAnimationStorage::ptr()->getAnimation("data/dock.x"), 1, "Foobar", NULL, 0, 1, 1, 1, 1, 1, 1, GENERAL, 1, NULL)), aReady, aPos, aXAngle, aYAngle)
{
	mSize					= 1.5f;
	game->sendMessage(EMsgBuildingReady, this, 0, 0, 0);
}

CDock::CDock(istream &aStream) :
	CBuilding(aStream)
{
}

CDock::~CDock()
{
}

void CDock::externalize(ostream &aStream)
{
}

void CDock::handleMessage(CMessage *aMsg)
{
	D3DXVECTOR3		temp;
	int				i;

	switch (aMsg->mMsg)
	{
	case EMsgBuildingReady:
		i = 1;
	}
}

