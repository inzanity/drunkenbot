/** @file bot.h
 * Definition of Dippaadai.
 * Rairairai.
 *
 * @todo foo
 *
 * @author Japa
 * @version 0.1
 */

#ifndef BOT_H
#define BOT_H

#include "botai.h"

/**
 * Class to store all bot information.
 * Bots are controlled using mBotAI, and all data is stored in mBotInfo.
 * 
 */
class CBot
{
public:
	/**
	 * Default constructor.
	 */
	CBot();

private:
	CBotAI *mBotAI;
	CBotInfo mBotInfo;
}


#endif // BOT_H
