#ifndef GAME_H
#define GAME_H

#include "indexList.h"
#include "gameObj.h"
#include "camera.h"
#include "message.h"
#include "heightMap.h"
#include "messageBox.h"

class CGame
{
public:
	CGame();
	~CGame();
	bool init();
	bool loop();
	void sendMessage(CMessage *aMsg);
	void sendMessage(uint16 aMsg, uint16 aSenderId, uint16 aReceiverId,
					 uint16 aSender, uint16 aReceiver,
					 uint32 aParam1, uint32 aParam2, uint32 aTime = 0);
	void sendMessage(uint16 aMsg, MGameObj *aObj,
					 uint32 aParam1, uint32 aParam2, uint32 aTime = 0);
	void destroyObj(MGameObj *aObj);
	MGameObj *getGameObj(uint16 aIndex, uint16 aId);

private:
	void chkDestroyList	();
private:
	list<MGameObj *> mDestroyList;
	CMessageList mMsgList;
	CIndexList<CDrawable> mBuildings;
	CHeightMap *mHeightMap;
	CCamera *cam;
	CMessageBox *mMessageBox;
	int mTime;
};

extern CGame *game;

#endif // GAME_H