/** @file bullet.h
 * Definition of bullet info classes CVisibleBulletInfo and CBulletInfo.
 * This header specifies the public (for the AI:s) and private
 * (for the engine) data structures for flying bullets.
 *
 * @todo Everything
 *
 * @author inz
 * @version 0.2
 */

#ifndef BULLET_H
#define BULLET_H

#include "gameobj.h"

/**
 * Public class for bullets
 * When bots see a bullet, this is the info they get of it
 */
class CVisibleBulletInfo : public CMovingGameObj
{
public:
	/** Default constructor. */
	CVisibleBulletInfo(int aBulletType);

	/**
	 * Copy constructor.
	 * This will only be called with data of type CBulletInfo,
	 * and will change coordinates from absolute to relative,
	 * thus needing current location of the bot
	 *
	 * @param aBullet The private info of the bullet
	 * @param aXPos Current X coordinate of the bot
	 * @param aYPos Current Y coordinate of the bot
	 */
	CVisibleBulletInfo(const CVisibleBulletInfo *aBullet, float aXPos, float aYPos);

	/** Destructor. */
	virtual ~CVisibleBulletInfo();
	
	/** Getter for the bullet type. */
	int bulletType() const;

private:
	int mBulletType;
};

/**
 * Private class for bullets.
 * Data structure the engine uses for storing information about bullets.
 */
class CBulletInfo : public CVisibleBulletInfo
{
public:
	/** Constructor with type and coordinates.
	 * Creates a bullet of given type to given coordinates.
	 * @param aBulletType Type of bullet.
	 * @param aXPos X position for the bullet.
	 * @param aYPos Y position for the bullet.
	 * @param aDirection Direction of the bullet (in radians).
	 * @param aVelocity Velocity of the bullet (tiles/round).
	 * @param aShooter The bot that shot this bullet.
	 */
	CBulletInfo(int aBulletType, float aXPos, float aYPos, float aDirection, float aVelocity, CBotInfo *aShooter);

	/** Destructor. */
	virtual ~CBulletInfo();

	/** Increase frag counter. @param aAddFrag True to increase, false to reduce counter. */
	void changeFragNum(bool aAddFrag);

protected:
	/**
	 * Collision handling.
	 * @param aDamage Damage caused by collision.
	 * @return False if object has died (never for bullets).
	 */
	bool handleCollision(int aDamage);

	/** Getter for damage caused in collision. */
	int getDamage() const;

private:
	CBotInfo *mShooter;
};

#endif // BULLET_H
