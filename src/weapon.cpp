#include <cmath>
#include <iostream>

#include "../inc/weapon.h"

using std::istream;

CWeapon::CWeapon(istream *aStream) : mReloadTime(0), mBulletCount(0), mBulletSpeed(0.f), mExplosionRadius(0.f), mExplosionSpeed(0.f), mDamage(0.f)
{
	*aStream >> mBulletCount
		>> mReloadTime
		>> mBulletSpeed
		>> mExplosionRadius
		>> mExplosionSpeed
		>> mDamage;
	if (mExplosionSpeed <= 0) mExplosionRadius = 0;
	if (mExplosionRadius > 0)
		mDamage /= ceilf(mExplosionRadius / mExplosionSpeed);
}

CWeapon::CWeapon(const CWeapon *aWeapon) : mReloadTime(aWeapon->mReloadTime), mBulletCount(aWeapon->mBulletCount), mBulletSpeed(aWeapon->mBulletSpeed), mExplosionRadius(aWeapon->mExplosionRadius), mExplosionSpeed(aWeapon->mExplosionSpeed), mDamage(aWeapon->mDamage)
{
}

bool CWeapon::shoot()
{
	if (!mBulletCount)
		return false;
	if (mBulletCount > 0)
		--mBulletCount;
	return true;
}

int CWeapon::reloadTime() const
{
	return mReloadTime;
}

int CWeapon::bulletCount() const
{
	return mBulletCount;
}

float CWeapon::bulletSpeed() const
{
	return mBulletSpeed;
}

float CWeapon::explosionRadius() const
{
	return mExplosionRadius;
}

float CWeapon::explosionSpeed() const
{
	return mExplosionSpeed;
}

float CWeapon::damage() const
{
	return mDamage;
}
