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
 * Bots are controlled using CBotAI, and all data is stored in CBotInfo.
 * 
 */
class CBot
{
public:
	/** Constructor. @param aDllName Name of the .dll file to load <code>CBotAI</code>. */
	CBot(const char *aDllName);

	/** */

private:
	CBotAI *mBotAI;
	CBotInfo mBotInfo;
	char *mDllName;
}


#endif // BOT_H
