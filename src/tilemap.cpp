#include "../inc/tilemap.h"

CTilemap::CTilemap(int aWidth, int aHeight) : mWidth(aWidth), mHeight(aHeight)
{
	mTilemap = new unsigned char *[2 * mHeight - 1];
	for (int i = 0; i < 2 * mHeight - 1; i++)
	{
		mTilemap[i] = new unsigned char[2 * mWidth - 1]; 
		for (int j = 0; j < 2 * mWidth - 1; j++)
			mTilemap[i][j] = ETileUndetected | (1<<7);
	}
}

void CTilemap::setTile(int aX, int aY, TTileType aTile)
{
	if (aX <= -mWidth || aX >= mWidth || aY <= -mHeight || aY >= mHeight)
		return;
	mTilemap[aY + mHeight - 1][aX + mWidth - 1] = (unsigned char)aTile;
}

void CTilemap::resetFogOfWar()
{
	for (int i = 0; i < 2 * mHeight - 1; i++)
		for (int j = 0; j < 2 * mWidth - 1; j++)
			mTilemap[i][j] |= (1<<7);
}

unsigned char CTilemap::getTile(int aX, int aY) const
{
	if (aX <= -mWidth || aX >= mWidth || aY <= -mHeight || aY >= mHeight)
		return (ETileUndetected | (1<<7));
	return mTilemap[aY + mHeight - 1][aX + mWidth - 1];
}

unsigned char CTilemap::getTile(float aX, float aY) const
{
	return getTile(int(aX), int(aY));
}

unsigned char CTilemap::getTile(const CGameObj *aGameObj) const
{
	return getTile(int(aGameObj->xPos()), int(aGameObj->yPos()));
}

CTilemap::TTileType CTilemap::type(unsigned char aTile) const
{
	return (TTileType(aTile & 0x7F));
}

bool CTilemap::visible(unsigned char aTile) const
{
	return !(aTile & (1<<7));
}
