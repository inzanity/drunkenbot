/** @file bot.h
 * Definition of CBot.
 * Bot information from engines point of view. 
 *
 * @author Japa
 * @version 1.0
 */

#ifndef BOT_H
#define BOT_H

#include <list>
#include "botai.h"
#include "graphicsengine.h"

using std::list;
/*
#ifndef HMODULE
#define HMODULE void *
#endif
*/
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
	 * @param aTilemap Tilemap for locating new bot.
	 * @param aWidth Width of the tilemap. @aHeight Height of the tilemap.
	 * @param aGameObjects Bot should not be located near these objects.
	 */
	void spawn(const char **aTilemap, int aWidth, int aHeight, const CGameObj *aGameObjects);

	/**
	 * Updates visible objects and bots actions using CBotAI.
	 * @param aTilemap The whole tilemap.
	 * @param aBots Array containing all bots.
	 * @param aBulletList List of all bullets in the field.
	 * @param aWeaponList List of all weapons in the field.
	 * @param aVoices List of all voice sources.
	 */
	void think(const char **aTilemap, CVisibleBotInfo *aBots, list<CBulletInfo *> *aBulletList,
			   list<CWeaponInfo *> *aWeaponList, list<TPosition> *aVoices);

	/**
	 * Does error checking and performs valid actions.
	 * @param aBulletList List to add new bullets in case of <code>EActionShoot</code>.
	 * @param aVoices List to add new sound sources in case of <code>EActionShoot</code>.
	 */
	void performActions(list<CWeaponInfo *> *aBulletList, list<TPosition> *aVoices);

	/**
	 * Checks and handles collisions. See CMovingGameObj::chkCollision().
	 * @param aTilemap Tilemap to detect collisions. Bots can collide with walls and obstacles.
	 * @param aBots List of all bots.
	 */
	void chkCollision(const char **aTilemap, CBotInfo **aBots);

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
	int mFrags;
};

#endif // BOT_H
