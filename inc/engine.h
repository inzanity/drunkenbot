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

/**
 * Game engine includes all higher level logic for the game.
 * Allocates required number of bots, stores team information and game options.
 */
class CGameEngine
{
public:
	/**
	 * Default constructor to load all available bots. Game mode is FFA.
	 * @param aMap Stream to read tilemap from.
	 */
	CGameEngine(istream &aMap);

	/**
	 * Constructor to load bots from stream.
	 * @param aStream Stream to load teams and bots.
	 * @param aMap Stream to read tilemap.
	 */
	CGameEngine(istream &aTeamInfo, istream &aMap);

	/**
	 * Setter for graphics engine.
	 * @param aGraphicsEngine Graphics engine used to draw game, NULL to disable drawing.
	 */
	void setGraphicsEngine(CGraphicsEngine *aGraphicsEngine);

	/** Game loop. @return False if game has ended. */
	bool loop();

	/**
	 * Getter for results.
	 * Results are sorted by scores. First 5 characters of each line are reserved for scores.
	 * @param aTeamResults True to get team results, false to get results for every bot.
	 * @return Sorted table of strings including scores and names.
	 */
	char **getResults(bool aTeamResults);

	/**
	 * Resets scores and initializes new game.
	 * @param aMap Stream to load new tilemap, NULL to restart with old map.
	 */
	void restart(istream *aMap = NULL);

	/** Setter for frag limit. */
	void setFragLimit(int aFragLimit);

private:
	CGraphicsEngine *mGfxEngine;
	CBot *mBots;
	char **mTileMap;
	int mFragLimit;
	int mTeamNum;
	char **mTeamNames;
	bool mFriendlyFire;
};

#endif // ENGINE_H
