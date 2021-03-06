/**
 * @file botai.h
 * Definition of CBotAI base class.
 * Includes everything needed to build AI for the bot.
 * In order to load AI from dll, <code>getBotAI()</code> must be implemented.
 *
 * @author Japa
 * @version 1.0
 */

#ifndef BOTAI_H
#define BOTAI_H

#include <list>
#include "tilemap.h"
#include "botinfo.h"
#include "weaponinfo.h"
#include "bullet.h"

using std::list;

#define KFov				(PI / 2.f)
#define KMoveSpeedForward	(.25f)
#define KMoveSpeedBackwards	(.1f)
#define KTurnSpeed			(.1f * PI)
#define KInaccuracy			(PI / 36.f)

/**
 * Base class for external AI class.
 * Bots are controlled using think() method once in every frame.
 * Action flags are used to define bot movement and other actions (see TBotAction).
 * @note
 * All provided information about this bot or surrounding world is "read only".
 * Changing anything else than action flags and moving/shooting direction will have
 * no effects.
 *
 * @include bot.txt
 */
class CBotAI
{
public:
	/**
	 * Bot moving directions. Direction can be only one of these.
	 * @see move().
	 */
	enum TBotMovingDir {EMoveForward, EMoveBackwards, EMoveLeft, EMoveRight};

	/**
	 * Bot turning directions. There are only two possible directions. No combinations are allowed.
	 * @see turn().
	 */
	enum TBotTurningDir {ETurnLeft, ETurnRight};

	/** Default constructor. */
	CBotAI();

	/** Destructor. */
	virtual ~CBotAI();

	/**
	 * Pure virtual think method.
	 * All bots must be provided with AI by implementing this method.
	 */
	virtual void think() = 0;

	/**
	 * Initializing method to configure attributes of the bot.
	 * Bot speed, armour, aiming and size (smallness) can be adjusted with different factor, from 75% up to 125%.
	 * However, total sum of factors must be 400%. All attributes are 100% by default.
	 * @param aSpeed Weight factor of the speed percentage. Moving and turning speeds are both affected.
	 * @param aArmour Weight factor of the armour percentage.
	 * @param aAiming Weight factor of the aiming percentage. 
	 * @param aSize Inverted weight factor of the size percentage. Note that smaller factor equals bigger bot.
	 *              That is, real size factor is <code>200% - aSize</code>.
	 */
	virtual void init(int &aSpeed, int &aArmour, int &aAiming, int &aSize);

	/** Reset action to <code>EActionNone</code>. */
	void resetAction();

	/**
	 * Move bot to the given direction.
	 * If <code>EActionBunker</code>, <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> is defined,
	 * it will be cancelled. If bot has been bunkered, it can not move before unbunkering.
	 * @param aDir Moving direction. If direction is invalid, no action is performed.
	 * @return True if action was legal.
	 */
	bool move(TBotMovingDir aDir);

	/**
	 * Turn bot to the given direction.
	 * If <code>EActionBunker</code>, <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> is defined,
	 * it will be cancelled.
	 * @param aDir Turning direction. If direction is invalid, no action is performed.
	 * @return True if action was legal.
	 */
	bool turn(TBotTurningDir aDir);

	/**
	 * Shoot to the given direction with the current weapon.
	 * If <code>EActionBunker</code>, <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> is defined,
	 * it will be cancelled. Moving at the same time will decrease precision.
	 * Reloading time depends on weapon. If bot is still reloading the weapon, no action is performed.
	 * @param aDir Shooting direction in radians. If direction is invalid (|dir| > PI/8), no action is performed.
	 * @return True if action was legal.
	 */
	bool shoot(float aDir);

	/**
	 * Bunker/unbunker bot. Action duration 4 rounds.
	 * All other actions will be cancelled.
	 * @return True if action was legal.
	 */
	bool bunker();

	/**
	 * Pick up a weapon. Action duration 2 rounds.
	 * All other actions will be cancelled.
	 * @return True if action was legal.
	 */
	bool pickWeapon();

	/**
	 * Drop a weapon. Action duration 1 round.
	 * All other actions will be cancelled.
	 * @return True if action was legal.
	 */
	bool dropWeapon();

	/** Cancel action. @param aAction Action to be cancelled. */
	void cancelAction(TBotAction aAction);

	/** Getter for action. @return Action to be performed next. */
	int action() const;

	/**
	 * Check validity of the action.
	 * Only one of <code>EActionShoot</code>, <code>EActionBunker</code>,
	 * <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> can be defined at the time.
	 * If bot has been bunkered, <code>EActionMove</code> can not be defined.
	 * Action can not be changed, before it has been completed. See CBotInfo::actionDelay().
	 * @return True for legal actions, false for illegal.
	 */
	bool checkAction() const;

	/** Getter for moving direction. */
	TBotMovingDir movingDir();

	/** Getter for turning direction. */
	TBotTurningDir turningDir();

	/** Getter for shooting direction. */
	float shootingDir();

	/** Map of the surrounding world. Bot is placed in the middle of the map (tile 0, 0). */
	CTilemap *mTilemap;

	/** List of all visible bots. */
	list<CVisibleBotInfo *> mBots;

	/** List of all visible weapons. */
	list<CWeaponInfo *> mWeapons;

	/** List of all visible bullets. Bullets are usually fast, but may be detected and dodged. */
	list<CMovingGameObj *> mBullets;

	/** List of sources causing noise. Only angle of incidence can be detected. */
	list<float> mSourcesOfNoise;

	/** Information about this bot. */
	CBotInfo *mData;

private:
	bool setAction(TBotAction aAction, int aExclusiveActions);

	int mAction;
	TBotMovingDir mMovingDir;
	TBotTurningDir mTurningDir;
	float mShootingDir;
};

/** Type of the AI class getter function. */
typedef CBotAI *(*TBotAIGetter)();

/** Getter function for the AI class. Must be implemented. @return Instance of the AI class. */
extern "C" CBotAI *getBotAI();


#endif // BOTAI_H
