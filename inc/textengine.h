/** @file textengine.h
 * Definition of CTextGraphicsEngine.
 * A general really dummy Text-Graphics engine.
 *
 * @autor inz
 * @version 0.001
 */

#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include "gameobj.h"
#include "graphicsengine.h"

/**
 * Text graphics engine.
 */
class CTextEngine : public IGraphicsEngine
{
public:
	/** Constructor. */
	CTextEngine();

	/** Destructor. */
	~CTextEngine();

	/**
	 * Pure virtual method for drawing tilemap.
	 * @param aTileMap Tiles to draw. Two most significant bits define type of the tile. See TTileType.
	 * @param aWidth Width of the tilemap.
	 * @param aHeight Height of the tilemap.
	 */
	void drawTilemap(char **aTilemap, int aWidth, int aHeight);

	/** Pure virtual method for drawing game object. */
	void drawGameObj(const CGameObj *aGameObj);
private:
	char *mMapSymbols;
	char *mBotSymbols;
};

#endif // TEXTENGINE_H