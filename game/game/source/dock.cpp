#include "../include/game.h"
#include "../include/dock.h"
#include "../include/animationStorage.h"

CDock::CDock(CGameObjPtr aObjPtr, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
CBuilding(aObjPtr, new CBuildingData(CAnimationStorage::ptr()->getAnimation("data/dock.x"), NULL, CAnimationStorage::ptr()->getAnimation("data/dock.x"), 1, "Foobar", NULL, 0, 1, 1, 1, 1, 1, 1, DOCKYARD, 1, NULL), aReady, aPos, aXAngle, aYAngle)
{
	game->sendMessage(EMsgBuildingReady, this, 0, 0, 0);
}

CDock::CDock(CGameObjPtr aObjPtr, CBuildingData *aBuildingData, bool aReady, const D3DXVECTOR3 *aPos, float aXAngle, float aYAngle) :
	CBuilding(aObjPtr, aBuildingData, aReady, aPos, aXAngle, aYAngle)
{
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
	CBuilding::handleMessage(aMsg);
}

