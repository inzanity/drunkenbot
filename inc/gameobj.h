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
class CTilemap;

/** Struct for 2D coordinates. */
struct TPosition
{
	/** X-coordinates */
	float mX;
	/** Y-coordinates */
	float mY;
};

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

	/** Getter for x-coordinates of the object. */
	float xPos();
	
	/** Getter for y-coordinates of the object. */
	float yPos();

	/** Getter for the radius. Used with drawing and collision detection. */
	float radius();

	/** Getter for the orientation of the object. */
	float direction();

	/** Getter for the type of the object. @see mType. */
	int type();

	/** Getter for animation timer. May be used by IGraphicsEngine. */
	float animationTimer();

protected:
	/** Coordinates of the object. */
	TPosition mPos;
	/** Radius of the game object. */
	float mRadius;
	/** Orientation of the game object. With moving objects, orientation is used as a moving direction. */
	float mDirection;
	/** Type of the game object. @todo Game object type specification. */
	int mType;
	/** Timer for animations. Animation is played during 1 time units. */
	float mAnimationTimer;
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

	/** Getter for the velocity. */
	float velocity();

protected:
	/**
	 * Moves object according to its velocity and direction.
	 * @param aTimeFactor Time interval to move object.
	 */
	void move(float aTimeFactor);

	/**
	 * Checks collisions and calculates time factor for moving.
	 * Time factor is 1.0, if no collisions are detected. In a case of collision
	 * both objects are handled using handleCollision().
	 * @param aTilemap Tilemap to detect collisions.
	 * @param aBots List of all bots.
	 * @param aCollisionWithObstacles True to enable collisions with obstacles.
	 *        Bots can collide with walls and obstacles, while bullets can collide only with walls.
	 */
	void chkCollision(const char **aTilemap, CBotInfo **aBots, bool aCollisionWithObstacles);

	/**
	 * Raycaster to scan visible tiles. Can be used to check collisions with walls.
	 * @param aSrcTilemap Full tilemap to read tiles for scanning.
	 * @param aAngle Moving direction for casted ray.
	 * @param aDstTilemap Tilemap to write visible tiles. NULL to disable vision scanning
	 *		  (with collision detection).
	 * @return Position of the detected collision.
	 */
	TPosition scanTilemap(const char **aSrcTilemap, float aAngle, CTilemap *aDstTilemap);

	/** Collision handling.  */
	virtual void handleCollision(int aType) = 0;

	/** Velocity of the moving game object. Moving direction depends on the orientation of the object. */
	float mVelocity;

private:
	float mMovingTimeFactor;
};

#endif // GAMEOBJ_H
