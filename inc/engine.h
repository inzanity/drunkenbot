/** @file engine.h
 * Definition of CGameEngine.
 * Engine is an interface between the game and the program entry point.
 * This way game remains platform independent. CGraphicsEngine must be
 * implemented for used platform in order to draw game. However,
 * game can be runned without CGraphicsEngine for statistical purposes.
 *
 * @author Japa
 * @version 1.0
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "bot.h"
#include "graphicsengine.h"
#include "bullet.h"
#include <list>

using std::list;

/**
 * Game engine includes all higher level logic for the game.
 * Allocates required number of bots, stores team information and game options.
 */
class CGameEngine
{
public:
	/**
	 * Constructor to load bots from stream.
	 * Data is read from streams in order team info, weapons, map.
	 * @param aWeapons Stream to read available weapon types.
	 * @param aMap Stream to read tilemap.
	 * @param aTeamInfo Stream to load teams and bots, or NULL to load all available bots (ffa).
	 */
	CGameEngine(istream *aWeapons, istream *aMap, istream *aTeamInfo = NULL);

	/**
	 * Setter for graphics engine.
	 * @param aGraphicsEngine Graphics engine used to draw game. NULL to disables drawing.
	 */
	void setGraphicsEngine(CGraphicsEngine *aGraphicsEngine);

	/** Game loop. @return False if game has ended. */
	bool loop();

	/**
	 * Draw game using current graphics engine.
	 * @param aTimeInterval Time interval to move and animate objects.
	 * For smooth animation, game should be drawn using constant time interval,
	 * with total time interval of 1 for every loop.
	 * @param aBotIndex Index of the bot used for drawing, or -1 to use general drawing.
	 */
	void draw(float aTimeInterval, int aBotIndex);

	/**
	 * Getter for results.
	 * Results are sorted by scores. First 5 characters of each line are reserved for scores.
	 * @param aTeamResults True to get team results, false to get results for every bot.
	 * @param aResults Pointer to previously fetched scores or NULL.
	 * @return Sorted table of strings including scores and names.
	 */
	char **getResults(bool aTeamResults, char **aResults) const;

	/**
	 * Resets scores and initializes new game.
	 * @param aMap Stream to load new tilemap, NULL to restart with old map.
	 */
	void restart(istream *aMap = NULL);

	/** Setter for frag limit. */
	void setFragLimit(int aFragLimit);

private:
	CGraphicsEngine *mGfxEngine;
	list<CWeaponInfo *> mWeaponList;
	list<CBulletInfo *> mBulletList;
	list<TVector> mVoiceList;
	CWeapon **mWeaponTypes;
	CBot **mBots;
	char **mTilemap;
	int mBotNum;
	int mWeaponNum;
	int mMapWidth;
	int mMapHeight;
	int mFragLimit;
	int mTeamNum;
	char **mTeamNames;
	bool mFriendlyFire;
};

#endif // ENGINE_H
