/** @file tilemap.h
 * Definition of CTilemap class.
 *
 * @author Japa
 * @version 1.0
 */

#ifndef TILEMAP_H
#define TILEMAP_H

#include "gameobj.h"

/**
 * Class to store tilemap for CBotAI.
 * Coordinates are relative to bots spawning position.
 * <code>ETileUndetected</code> combined with "fog of war"-flag
 * is returned from all getters in case of coordinate overflow.
 */
class CTilemap
{
public:
	/** Generalized tile types. */
	enum TTileType{ETileUndetected, ETileWall, ETileObstacle, ETileEmpty};

	/**
	 * Constructor with the size of the source tilemap.
	 * @param aWidth Width of the source tilemap.
	 * @param aHeight Height of the source tilemap.
	 */
	CTilemap(int aWidth, int aHeight);

	/**
	 * Setter for tiles.
	 * @param aX X-coordinates of the tile.
	 * @param aY Y-coordinates of the tile.
	 * @param aTile Type of the tile. Setted tile is always visible.
	 */
	void setTile(int aX, int aY, TTileType aTile);

	/** Resets "fog of war"-flags to true. All tiles are initially not visible. */
	void resetFogOfWar();

	/**
	 * Tile getter. @param aX X-coordinates of the tile. @param aY Y-coordinates of the tile.
	 * @return TTileType combined with "fog of war"-flag (most significant bit).
	 */
	unsigned char getTile(int aX, int aY) const;

	/**
	 * Tile getter with float coordinates. Coordinates are rounded downwards.
	 * @param aX X-coordinates of the tile. @param aY Y-coordinates of the tile.
	 * @return TTileType combined with "fog of war"-flag (most significant bit).
	 */
	unsigned char getTile(float aX, float aY) const;

	/**
	 * Gets tile in position of the given object.
	 * @param aGameObj Object to get coordinates.
	 * @return TTileType combined with "fog of war"-flag (most significant bit).
	 */
	unsigned char getTile(const CGameObj *aGameObj) const;

	/** Getter for tile type. @param aTile Tile to get type from. */
	TTileType type(unsigned char aTile) const;

	/**
	 * Getter for visibility of the tile. "Fog of war" is detected from the most significant bit of the tile.
	 * @param aTile Tile to get visibility from.
	 * @return True if tile is visible.
	 */
	bool visible(unsigned char aTile) const;

private:
	unsigned char **mTilemap;
	int mWidth, mHeight;
};

#endif // TILEMAP_H
