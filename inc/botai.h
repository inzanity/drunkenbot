/**
 * @file botai.h
 * Definition of CBotAI base class.
 * Includes everything needed to build AI for the bot.
 * In order to load AI from dll, <code>getBotAI</code> must be implemented.
 *
 * @author Japa
 * @version 1.0
 */

#ifndef BOTAI_H
#define BOTAI_H

#include <list>
#include "tilemap.h"
#include "botinfo.h"
#include "weapon.h"
#include "bullet.h"

using std::list;

/**
 * Base class for external AI class.
 * Bots are controlled using <code>think</code> method once in every frame.
 * Action flags are used to define bot movement and other actions. @see TBotAction
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
	 * Bot actions. Every action is a combination of these.
	 * @see checkAction() for detailed information about legal actions.
	 */
	enum TBotAction {EActionNone = 0, EActionMove = 1, EActionShoot = 2,
					 EActionBunker = 4, EActionPickWeapon = 8, EActionDropWeapon = 16};

	/**
	 * Bot moving directions. Direction can be only one of these.
	 * @see move().
	 */
	enum TBotMovingDir {EMoveForward, EMoveBackwards, EMoveLeft, EMoveRight};

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
	 * Bot speed, armour and aiming can be adjusted with different power, from 50% up to 150%.
	 * However, total power is scaled to be 300%. All attributes are 100% by default.
	 * @param aSpeed Weight factor of the speed. Moving and turning speeds are both affected.
	 * @param aArmour Weight factor of the armour.
	 * @param aAiming Weight factor of the aiming. 
	 */
	virtual void init(int &aSpeed, int &aArmour, int &aAiming);

	/** Reset action to <code>EActionNone</code>. */
	void resetAction();

	/**
	 * Move bot to the given direction.
	 * If <code>EActionBunker</code>, <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> is defined,
	 * it will be cancelled. If bot has been bunkered, it can not move before unbunkering.
	 * @param aDir Moving direction. If direction is invalid, no action is performed.
	 */
	void move(TBotMovingDir aDir);

	/**
	 * Shoot to the given direction with the current weapon.
	 * If <code>EActionBunker</code>, <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> is defined,
	 * it will be cancelled. Moving at the same time will decrease precision.
	 * Reloading time depends on weapon. If bot is still reloading the weapon, no action is performed.
	 * @param aDir Shooting direction in radians. If direction is invalid (|dir| > PI/8), no action is performed.
	 */
	void shoot(float aDir);

	/**
	 * Bunker/unbunker bot. Action duration 4 rounds.
	 * If <code>EActionShoot</code>, <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> is defined,
	 * it will be cancelled.
	 */
	void bunker();

	/**
	 * Pick up a weapon. Action duration 2 rounds.
	 * If <code>EActionShoot</code>, <code>EActionBunker</code> or <code>EActionDropWeapon</code> is defined,
	 * it will be cancelled. To pick a weapon, it has to be close enough (distance <= 1).
	 */
	void pickWeapon();

	/**
	 * Drop a weapon. Action duration 2 rounds.
	 * If <code>EActionShoot</code>, <code>EActionBunker</code> or <code>EActionPickWeapon</code> is defined,
	 * it will be cancelled. Dropped weapon disappears permanently, and it can not be picked up again.
	 */
	void dropWeapon();

	/** Getter for action. @return Action to be performed next. */
	int action();

	/**
	 * Check validity of the action.
	 * Only one of <code>EActionShoot</code>, <code>EActionBunker</code>,
	 * <code>EActionPickWeapon</code> or <code>EActionDropWeapon</code> can be defined at the time.
	 * If bot has been bunkered, <code>EActionMove</code> can not be defined.
	 */
	bool checkAction();

	/** Map of the surrounding world. Bot is placed in the middle of the map (tile 0, 0). */
	CTileMap *mTileMap;

	/** List of all visible bots. */
	list<CVisibleBotInfo *> mBots;

	/** List of all visible weapons. */
	list<CVisibleWeaponInfo> aWeapons;

	/** List of all visible bullets. Bullets are usually fast, but may be detected and dodged. */
	list<CVisibleBulletInfo> mBullets;

	/** List of sources causing noise. Only angle of incidence can be detected. */
	list<float> mSourceOfNoise;

	/** Information about this bot. */
	CBotInfo mData;

private:
	int mAction;
	TBotMovingDir mMovingDir;
	float mShootingDir;
};

/** Type of the AI class getter function. */
typedef CBotAI *(*)() TBotAIGetter;

/** Getter function for the AI class. Must be implemented. @return Instance of the AI class. */
CBotAI *getBotAI();


#endif // BOTAI_H