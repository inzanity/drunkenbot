/** @file ddgraphicsengine.h
 * Definition of CSDLGraphicsEngine.
 * Direct Draw graphics for the game. 
 *
 * @author inz
 * @version 1.0
 */

#ifdef SDL

#ifndef SDLGRAPHICSENGINE_H
#define SDLGRAPHICSENGINE_H

#include <istream>
#include <SDL/SDL.h>
#include "graphicsengine.h"

using std::istream;

/**
 * Simple DirectMedia Layer graphics engine.
 */
class CSDLGraphicsEngine : public CGraphicsEngine
{
public:
	/**
	 * Constructor. Window should be created before creating Direct Draw graphics engine.
	 * @param aHWnd Handle to the created window.
	 * @param aWidth Width of the canvas.
	 * @param aHeight Height of the canvas.
	 * @param aGfxInfo Stream to contain information about stream.
	 */
	CSDLGraphicsEngine(istream *aGfxInfo);

	/** Destructor. */
	virtual ~CSDLGraphicsEngine();

	/**
	 * Draws tilemap.
	 * @param aTileMap Tiles to draw. Two most significant bits define type of the tile. See TTileType.
	 * @param aWidth Width of the tilemap.
	 * @param aHeight Height of the tilemap.
	 */
	void drawTilemap(char **aTilemap, int aWidth, int aHeight);

	/** Draws game object. */
	void drawGameObj(const CGameObj *aGameObj);

	/** Flip buffers. */
	bool flip();

	/**
	 * Resizes canvas if size of the window has changed.
	 * @return True if resizing was successful.
	 */
/*	bool resize();*/

private:
	void releaseObjects();
/*	HRESULT restore();
	bool initFail(const char *aMsg); */

	int mWidth, mHeight;
	int mMapWidth, mMapHeight;
	int mSrcTileWidth, mSrcTileHeight;
	int mDestTileWidth, mDestTileHeight;
	SDL_Surface *mBack;
	SDL_Surface *mTiles;
	SDL_Surface *mBots;
	SDL_Surface *mBullets;
	SDL_Surface *mScreen;
};

#endif // SDLGRAPHICSENGINE_H

#endif // SDL
