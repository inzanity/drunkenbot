#include <cmath>
#include "../inc/tilemap.h"

CTilemap::CTilemap(int aWidth, int aHeight) : mWidth(aWidth), mHeight(aHeight)
{
	mTilemap = new char *[2 * mHeight - 1];
	for (int i = 0; i < 2 * mHeight - 1; i++)
	{
		mTilemap[i] = new char[2 * mWidth - 1]; 
		for (int j = 0; j < 2 * mWidth - 1; j++)
			mTilemap[i][j] = (char)(ETileUndetected | KTileFowMask);
	}
}

CTilemap::~CTilemap()
{
	if (mTilemap)
	{
		for (int i = 0; i < 2 * mHeight - 1; i++)
			delete [] mTilemap[i];
		delete [] mTilemap;
	}
}

void CTilemap::setTile(int aX, int aY, char aTile)
{
	if (aX <= -mWidth || aX >= mWidth || aY <= -mHeight || aY >= mHeight)
		return;
	mTilemap[aY + mHeight - 1][aX + mWidth - 1] = aTile;
}

void CTilemap::resetFogOfWar()
{
	for (int i = 0; i < 2 * mHeight - 1; i++)
		for (int j = 0; j < 2 * mWidth - 1; j++)
			mTilemap[i][j] |= KTileFowMask;
}

char CTilemap::getTile(int aX, int aY) const
{
	if (aX <= -mWidth || aX >= mWidth || aY <= -mHeight || aY >= mHeight)
		return (char)(ETileUndetected | KTileFowMask);
	return mTilemap[aY + mHeight - 1][aX + mWidth - 1];
}

char CTilemap::getTile(float aX, float aY) const
{
	return getTile((int)floorf(aX), (int)floorf(aY));
}

char CTilemap::getTile(const CGameObj *aGameObj) const
{
	return getTile((int)floorf(aGameObj->xPos()), (int)floorf(aGameObj->yPos()));
}

CTilemap::TTileType CTilemap::type(char aTile) const
{
	return (TTileType(aTile & KTileTypeMask));
}

bool CTilemap::visible(char aTile) const
{
	return !(aTile & KTileFowMask);
}
