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

using std::list;

#ifdef WIN32
#	include <windows.h>
#else
#	include <dlfcn.h>
#	define HMODULE void *
#endif

/**
 * Class to store all bots information.
 * Bots are controlled using CBotAI. AI is always reloaded during spawning.
 */
class CBot : public CBotInfo
{
public:
	/** Constructor. @param aDllName Name of the .dll file to load CBotAI. @param aTeamNumber Team number. */
	CBot(const char *aDllName, int aTeamNumber);

	/**
	 * Reloads AI from dll, creates and initializes new bot.
	 * @param aTilemap Tilemap for locating new bot.
	 * @param aWidth Width of the tilemap. @aHeight Height of the tilemap.
	 * @param aGameObjects Bot should not be located near these objects.
	 * @param aWeapon Default weapon.
	 */
	void spawn(const char **aTilemap, int aWidth, int aHeight, const CGameObj **aGameObjects, const CWeapon *aWeapon);

	/**
	 * Updates visible objects and bots actions using CBotAI.
	 * @param aTilemap The whole tilemap.
	 * @param aWidth Width of the tilemap. @aHeight Height of the tilemap.
	 * @param aBots Array containing all bots.
	 * @param aBulletList List of all bullets in the field.
	 * @param aWeaponList List of all weapons in the field.
	 * @param aVoices List of all voice sources.
	 */
	void think(const char **aTilemap, int aWidth, int aHeight, CVisibleBotInfo **aBots,
			   list<CBulletInfo *> *aBulletList, list<CWeaponInfo *> *aWeaponList, list<TVector> *aVoices);

	/**
	 * Does error checking and performs valid actions.
	 * @param aBulletList List to add new bullets in case of <code>EActionShoot</code>.
	 * @param aVoices List to add new sound sources in case of <code>EActionShoot</code>.
	 */
	void performActions(list<CBulletInfo *> *aBulletList, list<TVector> *aVoices);

	/** Update bot according to its new state and position. @return True if bot is alive. */
	bool update();

	/** Getter for AI. May be used by CGraphicsEngine. @return Const pointer to the AI object. */
	const CBotAI *botAI() const;

	/** Getter for x-coordinates of the spawning location. May be used by CGraphicsEngine. */
	float spawningXPos() const;
	
	/** Getter for y-coordinates of the spawning location. May be used by CGraphicsEngine. */
	float spawningYPos() const;


private:
	/** Load AI class from file (mDllName). */
	void loadAI();

	/**
	 * Raycaster to scan visible tiles.
	 * @param aTilemap Full tilemap to read tiles for scanning.
	 * @param aDAngle Angle difference between casted rays.
	 */
	void scanTilemap(const char **aTilemap, float aDAngle) const;

	CBotAI *mBotAI;
	char *mDllName;
	HMODULE mDllHandle;
	int mFrags;
	TVector mSpawningPos;
};

#endif // BOT_H
