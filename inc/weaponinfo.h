/** @file weaponinfo.h
 * Definition of weapon info classes CVisibleWeaponInfo and CWeaponInfo.
 * This header specifies the public (for the AI:s) and private
 * (for the engine) data structures for weapons lying in the field
 *
 * @author inz
 * @version 0.1
 */

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

#include "gameobj.h"

/**
 * Public interface for weapons.
 * When bots see a weapon, this is the info they get of it
 */
class CVisibleWeaponInfo : public CGameObj
{
public:
	/** Default constructor */
	CVisibleWeaponInfo(int aWeaponType);

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

private:
	int mWeaponType;
};

/**
 * Private interface for weapons.
 * Data structure the engine uses for storing information about weapons
 * to be found.
 */
class CWeaponInfo : public CVisibleWeaponInfo
{
public:
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

#endif // WEAPONINFO_H
