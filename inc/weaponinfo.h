/** @file weapon.h
 * Definition of weapon info classes.
 * This header specifies the public (for the AI:s) and private
 * (for the engine) data structures for weapons lying in the field
 *
 * @author inz
 * @version 0.1
 */

#ifndef WEAPON_H
#define WEAPON_H

/**
 * Public interface for weapons.
 * When bots see a weapon, this is the info they get of it
 */
class CVisibleWeaponInfo : public CGameObj
{
public:
	/** Default constructor */
	CVisibleWeaponInfo();

	/**
	 * Copy constructor
	 * This will only be called with data of type, and will change
	 * coordinates from absolute to relative, thus needing current
	 * location of the bot
	 *
	 * @param aWeapon The private info of the weapon.
	 * @param aXPos Current X coordinate of the bot.
	 * @param aYPos Current Y coordinate of the bot.
	 */
	CVisibleWeaponInfo(const CVisibleWeaponInfo *aWeapon, float aXPos, float aYPos);
	
	/** Destructor. */
	virtual ~CVisibleWeaponInfo();

	/**
	 * Getter for the weapon type.
	 * @return Type of weapon.
	 * */
	int weaponType();
	/**
	 * Getter for the bullet type.
	 * @return Type of bullet the weapon shoots.
	 */

private:
	int mWeaponType;
	int mBulletType;
};

/**
 * Private interface for weapons.
 * Data structure the engine uses for storing information about weapons
 * to be found.
 */
class CWeaponInfo : public CVisibleWeaponInfo
{
	/** Default constructor */
	CWeaponInfo();

	/**
	 * Constructor with type and coordinates.
	 * Creates a weapon of given type to given coordinates..
	 * @param aWeaponType Type of weapon.
	 * @param aXPos X position for the weapon.
	 * @param aYPos Y position for the weapon.
	 */
	CWeaponInfo(int aWeaponType, float aXPos, float aYPos);

	/** Destructor. */
	virtual ~CWeaponInfo();
};

#endif // WEAPON_H
