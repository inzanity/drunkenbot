/** @file textengine.h
 * Definition of CTextGraphicsEngine.
 * A general really dummy Text-Graphics engine.
 *
 * @autor inz
 * @version 0.001
 */

#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include "graphicsengine.h"

/**
 * Text graphics engine.
 */
class CTextEngine : public CGraphicsEngine
{
public:
	/** Constructor. */
	CTextEngine();

	/** Destructor. */
	~CTextEngine();

	/**
	 * Method for drawing tilemap.
	 * @param aTileMap Tiles to draw. Two most significant bits define type of the tile. See TTileType.
	 * @param aWidth Width of the tilemap.
	 * @param aHeight Height of the tilemap.
	 */
	void drawTilemap(char **aTilemap, int aWidth, int aHeight);

	/**
	 * Method for drawing a game object.
	 * @param aGameObj Object to be drawed.
	 */
	
	void drawGameObj(const CGameObj *aGameObj);
private:
	char *mMapSymbols;
	char *mBotSymbols;
};

#endif // TEXTENGINE_H
