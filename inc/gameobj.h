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

class CVisibleBotInfo;

/**
 * Base class for all game objects.
 * Common attributes for locating and drawing objects.
 */
class CGameObj
{
public:
	/** Constructor. @param aType Type of the game object. @todo Game object type explanation. */
	CGameObj(int aType);

	/** Destructor. */
	virtual ~CGameObj();

	/** Setter for object position. @param aXPos Position */
	void setPos(float aXPos, float aYPos);

	float xPos();
	
	float yPos();

private:
	CBotAI *mBotAI;
	CBotInfo mBotInfo;
};

class CMovingGameObj : public CGameObj
{
public:
	CMovingGameObj();
	virtual ~CMovingGameObj();
};


#endif // BOT_H
