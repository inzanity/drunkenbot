#include <iostream>
#include <cstring>
#include <cstdio>

#include "../inc/textengine.h"

using namespace std;

CTextEngine::CTextEngine() : mMapSymbols(NULL), mBotSymbols(NULL), mBulletSymbols(NULL)
{
	mMapSymbols = " #o. $u, Ms' %n` ";
	mBotSymbols = "@0&P";
	mBulletSymbols = "-\\|/";
	cout << "\033[2J\033[?25l" << endl;
}

CTextEngine::~CTextEngine()
{
	cout << "\033[?25h\033[2J" << endl << "\033[1;1H";
}

void CTextEngine::drawTilemap(char **aTilemap, int aWidth, int aHeight)
{
	cout << "\033[1;1H";
	char *sTemp;
	char cTemp;
	char cOld = 5;
	sTemp = new char[aWidth];
	sTemp[aWidth] = '\0';
	for (int i = 0; i < aHeight; i++)
	{
		for (int j = 0; j < aWidth; j++)
		{
			if (mActiveBot)
				cTemp = mActiveBot->botAI()->mTilemap->getTile(j - mActiveBot->spawningXPos(), i - mActiveBot->spawningYPos());
			else
				cTemp = aTilemap[i][j];
			if (cTemp >> 7 != cOld)
				cout << "\033[" << (cTemp >> 7?"0;34m":"0m");;
			cout << mMapSymbols[cTemp & 15];
			cOld = cTemp >> 7;
		}
		cout << endl;
	}
	delete [] sTemp;
}

void CTextEngine::drawGameObj(const CGameObj *aGameObj)
{
	int x, y;
	int j;
	char out = '\0';
	x = (int)aGameObj->xPos();
	y = (int)aGameObj->yPos();
	if ((aGameObj->type() & 0xf) == EObjectExplosion)
	{
		for (int i = -int(aGameObj->radius()); i <= int(aGameObj->radius()); i++)
		{
			if (y + i < 0) continue;
			j = abs(i) - int(aGameObj->radius());
			printf("\033[%d;%dH", y + i + 1, (x + j > 0 ? x + j + 1 : 1));
			for (; j < int(aGameObj->radius()) - abs(i) - 1; j++)
			{
				if (x + i + j < 0) continue;
				cout << "*";
			}
			cout << endl;
		}
		return;
	}
	if ((aGameObj->type() & 0xf) == EObjectBot)
		out = mBotSymbols[(aGameObj->type() >> 8) & 3];
	else if ((aGameObj->type() & 0xf) == EObjectBullet)
		out = mBulletSymbols[int((((const CMovingGameObj *)aGameObj)->movingDirection()) / PI * 4 + .5f) & 3];
	if (mActiveBot)
	{
		if (!(mActiveBot->botAI()->mTilemap->getTile(x - mActiveBot->spawningXPos(), y - mActiveBot->spawningYPos()) >> 7))
			printf("\033[%d;%dH\033[1;36m%c\n", y + 1, x + 1, out);
	}
	else
		printf("\033[%d;%dH\033[1;36m%c\n", y + 1, x + 1, out);
}
