/** @file weapon.h
 * Definition of weapon attribute class.
 * This header specifies a blablabla for...
 *
 * @author inz
 * @version 0.1
 */

#ifndef WEAPON_H
#define WEAPON_H

using std::istream;

/**
 * Blablabla
 * foo
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
	 * Getter for bullet count.
	 * @return Amount of bullets available.
	 */
	int bulletCount();

	/**
	 * Getter for bullet speed.
	 * @return Speed of bullet in tiles/turn.
	 */
	float bulletSpeed();

	/**
	 * Getter for explosion radius.
	 * @return Radius of explosion of bullet in tiles, 0 for non-explosive bullet.
	 */
	float explosionRadius();
	
	/**
	 * Getter for explosion speed.
	 * @return Explosion radius growth speed in tiles/turn.
	 */
	float explosionSpeed();

	/**
	 * Getter for bullet damage.
	 * @return Amount of damage done when hit/caught in explosion.
	 */
	float damage();

private:
	int mBulletCount;
	float mBulletSpeed;
	float mExplosionRadius;
	float mExplosionSpeed;
	float mDamage;
};

#endif // WEAPON_H
