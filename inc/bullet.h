/** @file bullet.h
 * Definition of bullet info classes
 * This header specifies the public (for the AI:s) and private
 * (for the engine) data structures for bullets flying in da air
 *
 * @todo Everything
 *
 * @author inz
 * @version 0.2
 */

#ifndef BULLET_H
#define BULLET_H

/**
 * Public class for weapons
 * When bots see a weapon, this is the info they get of it
 */
class CVisibleBulletInfo : public CMovingGameObj
{
public:
	/** Default constructor */
	CVisibleBulletInfo();
	/** Copy constructor
	 * This will only be called with data of type CBulletInfo,
	 * and will change coordinates from absolute to relative,
	 * thus needing current location of the bot
	 *
	 * @param aBullet The private info of the bullet
	 * @param aXPos Current X coordinate of the bot
	 * @param aYPos Current Y coordinate of the bot
	 */
	CVisibleBulletInfo(const CVisibleBulletInfo *aBullet, float aXPos, float aYPos);
	/** Getter for the weapon type */
	int bulletType();

private:
	int mBulletType;
};

/**
 * Private class for bullets
 * Data structure the engine uses for storing information about bullets
 */
class CBulletInfo : public CVisibleBulletInfo
{
	/** Default constructor */
	CBulletInfo();
	/** Constructor with type and coordinates
	 * Creates a weapon of given type to given coordinates.
	 * @param aBulletType Type of bullet
	 * @param aXPos X position for the bullet
	 * @param aYPos Y position for the bullet
	 * @param aDirection Direction of the bullet (in radians)
	 * @param aVelocity Velocity of the bullet (tiles/round)
	 */
	CBulletInfo(int aBulletType, float aXPos, float aYPos, float aDirection, float aVelocity);
};

#endif // BULLET_H
