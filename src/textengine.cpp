#include <iostream>
#include <cstring>
#include <cstdio>

#include "../inc/textengine.h"

using namespace std;

CTextEngine::CTextEngine() : mMapSymbols(NULL), mBotSymbols(NULL)
{
	mMapSymbols = " #o. $u, Ms' %n` ";
	mBotSymbols = "@0&P";
	cout << "\033[2J\033[1;1H";
}

CTextEngine::~CTextEngine()
{
}

void CTextEngine::drawTilemap(char **aTilemap, int aWidth, int aHeight)
{
	cout << "\033[1;1H";
	char *sTemp;
	sTemp = new char[aWidth];
	sTemp[aWidth] = '\0';
	for (int i = 0; i < aHeight; i++)
	{
		for (int j = 0; j < aWidth; j++)
		{
			sTemp[j] = mMapSymbols[aTilemap[i][j] & 15];
		}
		cout << sTemp << endl;
	}
	delete [] sTemp;
}

void CTextEngine::drawGameObj(const CGameObj *aGameObj)
{
	int x, y;
	x = (int)aGameObj->xPos();
	y = (int)aGameObj->yPos();
	printf("\033[%d;%dH%c", y, x, mBotSymbols[(aGameObj->type() >> 4) & 3]);
}
