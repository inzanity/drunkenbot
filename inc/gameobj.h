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

	/** Getter for animation timer. May be used by CGraphicsEngine. */
	float animationTimer();

protected:
	/** X-coordinates of the object. */
	float mXPos;
	/** Y-coordinates of the object. */
	float mYPos;
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
	 * Move object according to its velocity and direction.
	 * @param aTimeFactor Time interval to move object.
	 */
	void move(float aTimeFactor);

	/**
	 * Check collisions and calculate time factor for moving.
	 * Time factor is 1.0, if no collisions are detected. In a case of collision
	 * both objects are handled using handleCollision().
	 * @param aTileMap Tilemap to detect collisions. Bots can collide with walls and obstacles,
	 *        while bullets can collide only with walls.
	 * @param aBots List of all bots.
	 */
	void chkCollision(char **aTileMap, CBotInfo **aBots);

	/** Collision handling.  */
	virtual void handleCollision(int aType) = 0;

	/** Velocity of the moving game object. Moving direction depends on the orientation of the object. */
	float mVelocity;

private:
	float mMovingTimeFactor;
};

/*
(v1x^2 + v2x^2)*t^2 + (v1x - v2x)(x1x - x2x)t + x1x^2 + x2x^2 +
(v1y^2 + v2y^2)*t^2 + (v1y - v2y)(x1y - x2y)t + x1y^2 + x2y^2 = (r1 + r2)^2
(v1x^2 + v2x^2 + v1y^2 + v2y^2) * t^2 + ((v1x - v2x)(x1x - x2x) + (v1y - v2y)(x1y - x2y)t + x1y^2 + x2y^2 - (r1 + r2)^2 = 0

t = ((v2x - v1x)(x1x - x2x) + (v2y - v1y)(x1x - x2x) ± v(((v1x - v2x)(x1x - x2x) + (v1y - v2y)(x1x - x2x))^2 - 4(v1x^2 + v2x^2 + v1y^2 + v2y^2)(x1y^2 + x2y^2 - (r1 + r2)^2)))/2(v1x^2 + v2x^2 + v1y^2 + v2y^2)
*/

#endif // GAMEOBJ_H
