/** @file ddgraphicsengine.h
 * Definition of CDDGraphicsEngine.
 * Direct Draw graphics for the game. 
 *
 * @author Japa
 * @version 1.0
 */

#ifdef WIN32

#ifndef DDGRAPHICSENGINE_H
#define DDGRAPHICSENGINE_H

#include <istream>
#include <ddraw.h>
#include "ddutil.h"
#include "graphicsengine.h"
#include "gameobj.h"

#pragma comment(lib, "ddraw")

using std::istream;

/**
 * Direct Draw graphics engine.
 */
class CDDGraphicsEngine : public IGraphicsEngine
{
public:
	/**
	 * Constructor. Window should be created before creating Direct Draw graphics engine.
	 * @param aHWnd Handle to the created window.
	 * @param aWidth Width of the canvas.
	 * @param aHeight Height of the canvas.
	 * @param aGfxInfo Stream to contain information about stream.
	 */
	CDDGraphicsEngine(HWND aHWnd, const istream *aGfxInfo);

	/** Destructor. */
	virtual ~CDDGraphicsEngine();

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
	void flip();

	/**
	 * Resizes canvas if size of the window has changed.
	 * @return True if resizing was successful.
	 */
	bool resize();

private:
	void releaseObjects();
	HRESULT restore();
	bool initFail(const char *aMsg);

	HWND mHWnd;
	int mWidth, mHeight;
	int mMapWidth, mMapHeight;
	int mSrcTileWidth, mSrcTileHeight;
	int mDestTileWidth, mDestTileHeight;
	LPDIRECTDRAWSURFACE mPrimary;
	LPDIRECTDRAWCLIPPER mClipper;
	LPDIRECTDRAW mDD;
	CSurface *mBack;
	CSurface *mTiles;
	CSurface *mBots;
};

#endif // DDGRAPHICSENGINE_H

#endif // WIN32
