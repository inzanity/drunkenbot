/** @file bullet.h
 * Definition of bullet info class CBulletInfo.
 *
 * @author inz
 * @version 1.0
 */

#ifndef BULLET_H
#define BULLET_H

#include "gameobj.h"

/**
 * Class for bullets.
 * Data structure the engine uses for storing information about bullets.
 * @note AI classes can only see bullets as moving game objects.
 */
class CBulletInfo : public CMovingGameObj
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
	CBulletInfo(float aXPos, float aYPos, float aDirection, CBotInfo *aShooter);

	/** Destructor. */
	virtual ~CBulletInfo();

	/** Increase frag counter. @param aAddFrag True to increase, false to reduce counter. */
	void changeFragNum(bool aAddFrag);

	/** Checks if bullet is still alive and updates radius for explosions. @return True if bullet is alive. */
	bool alive();

	/** Update bullet according to its new state and position. @return True if bullet is alive. */
	bool update();

protected:
	/**
	 * Collision handling.
	 * @param aDamage Damage caused by collision.
	 * @return False if object has died (never for bullets).
	 */
	bool handleCollision(char aDamage);

	/** Getter for damage caused in collision. */
	char getDamage() const;

private:
	CBotInfo *mShooter;
	float mExplosionSpeed;
	float mExplosionRadius;
	float mDamage;
};

#endif // BULLET_H
