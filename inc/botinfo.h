/** @file botinfo.h
 * Definition of bot info classes
 * This header specifies internal and external data structures for the
 * game objectish information of the robots.
 * @see CBot
 *
 * @todo Foo
 *
 * @author inz
 * @version 0.1
 */

#ifndef BOTINFO_H
#define BOTINFO_H

/**
 * Public class for general bot information
 * When bots see other bots, they will get this information of it.
 */
class CVisibleBotInfo : public CMovingGameObj
{
public:
	/** Default constructor */
	CVisibleBotInfo();
	/**
	 * Copy constructor
	 * Will be used when giving info to bots, changes coordinates from
	 * absolute to relative
	 *
	 * @param aBotInfo The private info of the bot
	 * @param aXPos Current X coordinate of the bot
	 * @param aYPos Current Y coordinate of the bot
	 */
	CVisibleBotInfo(CVisibleBotInfo *aBotInfo, float aXPos, float aYPos);
};

/**
 * Private class for general bot information
 */
class CBotInfo : public CVisibleBotInfo
{
public:
	/** Default constructor */
	CBotInfo();
};

#endif // BOTINFO_H
