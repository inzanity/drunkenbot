#ifndef GAME_H
#define GAME_H

#include "indexList.h"
#include "mech.h"
#include "camera.h"
#include "message.h"
#include "heightMap.h"
#include "messageBox.h"
#include "gameui.h"

enum TObjectType{ETypeBuilding, ETypeMech, ETypeExplosion, ETypeCamera};

class CGame
{
public:
	CGame();
	~CGame();
	bool init();
	bool loop();
	void sendMessage(CMessage *aMsg);
	void sendMessage(uint16 aMsg, CGameObjPtr aSender, CGameObjPtr aReceiver,
					 uint32 aParam1, uint32 aParam2, uint32 aTime = 0);
	void sendMessage(uint16 aMsg, MGameObj *aObj,
					 uint32 aParam1, uint32 aParam2, uint32 aTime = 0);
	void destroyObj(MGameObj *aObj);
	CGameObjPtr getNewGameObjectPtr(TObjectType aObjType);
	MGameObj *getGameObj(uint16 aIndex, uint16 aId);
private:
	void chkDestroyList	();

public:
	CHeightMap *mHeightMap;
private:
	list<MGameObj *> mDestroyList;
	CMessageList mMsgList;
	CIndexList<CDrawable> mBuildings;
	CIndexList<CMech> mMechs;
	CCamera *mCam;
	CMessageBox *mMessageBox;
	CGameUI *mGameUI;
	uint32 mTime;
	uint16 mNewId;
};

extern CGame *game;

#endif // GAME_H