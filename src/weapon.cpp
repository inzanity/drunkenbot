#include <iostream>

#include "../inc/weapon.h"

using std::istream;

CWeapon::CWeapon(istream *aStream)
{
	*aStream >> mBulletCount
		>> mBulletSpeed
		>> mExplosionRadius
		>> mExplosionSpeed
		>> mDamage;
}

CWeapon::CWeapon(const CWeapon *aWeapon)
{
	mBulletCount = aWeapon->mBulletCount;
	mBulletSpeed = aWeapon->mBulletSpeed;
	mExplosionRadius = aWeapon->mExplosionRadius;
	mExplosionSpeed = aWeapon->mExplosionSpeed;
	mDamage = aWeapon->mDamage;
}

bool CWeapon::shoot()
{
	if (!mBulletCount)
		return false;
	--mBulletCount;
	return true;
}

int CWeapon::bulletCount()
{
	return mBulletCount;
}

float CWeapon::bulletSpeed()
{
	return mBulletSpeed;
}

float CWeapon::explosionRadius()
{
	return mExplosionRadius;
}

float CWeapon::explosionSpeed()
{
	return mExplosionSpeed;
}

float CWeapon::damage()
{
	return mDamage;
}
