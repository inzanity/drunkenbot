/**
 * @file botinfo.h
 * Definition of bot info classes CVisibleBotInfo and CBotInfo.
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

#include "weapon.h"

/**
 * Bot actions. Every action is a combination of these.
 * See CBotAI::checkAction() for detailed information about legal actions.
 */
enum TBotAction {EActionNone = 0, EActionMove = 1, EActionTurn = 2, EActionShoot = 4,
				 EActionBunker = 8, EActionPickWeapon = 16, EActionDropWeapon = 32};

/**
 * Public class for general bot information.
 * When bots see other bots, they will get this information of it.
 */
class CVisibleBotInfo : public CMovingGameObj
{
public:
	/** Default constructor */
	CVisibleBotInfo();
	/**
	 * Copy constructor.
	 * Used when giving info to bots, changes coordinates from
	 * absolute to relative.
	 *
	 * @param aBotInfo The private info of the bot
	 * @param aXPos Current X coordinate of the bot
	 * @param aYPos Current Y coordinate of the bot
	 */
	CVisibleBotInfo(CVisibleBotInfo *aBotInfo, float aXPos, float aYPos);

	/** Destructor. */
	virtual ~CVisibleBotInfo();

	/**
	 * Checks whether the bot is hostile.
	 * @return True if seen bot is in other team, false if in own.
	 */
	bool enemy();

	/** Collision handling. @param aDamage Damage caused by collision. */
	void handleCollision(int aDamage);
};

/** Private class for general bot information. */
class CBotInfo : public CVisibleBotInfo
{
public:
	/** Default constructor. */
	CBotInfo();

	/** Destructor. */
	virtual ~CBotInfo();

	/** Getter for current weapon */
	const CWeapon *weapon();

	/**
	 * Getter for action being executed.
	 * @return Unfinished action. actionDelay() can be used to get remaining time to finish action.
	 */
	TBotAction currentAction();

	/** Getter for the remaining time to finish current action. @return Turns left before finished current action. */
	char actionDelay();

	/** Getter for bunkering mode. @return True if bot is bunkered. */
	bool bunkered();

protected:
	/** Current weapon. */
	CWeapon *mWeapon;

	/** Current action. */
	TBotAction mBotAction;

	/** Turns remaining before done current action. */
	char mActionDelay;

	/** Is the robot bunkered. */
	bool mBunkered;
};

#endif // BOTINFO_H
