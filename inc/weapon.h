/** @file weapon.h
 * Definition of weapon attribute class CWeapon.
 * This header specifies a blablabla for...
 *
 * @author inz
 * @version 0.1
 */

#ifndef WEAPON_H
#define WEAPON_H

#include <istream>

using std::istream;

/**
 * Class in which information about different weapon types is stored.
 */
class CWeapon
{
public:
	/**
	 * Constructor from stream.
	 * Creates a weapon preference container, reading data from aStream.
	 * @param aStream Stream containing info for the.
	 */
	CWeapon(istream *aStream);

	/** Copy constructor. */
	CWeapon(const CWeapon *aWeapon);

	/**
	 * Reduces amount of ammo if possible
	 * @return True if ammo left, false if not.
	 */
	bool shoot();
	
	/**
	 * Getter for reload time.
	 * @return Reload time in turns.
	 */
	int reloadTime() const;
	
	/**
	 * Getter for bullet count.
	 * @return Amount of bullets available.
	 */
	int bulletCount() const;

	/**
	 * Getter for bullet speed.
	 * @return Speed of bullet in tiles/turn.
	 */
	float bulletSpeed() const;

	/**
	 * Getter for explosion radius.
	 * @return Radius of explosion of bullet in tiles, 0 for non-explosive bullet.
	 */
	float explosionRadius() const;
	
	/**
	 * Getter for explosion speed.
	 * @return Explosion radius growth speed in tiles/turn.
	 */
	float explosionSpeed() const;

	/**
	 * Getter for bullet damage.
	 * @return Amount of damage done when hit/caught in explosion.
	 */
	float damage() const;

private:
	int mReloadTime;
	int mBulletCount;
	float mBulletSpeed;
	float mExplosionRadius;
	float mExplosionSpeed;
	float mDamage;
};

#endif // WEAPON_H
