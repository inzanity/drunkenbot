#ifndef INDEXLIST_H
#define INDEXLIST_H

#define		KNullIndex		0xFFFF

#include "def.h"

// Interface

template <class T>
class MIndexList
{
public:
	virtual uint16		firstEmpty	()				= 0;
	virtual void		add			(T *aObj)		= 0;
	virtual void		remove		(uint16 aIndex)	= 0;
	virtual T *			object		(uint16 aIndex)	= 0;
};

// Default implementation

template <class T>
class TIndexListNode
{
public:
	uint16				mNext, mPrev;	
	T *					mObj;
};

template <class T>
class CIndexList : public MIndexList<T>
{
public:
						CIndexList			(uint16 aMaxSize);
	virtual				~CIndexList			();
	inline	uint16		size				();
	inline	uint16		maxSize				();
	inline	uint16		first				();
	inline	uint16		firstEmpty			();
	inline	uint16		end					();
			T *			object				(uint16 aIndex);
			void		add					(T *aObj);
			void		remove				(uint16 aIndex);
			void		removeAll			();
			void		removeAndDestroy	(uint16 aIndex);
			void		removeAndDestroyAll	();
			void		increaseSize		(uint16 aNum);
private:
			void		init				();
public:
	TIndexListNode<T> *	mTable;
private:
	uint16				mSize, mMaxSize;
	uint16				mFirst, mFirstEmpty;
};

#include "..\source\indexList.inl"

#endif // INDEXLIST_H