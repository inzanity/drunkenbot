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

/**
 * Class to store all bot information.
 * Bots are controlled using CBotAI. AI is always reloaded during spawning.
 */
class CBot : public CBotInfo
{
public:
	/** Constructor. @param aDllName Name of the .dll file to load CBotAI. */
	CBot(const char *aDllName);

	/**
	 * Reload AI from dll, create and initialize new bot.
	 * @param aTileMap Tilemap for locating new bot.
	 * @param aGameObjects Bot should not be located near these objects. */
	void spawn(const char **aTileMap, const CGameObj *aGameObjects);

private:
	void loadAI();

	CBotAI *mBotAI;
	char *mDllName;
	HMODULE mDllHandle;
};

#endif // BOT_H
