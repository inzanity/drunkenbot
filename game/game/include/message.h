#ifndef	MESSAGELIST_H
#define	MESSAGELIST_H

#include <list>
#include "gameobj.h"
#include "indexlist.h"

using std::list;

struct CMessage
{
	CMessage(uint16 aMsg, uint16 aSenderId, uint16 aReceiverId, uint16 aSender,
			uint16 aReceiver, uint32 aParam1, uint32 aParam2, uint32 aTime);
	uint16	mMsg;
	uint16	mSenderId, mReceiverId;
	uint16	mSender, mReceiver;
	uint32	mParam1, mParam2;
	uint32	mTime;
};

class CMessageList
{
public:
				CMessageList	();
	virtual		~CMessageList	();
	void		add				(CMessage *aMsg);
	void		sendMessages	(double aTime, MIndexList<MGameObj> *aObjList);
private:
	list<CMessage *>	mList;
};

#endif // MESSAGELIST_H