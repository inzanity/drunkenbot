/** @file weaponinfo.h
 * Definition of weapon info classes CWeaponInfo and CWeaponInfo.
 * This header specifies the public (for the AI:s) and private
 * (for the engine) data structures for weapons lying in the field
 *
 * @author inz
 * @version 0.1
 */

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

#include "gameobj.h"
#include "weapon.h"

/**
 * Public interface for weapons.
 * When bots see a weapon, this is the info they get of it
 */
class CWeaponInfo : public CGameObj, CWeapon
{
public:
	/**
	 * Constuctor.
	 * @param aWeapon Source CWeapon to read data from.
	 * @param aXPos X position of the weapon.
	 * @param aYPos Y position of the weapon.
	 */
	CWeaponInfo(const CWeapon *aWeapon, float aXPos, float aYPos);

	/**
	 * Copy constructor with relative position.
	 *
	 * @param aWeapon The private info of the weapon.
	 * @param aXPos Current X coordinate of the bot.
	 * @param aYPos Current Y coordinate of the bot.
	 */
	CWeaponInfo(const CWeaponInfo *aWeapon, float aXPos, float aYPos);
	
	/** Destructor. */
	virtual ~CWeaponInfo();
};

#endif // WEAPONINFO_H
