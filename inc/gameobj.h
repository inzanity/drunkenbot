/** @file gameobj.h
 * Definition of CGameObj and CMovingGameObj classes.
 * All visible objects in game excluding tilemap are considered as game objects.
 * Drawing game objects is done using CGameObj interface,
 * while CMovingGameObj is responsible of moving objects and collision detection.
 *
 * @author Japa
 * @version 1.0
 */

#ifndef GAMEOBJ_H
#define GAMEOBJ_H

class CBotInfo;

/**
 * Base class for all game objects.
 * Common attributes for locating and drawing objects.
 */
class CGameObj
{
public:
	/** Constructor. @param aType Type of the game object. @see mType. */
	CGameObj(int aType);

	/** Destructor. */
	virtual ~CGameObj();

	/** Setter for the position of the object. @param aXPos X-coordinates. @param aYPos Y-coordinates. */
	void setPos(float aXPos, float aYPos);

	/** Getter for x-coordinates of the object. */
	float xPos();
	
	/** Getter for y-coordinates of the object. */
	float yPos();

	/** Getter for the radius. Used with drawing and collision detection. */
	float radius();

	/** Getter for the orientation of the object. */
	float angle();

	/** Getter for the type of the object. @see mType. */
	int type();

protected:
	/** X-coordinates of the object. */
	float mXPos;
	/** Y-coordinates of the object. */
	float mYPos;
	/** Radius of the game object. */
	float mRadius;
	/** Orientation of the game object. With moving objects, angle is used as a moving direction. */
	float mAngle;
	/** Type of the game object. @todo Game object type specification. */
	int mType;
};

/**
 * Base class for all moving game objects.
 * Includes speed and collision detection.
 */
class CMovingGameObj : public CGameObj
{
public:
	/** Constructor. @param aType Type of the object. */
	CMovingGameObj(int aType);

	/** Destructor. */
	virtual ~CMovingGameObj();

	/** Setter for the speed. */
	void setSpeed(float aVelocity, float aAngle);

	/** Getter for the velocity. */
	float velocity();

	/**
	 * Moves object according to the speed. In a case of collision, both objects are stopped and
	 * affected by the collision.
	 * @param aTileMap Tilemap to detect collisions. Bots can collide with walls and obstacles,
	 * while bullets can collade only with walls.
	 * @param aBots List of all bots.
	 * @return true if moving was succesful, false in a case of collision.
	 */
	bool move(char **aTileMap, CBotInfo *aBots);

protected:
	/** Velocity of the moving game object. Moving direction depends on the orientation of the object. */
	float mVelocity;
};


#endif // GAMEOBJ_H
