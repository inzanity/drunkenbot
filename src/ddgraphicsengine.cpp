#ifdef WIN32

#include "../inc/ddgraphicsengine.h"

// CDDGraphicsEngine

CDDGraphicsEngine::CDDGraphicsEngine(HWND aHWnd, const istream *aGfxInfo) :
									 mHWnd(aHWnd), mWidth(0), mHeight(0), mMapWidth(0), mMapHeight(0),
									 mSrcTileWidth(0), mSrcTileHeight(0), mDestTileWidth(0), mDestTileHeight(0), 
									 mPrimary(NULL), mClipper(NULL), mDD(NULL), mBack(NULL), mTiles(NULL), 
									 mBots(NULL), mBullets(NULL), mWeapons(NULL)
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
/*
	    // Get exclusive mode
    ddrval = mDD->SetCooperativeLevel(mHWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if(ddrval != DD_OK)
    {
        initFail("foo");
		return;
    }

    // Set the video mode to 800x600x16
    ddrval = mDD->SetDisplayMode(800, 600, 16);
    if(ddrval != DD_OK)
    {
        initFail("bar");
		return;
    }
*/
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
	mBullets = CSurface::create(mDD, "res/bullet.bmp");
	mWeapons = CSurface::create(mDD, "res/weapon.bmp");
	mBots->setColorKey(RGB(255, 239, 239));
	mBullets->setColorKey(RGB(255, 239, 239));
	mWeapons->setColorKey(RGB(255, 239, 239));
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
	CRect rect, destRect(mDestTileWidth, mDestTileHeight);

	int x = 0, y = 0;
	for (int i = 0; i < aWidth; i++)
	{
		for (int j = 0; j < aHeight; j++)
		{
			char tile = (mActiveBot ?
				mActiveBot->botAI()->mTilemap->getTile(i - mActiveBot->spawningXPos(), j - mActiveBot->spawningYPos()) :
				aTilemap[j][i]);
			int type = tile & KTileTypeMask;
			int tileNum = (tile & KTileIndexMask) >> KTileIndexShift;
			rect.left = tileNum * mSrcTileWidth;
			rect.top = type * mSrcTileHeight;
			rect.right = rect.left + mSrcTileWidth;
			rect.bottom = rect.top + mSrcTileHeight;
			if (mActiveBot && (tile & KTileFowMask))
			{
				rect.top += 4 * mSrcTileHeight;
				rect.bottom += 4 * mSrcTileHeight;
			}
			mBack->blit(mTiles, rect, destRect, 0);
			destRect.top += mDestTileHeight; destRect.bottom += mDestTileHeight;
		}
		destRect.top = 0; destRect.bottom = mDestTileHeight;
		destRect.left += mDestTileWidth; destRect.right += mDestTileWidth;
	}
}

void CDDGraphicsEngine::drawGameObj(const CGameObj *aGameObj)
{
	int type = (aGameObj->type() & KObjectTypeMask) >> KObjectTypeShift;
	int index = (aGameObj->type() & KObjectTypeMask) >> KObjectIndexShift;
//	int team = (aGameObj->type() >> KObjectTeamShift) & KObjectTypeMask;
	int frame = aGameObj->animationTimer() * 0/*framenum*/;
	int dir = int((aGameObj->orientation() + PI / 8.f) / (PI / 4.f)) % 8;
	int w = mDestTileWidth * 2.f * aGameObj->radius();
	int h = mDestTileHeight * 2.f * aGameObj->radius();
	int x1 = aGameObj->xPos() * mDestTileWidth - w / 2;
	int y1 = aGameObj->yPos() * mDestTileHeight - h / 2;
	int x2 = x1 + w;
	int y2 = y1 + h;
	CRect rect;
	if (type == CGameObj::EObjectBot)
	{
		rect.left = frame * mSrcTileWidth;
		rect.top = dir * mSrcTileHeight;
		rect.right = rect.left + mSrcTileWidth;
		rect.bottom = rect.top + mSrcTileHeight;
		mBack->blit(mBots, rect, CRect(x1, y1, x2, y2));
	}
	else if (type == CGameObj::EObjectBullet || type == CGameObj::EObjectExplosion)
	{
		rect.left = index * mSrcTileWidth;
		rect.top = (type == CGameObj::EObjectExplosion ? 8 : dir) * mSrcTileHeight;
		rect.right = rect.left + mSrcTileWidth;
		rect.bottom = rect.top + mSrcTileHeight;
		mBack->blit(mBullets, rect, CRect(x1, y1, x2, y2));
	}
	else if (type == CGameObj::EObjectWeapon)
	{
		rect.left = frame * mSrcTileWidth;
		rect.top = index * mSrcTileHeight;
		rect.right = rect.left + mSrcTileWidth;
		rect.bottom = rect.top + mSrcTileHeight;
		mBack->blit(mWeapons, rect, CRect(x1, y1, x2, y2));
	}
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
	if (mBullets)
	{
		delete mBullets;
		mBullets = NULL;
	}
	if (mWeapons)
	{
		delete mWeapons;
		mWeapons = NULL;
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
	if (ddrval != DD_OK) return ddrval;
	ddrval = mBullets->restore();
	if (ddrval != DD_OK) return ddrval;
	ddrval = mWeapons->restore();
	return ddrval;
}

bool CDDGraphicsEngine::initFail(const char *aMsg)
{
	MessageBox(mHWnd, aMsg, "DirectDraw Graphics Engine", MB_OK);
	return false;
}

#endif // WIN32
