#include "../inc/tilemap.h"

CTilemap::CTilemap(int aWidth, int aHeight) : mWidth(aWidth), mHeight(aHeight)
{
	mTilemap = new char *[2 * mHeight - 1];
	for (int i = 0; i < 2 * mHeight - 1; i++)
	{
		mTilemap[i] = new char[2 * mWidth - 1]; 
		for (int j = 0; j < 2 * mWidth - 1; j++)
			mTilemap[i][j] = (char)(ETileUndetected | (1<<7));
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
			mTilemap[i][j] |= (1<<7);
}

char CTilemap::getTile(int aX, int aY) const
{
	if (aX <= -mWidth || aX >= mWidth || aY <= -mHeight || aY >= mHeight)
		return (char)(ETileUndetected | (1<<7));
	return mTilemap[aY + mHeight - 1][aX + mWidth - 1];
}

char CTilemap::getTile(float aX, float aY) const
{
	return getTile(int(aX), int(aY));
}

char CTilemap::getTile(const CGameObj *aGameObj) const
{
	return getTile(int(aGameObj->xPos()), int(aGameObj->yPos()));
}

CTilemap::TTileType CTilemap::type(char aTile) const
{
	return (TTileType(aTile & 3));
}

bool CTilemap::visible(char aTile) const
{
	return !(aTile & (1<<7));
}
