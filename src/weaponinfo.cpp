#include "../inc/weaponinfo.h"

CVisibleWeaponInfo::CVisibleWeaponInfo(int aWeaponType) : CGameObj(3), mWeaponType(aWeaponType)
{
}

CVisibleWeaponInfo::CVisibleWeaponInfo(const CVisibleWeaponInfo *aWeapon, float aXPos, float aYPos) : CGameObj(3), mWeaponType(aWeapon->mWeaponType)
{
	mPos.mX = aWeapon->mPos.mX - aXPos;
	mPos.mY = aWeapon->mPos.mY - aYPos;
}

CVisibleWeaponInfo::~CVisibleWeaponInfo()
{
}

int CVisibleWeaponInfo::weaponType()
{
	return mWeaponType;
}


CWeaponInfo::CWeaponInfo(int aWeaponType, float aXPos, float aYPos) : CVisibleWeaponInfo(aWeaponType)
{
	mPos.mX = aXPos;
	mPos.mY = aYPos;
}

CWeaponInfo::~CWeaponInfo()
{
}
