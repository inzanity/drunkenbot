#ifndef	MESSAGELIST_H
#define	MESSAGELIST_H

#include <list>
#include "gameobj.h"
#include "indexlist.h"

using namespace std;

class CMessage
{
public:
	CMessage(uint16 aMsg, CGameObjPtr aSender, CGameObjPtr aReceiver,
		uint32 aParam1, uint32 aParam2, uint32 aTime);
	uint16	mMsg;
	CGameObjPtr	mSender, mReceiver;
	uint32	mParam1, mParam2;
	uint32	mTime;
};

class CMessageList
{
public:
				CMessageList	();
	virtual		~CMessageList	();
	void		add				(CMessage *aMsg);
	void		sendMessages	(uint32 aTime);
private:
	list<CMessage *>	mList;
};

#endif // MESSAGELIST_H