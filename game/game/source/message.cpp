#include "..\include\message.h"

CMessage::CMessage(uint16 aMsg, uint16 aSenderId, uint16 aReceiverId, uint16 aSender,
					uint16 aReceiver, uint32 aParam1, uint32 aParam2, uint32 aTime)
					: mMsg(aMsg), mSenderId(aSenderId), mReceiverId(aReceiverId), mSender(aSender),
					mReceiver(aReceiver), mParam1(aParam1), mParam2(aParam2), mTime(aTime)
{
}

CMessageList::CMessageList()
{
}

CMessageList::~CMessageList()
{
	while (mList.size())
	{
		delete mList.back();
		mList.pop_back();
	}
}

void CMessageList::add(CMessage *aMsg)
{
	list<CMessage *>::iterator iter;
	for (iter = mList.begin(); iter != mList.end() && (*iter)->mTime < aMsg->mTime; ++iter);
	mList.insert(iter, aMsg);
}

void CMessageList::sendMessages(double aTime, MIndexList<MGameObj> *aObjList)
{
	list<CMessage *>::iterator iter, tempIter;
	for (iter = mList.begin(); iter != mList.end() && (*iter)->mTime <= aTime;)
	{
		if ((*iter)->mReceiverId == aObjList->object((*iter)->mReceiver)->id()) // Receiver exists
			aObjList->object((*iter)->mReceiver)->handleMessage(*iter);
		tempIter = iter;
		++iter;
		delete *tempIter;
		mList.erase(tempIter);
	}
}