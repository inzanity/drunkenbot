#include "../inc/weaponinfo.h"

// CVisibleWeaponInfo

CVisibleWeaponInfo::CVisibleWeaponInfo(int aWeaponType) : CGameObj(aWeaponType << 4 | 3), mWeaponType(aWeaponType)
{
}

CVisibleWeaponInfo::CVisibleWeaponInfo(const CVisibleWeaponInfo *aWeapon, float aXPos, float aYPos) :
									   CGameObj(aWeapon), mWeaponType(aWeapon->mWeaponType)
{
	mPos.mX = aWeapon->xPos() - aXPos;
	mPos.mY = aWeapon->yPos() - aYPos;
}

CVisibleWeaponInfo::~CVisibleWeaponInfo()
{
}

int CVisibleWeaponInfo::weaponType() const
{
	return mWeaponType;
}



// CWeaponInfo

CWeaponInfo::CWeaponInfo(int aWeaponType, float aXPos, float aYPos) : CVisibleWeaponInfo(aWeaponType)
{
	mPos.mX = aXPos;
	mPos.mY = aYPos;
}

CWeaponInfo::~CWeaponInfo()
{
}
