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

#define PI 3.14159265358979f

#define KTeamMask 0xF00

class CBotInfo;
class CTilemap;

enum TObjectType{EObjectBot, EObjectWeapon, EObjectBullet};

/** Struct for 2D coordinates. */
struct TVector
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

	/** Copy constructor. @param aGameObj Source game object to copy data from. */
	CGameObj(const CGameObj *aGameObj);

	/** Destructor. */
	virtual ~CGameObj();

	/** Getter for x-coordinates of the object. */
	float xPos() const;
	
	/** Getter for y-coordinates of the object. */
	float yPos() const;

	/** Getter for the radius. Used with drawing and collision detection. */
	float radius() const;

	/** Getter for the orientation of the object. */
	float orientation() const;

	/** Getter for the type of the object. @see mType. */
	int type() const;

	/** Getter for animation timer. May be used by CGraphicsEngine. */
	float animationTimer() const;

protected:
	/** Coordinates of the object. */
	TVector mPos;
	/** Radius of the game object. */
	float mRadius;
	/** Orientation angle of the game object. */
	float mOrientation;
	/** 
	 * Type of the game object.
	 * The type id consist of 3 four bit fields, team info, object specific id and object type id.
	 * Thus bitmasks are 0xf00 for team info, 0xf0 for object data and 0xf for object type.
	 * Currently there are 3 object types bot, weapon and bullet, with values 1, 2 and 3 in respective order.
	 */
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

	/** Copy constructor. @param aGameObj Source game object to copy data from. */
	CMovingGameObj(const CMovingGameObj *aGameObj);

	/** Destructor. */
	virtual ~CMovingGameObj();

	/** Getter for the velocity. */
	float velocity() const;

	/** Getter for the moving direction. */
	float movingDirection() const;

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
	 * @return True if no collisions were detected.
	 */
	bool chkCollision(const char **aTilemap, CBotInfo **aBots, bool aCollisionWithObstacles);

	/** Increase frag counter of the responsible CBot. @param aAddFrag True to increase, false to reduce counter. */
	void changeFragNum(bool aAddFrag);

	/** Update object according to its new state and position. @return True if object is alive. */
	virtual bool update();

protected:
	/** Collision handling. @param aDamage Damage caused by collision. @return False if object (CBot) has died. */
	virtual bool handleCollision(char aDamage);

	/** Getter for damage caused by this object in collisions. @return Caused damage. */
	virtual char getDamage() const;

	/** Velocity of the game object. */
	float mVelocity;

	/** Moving direction of the game object. */
	float mMovingDirection;

	/** Flag to determine if collision is detected. */
	bool mCollisionDetected;

	/**
	 * Find next edge from tilemap. <code>aPos + time * aSpeed</code> is always in the next tile.
	 * With collision detection, 2 * mEpsilon should be decreased from time.
	 * @param aPos Initial position of the "casted ray".
	 * @param aSpeed Speed of the "casted ray". In normal case, this is <code>velocity * [cos direction, sin direction]</code>.
	 * @return Time difference to the next edge. mEpsilon is added to time to avoid problems with precision.
	 */
	static float getNextEdge(TVector aPos, TVector aSpeed);

private:
	static const float mEpsilon;
	float mMovingTimeFactor;
};

#endif // GAMEOBJ_H
