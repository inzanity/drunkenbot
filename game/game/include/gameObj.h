/** @file gameObj.h
 * Definition of basic GameObject classes.
 * Most of the game objects are visible, but all interactive objects,
 * such as pieces of AI, should be derived from basic GameObject classes.
 * This availables general interaction and data management.
 *
 * @todo GameObject internalization and externalization.
 *
 * @author Japa
 * @version 0.1
 */

#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <istream>
#include <ostream>
#include <d3dx9math.h>
#include "animation.h"

using std::istream;
using std::ostream;

class CMessage;

/**
 * Secure pointer class for game objects.
 * Every GameObject has an unique id number, and index in used data structure.
 * GameObjects are stored using indices instead of pointers to avoid accessing
 * deleted/moved objects. @see MIndexList
 */
class CGameObjPtr
{
public:
	/** Constructor for NULL pointer. */
	CGameObjPtr();
	/** Constructor. @param aID Unique id @param aIndex Index */
	CGameObjPtr(uint16 aId, uint16 aIndex);
	/** Setter for the pointer. @param aID Unique id @param aIndex Index */
	void setPtr(uint16 aId, uint16 aIndex);
	/** Set pointer to NULL. */
	void setPtrToNULL();
	/** Getter for MGameObj ponter. @return Pointer to the valid MGameObj, or NULL if object */
	class MGameObj *ptr();
	/**	Getter for index. @return Index, where this object is stored */
	uint16 index() const;
	/**	Getter for id. @return Objects unique id */
	uint16 id() const;
private:
	uint16 mIndex, mId;
};

/**
 * General GameObject interface.
 */
class MGameObj
{
public:
	/**
	 * Constructor with id and index.
	 * @param aObjPtr Object pointer containing id and index for the object
	 */
	MGameObj(CGameObjPtr aObjPtr);
	/**	Constructor to read object from the stream. @param aStream Stream to read object from */
	MGameObj(istream &aStream);
	virtual ~MGameObj();
	/**
	 * Externalizes all data into a stream.
	 * Overloaded methods should call parents externalize() before externalization class specific data.
	 * @param aStream Destination Stream to write data
	 */
	virtual void externalize(ostream &aStream);
	/**	Getter for object pointer. @return Secure pointer for this game object */
	CGameObjPtr objectPtr();
	/**
	 * Message handler.
	 * Messages are used with AI and interaction between objects instead of polling.
	 * @param aMsg Information of received message
	 */
	virtual void handleMessage(CMessage *aMsg) = 0;
protected:
	CGameObjPtr mObjectPtr;
};

/**
 * Objects to be checked for collisions.
 * Collisions are tested using radius and collision rectangles.
 */
class MColliding
{
public:
	virtual ~MColliding();
	
	/** Check collision with another MColliding object. @param aObj Object to perform collision checks with */
	void checkCollision(MColliding *aObj);
	/** Handle detected collision or other interaction with another object. @param aObj Target of the collision */
	virtual	void handleCollision(const MGameObj *aObj) = 0;
	/** Get objects position. Usually center of the object. @return Objects position. */
	virtual const D3DXVECTOR3 *pos() const = 0;
	/** Get squared radius for collision checks. @return Radius ^ 2. */
	virtual float radiusSqr() const = 0;
	/** Get bounding box for collision checks. @return Collision box. */
	virtual const TBox *boundingBox() const = 0;
};

/**
 * Class for visible objects.
 * All drawable objects can be checked for collisions.
 * Objects are first transformed and then rendered using current Animation.
 */
class CDrawable : public MGameObj, public MColliding
{
public:
	/**
	 * Constructor with all required parameters. @see MGameObj
	 * @param aObjPtr Object pointer containing id and index for the object
	 * @param aAnim Animation for drawing. Null Animation disables drawing
	 * @param aPos Initial position of the object
	 * @param aRos Initial rotation angle of the object
	 */
	CDrawable(CGameObjPtr aObjPtr, MAnimation *aAnim,
			  float aAnimSpeed, const D3DXVECTOR3 *aPos, const D3DXQUATERNION *aOrientation);
	/**	Basic constructor with id and index. @see MGameObj */
	CDrawable(CGameObjPtr aObjPtr);
	/**	Constructor to read object from the stream. @see MGameObj */
	CDrawable(istream &aStream);
	virtual ~CDrawable();
	/**
	 * Externalizes data into a stream.
	 * Overloaded methods should call parents externalize() before externalization class specific data.
	 * @param aStream Destination Stream to write data
	 */
	virtual void externalize(ostream &aStream);
	/** Empty Message handler. @param aMsg Information of received message */
	virtual void handleMessage(CMessage *aMsg);
	/** Empty collision handler. @param aObj Target of the collision */
	virtual void handleCollision(const MGameObj *aObj);

	/** Draw object. @param aTimeFactor Elapsed time */
	virtual void draw(uint32 aTimeFactor);
	/** Transform object before rendering. @param aTimeFactor Elapsed time */
	virtual void update(uint32 aTimeFactor);
	/** Sets current Animation. @param aAnim New Animation to use @param aAnimSpeed Speed of the animation */
	void setAnimation(MAnimation *aAnim, float aAnimSpeed = 1);
	/** Get Animation playing speed. @return Animation speed */
	float animSpeed();
	/** Set Animation playing speed. @param Animation speed */
	void setAnimSpeed(float aAnimSpeed);

	/** Set objects position. @param New position */
	void setPos(const D3DXVECTOR3 *aPos);
	/** Set objects speed. @param New speed */
	void setSpeed(const D3DXVECTOR3 *aSpeed);
	/** Set objects orientation. @param New orientation */
	void setOrientation(const D3DXQUATERNION *aOrientation);
	/** Set objects rotation speed. @param New rotation speed */
	void setRotSpeed(const D3DXQUATERNION *aRotSpeed);
	/** Get objects position. @return Current position */
	const D3DXVECTOR3 *pos () const;
	/** Get objects speed. @return Current speed */
	const D3DXVECTOR3 * speed() const;
	/** Get objects orientation. @return Current orientation */
	const D3DXQUATERNION *orientation () const;
	/** Get objects rotation speed. @return Current rotation speed */
	const D3DXQUATERNION *rotSpeed() const;
	/**
	 * Get squared radius for collision checks.
	 * Radius of the current animation is used by default. @see MColliding
	 * @return Radius ^ 2
	 */
	virtual float radiusSqr() const;
	/**
	 * Get collision boxes for collision checks.
	 * Dimensions of the current animation are used by default. @see MColliding
	 * @return Collision box
	 */
	virtual const TBox *boundingBox() const;
protected:
	uint32			mAnimTime;
	float			mAnimSpeed;
	MAnimation *	mAnimation;
	D3DXVECTOR3		mPos, mSpeed;
	D3DXQUATERNION	mOrientation, mRotSpeed;
};

#endif // GAMEOBJ_H
