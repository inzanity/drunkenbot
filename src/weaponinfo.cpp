#include "../inc/weaponinfo.h"

// CWeaponInfo

CWeaponInfo::CWeaponInfo(const CWeapon *aWeapon, float aXPos, float aYPos) : CGameObj(aWeapon->type() << KObjectIndexShift | EObjectWeapon), CWeapon(aWeapon)
{
	mPos.mX = aXPos;
	mPos.mY = aYPos;
	mRadius = .5f;
}

CWeaponInfo::CWeaponInfo(const CWeaponInfo *aWeaponInfo, float aXPos, float aYPos) : CGameObj(aWeaponInfo, aXPos, aYPos), CWeapon(aWeaponInfo)
{
}

CWeaponInfo::~CWeaponInfo()
{
}
