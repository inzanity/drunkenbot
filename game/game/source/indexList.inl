#include "indexList.h"

template <class T>
CIndexList<T>::CIndexList(uint16 aMaxSize) : mMaxSize(aMaxSize)
{
	init();
}

template <class T>
CIndexList<T>::~CIndexList()
{
	if (mTable)
		delete [] mTable;
}

template <class T>
inline uint16 CIndexList<T>::size() const
{
	return mSize;
}

template <class T>
inline uint16 CIndexList<T>::maxSize() const
{
	return mMaxSize;
}

template <class T>
inline uint16 CIndexList<T>::first() const
{
	return mFirst;
}

template <class T>
inline uint16 CIndexList<T>::firstEmpty() const
{
	return mFirstEmpty;
}

template <class T>
inline uint16 CIndexList<T>::end() const
{
	return mFirstEmpty;
}

template <class T>
void CIndexList<T>::add(T *aObj)
{
	if (mSize == mMaxSize - 1)
		increaseSize(mMaxSize);
	mTable[mFirstEmpty].mObj = aObj;
	mFirstEmpty = mTable[mFirstEmpty].mNext;
	mSize += 1;
}

template <class T>
void CIndexList<T>::remove(uint16 aIndex)
{
	if (!mTable[aIndex].mObj)
		return;
	mTable[aIndex].mObj = 0;
	if (!--mSize)
	{
 		mFirst = mFirstEmpty = aIndex;
		return;
	}
	if (aIndex == mFirst)
	{
 		mFirst = mTable[aIndex].mNext;
		mTable[mFirst].mPrev = KNullIndex;
	}
	else
	{
		mTable[mTable[aIndex].mPrev].mNext = mTable[aIndex].mNext;
		mTable[mTable[aIndex].mNext].mPrev = mTable[aIndex].mPrev;
	}
	mTable[aIndex].mNext = mFirstEmpty;
	mTable[aIndex].mPrev = mTable[mFirstEmpty].mPrev;
	mTable[mTable[mFirstEmpty].mPrev].mNext = aIndex;
	mTable[mFirstEmpty].mPrev = aIndex;
	mFirstEmpty = aIndex;
}

template <class T>
void CIndexList<T>::removeAll()
{
	while (mSize)
		remove(mFirst);
}

template <class T>
void CIndexList<T>::removeAndDestroy(uint16 aIndex)
{
	delete mTable[aIndex].mObj;
	remove(aIndex);
}

template <class T>
void CIndexList<T>::removeAndDestroyAll()
{
	while (mSize)
		removeAndDestroy(mFirst);
}

template <class T>
void CIndexList<T>::increaseSize(uint16 aNum)
{
	uint16 newMaxSize = mMaxSize + aNum;
	TIndexListNode<T> *temp = new TIndexListNode<T>[newMaxSize];
	for (uint16 i = 0; i < newMaxSize; i++)
		if (i < mSize)
		{
			temp[i].mNext	= mTable[i].mNext;
			temp[i].mPrev	= mTable[i].mPrev;
			temp[i].mObj	= mTable[i].mObj;
		}
		else
		{
			temp[i].mNext	= i + 1;
			temp[i].mPrev	= i - 1;
			temp[i].mObj	= 0;
		}
	mTable[0].mPrev = temp[i - 1].mNext = KNullIndex;
	mMaxSize = newMaxSize;
	delete [] mTable;
	mTable = temp;
}

template <class T>
T *CIndexList<T>::object(uint16 aIndex)
{
	return mTable[aIndex].mObj;
}

template <class T>
void CIndexList<T>::init()
{
	mTable = new TIndexListNode<T>[mMaxSize];
	for (uint16 i = 0; i < mMaxSize; i++)
	{
		mTable[i].mNext	= i + 1;
		mTable[i].mPrev	= i - 1;
		mTable[i].mObj	= 0;
	}
	mTable[0].mPrev = mTable[i - 1].mNext = KNullIndex;
	mFirst = mFirstEmpty = mSize = 0;
}
