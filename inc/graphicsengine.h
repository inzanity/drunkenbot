/** @file graphicsengine.h
 * Definition of IGraphicsEngine.
 * All general graphics related stuff. 
 *
 * @author Japa
 * @version 1.0
 */

#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include "gameobj.h"

/**
 * Graphics engine interface.
 * Used by CGameEngine to draw the game.
 */
class IGraphicsEngine
{
public:
	/** Destructor. */
	virtual ~IGraphicsEngine() {}

	/**
	 * Pure virtual method for drawing tilemap.
	 * @param aTileMap Tiles to draw. Two most significant bits define type of the tile. See TTileType.
	 * @param aWidth Width of the tilemap.
	 * @param aHeight Height of the tilemap.
	 */
	virtual void drawTilemap(char **aTilemap, int aWidth, int aHeight) = 0;

	/** Pure virtual method for drawing game object. */
	virtual void drawGameObj(const CGameObj *aGameObj) = 0;
};

#endif // GRAPHICSENGINE_H
