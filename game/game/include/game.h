#ifndef GAME_H
#define GAME_H

#include "indexList.h"
#include "mech.h"
#include "camera.h"
#include "message.h"
#include "heightMap.h"
//#include "pathFinder.h"
#include "messageBox.h"
#include "rtsModeUI.h"
#include "fpsModeUI.h"
#include "building.h"

enum TObjectType{ETypeBuilding, ETypeMech, ETypeExplosion, ETypeCamera, ETypeNone};

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
	TObjectType getObjType(CGameObjPtr aPtr);
private:
	void chkDestroyList	();

public:
	CHeightMap *mHeightMap;
	CIndexList<CDrawable> mBuildings;
	CIndexList<CMech> mMechs;
	CCamera *mCam;
	CRTSModeUI *mRTSModeUI;
	CFPSModeUI *mFPSModeUI;
private:
	list<MGameObj *> mDestroyList;
//	CPathFinder *mPathFinder;
	CMessageList mMsgList;
	CMessageBox *mMessageBox;
	uint32 mTime;
	uint16 mNewId;
	DWORD mPrevTime;
	int mShowFPS;
};

extern CGame *game;

#endif // GAME_H