/** @file bot.h
 * Definition of CBot.
 * Bot information from engines point of view. 
 *
 * @author Japa
 * @version 1.0
 */

#ifndef BOT_H
#define BOT_H

#include "botai.h"
#include "graphicsengine.h"

/**
 * Class to store all bots information.
 * Bots are controlled using CBotAI. AI is always reloaded during spawning.
 */
class CBot : public CBotInfo
{
public:
	/** Constructor. @param aDllName Name of the .dll file to load CBotAI. */
	CBot(const char *aDllName);

	/**
	 * Reloads AI from dll, creates and initializes new bot.
	 * @param aTileMap Tilemap for locating new bot.
	 * @param aGameObjects Bot should not be located near these objects. */
	void spawn(const char **aTileMap, const CGameObj *aGameObjects);

	/**
	 * Checks and handles collisions. See CMovingGameObj::chkCollision().
	 * @param aTileMap Tilemap to detect collisions. Bots can collide with walls and obstacles.
	 * @param aBots List of all bots.
	 */
	void chkCollision(char **aTileMap, CBotInfo **aBots);

	/** Updates bots actions using CBotAI. Performs error checking for invalid actions. */
	void think();

	/**
	 * Moves bot and prepares it for drawing.
	 * @param aTimeFactor Elapsed time between 0 and 1.
	 */
	void update(float aTimeFactor);

private:
	void loadAI();

	CBotAI *mBotAI;
	char *mDllName;
	HMODULE mDllHandle;
};

#endif // BOT_H
