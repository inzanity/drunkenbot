#ifdef WIN32

#include "../inc/ddgraphicsengine.h"

// CDDGraphicsEngine

CDDGraphicsEngine::CDDGraphicsEngine(HWND aHWnd, const istream *aGfxInfo) :
									 mHWnd(aHWnd), mWidth(0), mHeight(0), mMapWidth(0), mMapHeight(0),
									 mSrcTileWidth(0), mSrcTileHeight(0), mDestTileWidth(0), mDestTileHeight(0), 
									 mPrimary(NULL), mClipper(NULL), mDD(NULL), mBack(NULL), mTiles(NULL), 
									 mBots(NULL)
{
	DDSURFACEDESC ddsd;
	HRESULT ddrval;

	ddrval = DirectDrawCreate(NULL, &mDD, NULL);
	if(ddrval != DD_OK)
	{
		initFail("DirectDrawCreate failed");
		return;
	}
   
	ddrval = mDD->SetCooperativeLevel(mHWnd, DDSCL_NORMAL);
	if(ddrval != DD_OK)
	{
		initFail("SetCooperativeLevel failed");
		return;
	}

	// Create the primary surface
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	ddrval = mDD->CreateSurface(&ddsd, &mPrimary, NULL);
	if(ddrval != DD_OK)
	{
		initFail("Primary surface creation failed");
		return;
	}

	if (!resize())
		return;

	// TODO: read bitmap names and stuff from stream.
	mSrcTileWidth = mSrcTileHeight = 16;
	mTiles = CSurface::create(mDD, "res/tiles.bmp");
	mBots = CSurface::create(mDD, "res/bot.bmp");
}

CDDGraphicsEngine::~CDDGraphicsEngine()
{
	releaseObjects();
}

void CDDGraphicsEngine::drawTilemap(char **aTilemap, int aWidth, int aHeight)
{
	mDestTileWidth = mWidth / aWidth; mDestTileHeight = mHeight / aHeight;
	mMapWidth = aWidth * mDestTileWidth;
	mMapHeight = aHeight * mDestTileHeight;
	CRect rect;

	int x = 0, y = 0;
	for (int i = 0; i < aWidth; i++)
	{
		for (int j = 0; j < aHeight; j++)
		{
			int type = aTilemap[j][i] & 3;
			int tileNum = (aTilemap[j][i] >> 2) & 31;
			rect.left = tileNum * mSrcTileWidth;
			rect.top = type * mSrcTileHeight;
			rect.right = rect.left + mSrcTileWidth;
			rect.bottom = rect.top + mSrcTileHeight;
			mBack->blit(mTiles, rect, CRect(x, y, x + mDestTileWidth, y + mDestTileHeight), 0);
			y += mDestTileHeight;
		}
		y = 0;
		x += mDestTileWidth;
	}
}

void CDDGraphicsEngine::drawGameObj(const CGameObj *aGameObj)
{
	int type = aGameObj->type() & 255;
	int index = (aGameObj->type() >> 8) & 255;
	int team = (aGameObj->type() >> 16) & 255;
	int frame = aGameObj->animationTimer() * 0/*framenum*/;
	int dir = int((aGameObj->orientation() + PI / 8.f) / (PI / 4.f)) % 8;
	int w = mDestTileWidth * 2.f * aGameObj->radius();
	int h = mDestTileHeight * 2.f * aGameObj->radius();
	int x1 = aGameObj->xPos() * mDestTileWidth - w / 2;
	int y1 = aGameObj->yPos() * mDestTileHeight - h / 2;
	int x2 = x1 + w;
	int y2 = y1 + h;
	CRect rect;
	rect.left = frame * mSrcTileWidth;
	rect.top = dir * mSrcTileHeight;
	rect.right = rect.left + mSrcTileWidth;
	rect.bottom = rect.top + mSrcTileHeight;
	mBack->blit(mBots, rect, CRect(x1, y1, x2, y2), 0);
}

void CDDGraphicsEngine::flip()
{
	HRESULT ddrval;
	RECT rect;
	POINT pt = {0, 0};
	GetClientRect(mHWnd, &rect);
	ClientToScreen(mHWnd, &pt);
	OffsetRect(&rect, pt.x, pt.y);
	do
	{
		ddrval = mPrimary->Blt(&rect, mBack->iSurface, &CRect(mMapWidth, mMapHeight), 0, NULL);
		if(ddrval == DD_OK)
			break;
		if(ddrval == DDERR_SURFACELOST)
		{
			if(restore() != DD_OK)
				break;
			continue;
		}
	} while (ddrval == DDERR_WASSTILLDRAWING);
}

bool CDDGraphicsEngine::resize()
{
	HRESULT ddrval;

	RECT rect;
	GetClientRect(mHWnd, &rect);
	if (!rect.right || !rect.bottom || (rect.right == mWidth && rect.left == mHeight))
		return false;
	mWidth = rect.right; mHeight = rect.bottom;

	if (mBack)
	{
		delete mBack;
		mBack = NULL;
	}
	if (mClipper)
	{
		mClipper->Release();
		mClipper = NULL;
	}

	mBack = CSurface::create(mDD, mWidth, mHeight);
	if(!mBack)
		return initFail("Backbuffer creation failed");
		
    // Create a clipper for the primary surface
	ddrval = mDD->CreateClipper(0, &mClipper, NULL);
	if(ddrval != DD_OK)
		return initFail("CreateClipper failed");
	
	ddrval = mClipper->SetHWnd(0, mHWnd);
	if(ddrval != DD_OK)
		return initFail("SetHWnd failed");

	ddrval = mPrimary->SetClipper(mClipper);
	if(ddrval != DD_OK)
		return initFail("SetClipper failed");

	return true;
}

void CDDGraphicsEngine::releaseObjects()
{
	if (mPrimary)
	{
		mPrimary->Release();
		mPrimary = NULL;
	}
	if (mBack)
	{
		delete mBack;
		mBack = NULL;
	}
	if (mClipper)
	{
		mClipper->Release();
		mClipper = NULL;
	}

	if (mTiles)
	{
		delete mTiles;
		mTiles = NULL;
	}
	if (mBots)
	{
		delete mBots;
		mBots = NULL;
	}

	if(mDD)
	{
		mDD->Release();
		mDD = NULL;
	}
}

HRESULT CDDGraphicsEngine::restore()
{
	HRESULT	ddrval;

	ddrval = mPrimary->Restore();
	if (ddrval != DD_OK) return ddrval;
	ddrval = mBack->restore();
	if (ddrval != DD_OK) return ddrval;
	ddrval = mTiles->restore();
	if (ddrval != DD_OK) return ddrval;
	ddrval = mBots->restore();
	return ddrval;
}

bool CDDGraphicsEngine::initFail(const char *aMsg)
{
	MessageBox(mHWnd, aMsg, "DirectDraw Graphics Engine", MB_OK);
	return false;
}

#endif // WIN32
