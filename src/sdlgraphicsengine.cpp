#ifdef SDL

#include <SDL/SDL.h>
#include <istream>
#include <string>
#include "../inc/sdlgraphicsengine.h"

using std::istream;

inline int SDL_GetPixel (SDL_Surface *f, Uint32 x, Uint32 y, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
        /*const Uint32 mask[] = {0x0, 0xff, 0xffff, 0xffffff, 0xffffffff};*/
        Uint32 pixel;
        Uint8 *pp;
        int n; /* general purpose 'n'. */
        if (f == NULL) return -1;
        pp = (Uint8 *) f->pixels;
        if (x >= (Uint32)f->w || y >= (Uint32)f->h) return -1;
        pp += (f->pitch * y);
        pp += (x * f->format->BytesPerPixel);
        /* we do not lock the surface here, it would be inefficient XXX */
        /* this reads the pixel as though it was a big-endian integer XXX */
        /* I'm trying to avoid reading part the end of the pixel data by
         * using a data-type that's larger than the pixels */
        for (n = 0, pixel = 0; n < f->format->BytesPerPixel; ++n, ++pp)
        {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
                pixel >>= 8;
                pixel |= *pp << (f->format->BitsPerPixel - 8);
#else
                pixel |= *pp;
                pixel <<= 8;
#endif
        }
        SDL_GetRGBA(pixel, f->format, r, g, b, a);
        return 0;
}

inline int SDL_PutPixel (SDL_Surface *f, Uint32 x, Uint32 y, Uint8 r, Uint8 g, Uint8 b)
{
        Uint32 pixel;
        Uint8 *pp;
        int n;
        if (f == NULL) return -1;
        pp = (Uint8 *) f->pixels;
        if (x >= (Uint32)f->w || y >= (Uint32)f->h) return -1;
        pp += (f->pitch * y);
        pp += (x * f->format->BytesPerPixel);
        pixel = SDL_MapRGB(f->format, r, g, b);
        for (n = 0; n < f->format->BytesPerPixel; ++n, ++pp)
        {
                *pp = (Uint8) (pixel & 0xFF);
                pixel >>= 8;
        }

        return 0;
}

inline void SDL_ScaleBlit(SDL_Surface *src, SDL_Rect *sr, SDL_Surface *dst, SDL_Rect *dr)
{

	if (sr->w == dr->w && sr->h == dr->h)
	{
		SDL_BlitSurface(src, sr, dst, dr);
		return;
	}
	
        Uint8 r, g, b, a;
	float xscale = float(sr->w) / dr->w;
	float yscale = float(sr->h) / dr->h;
	for (Uint32 x = 0; x < dr->w; x++)
	{
		for (Uint32 y = 0; y < dr->h; y++)
		{
			SDL_GetPixel(src, sr->x + int(xscale * x), sr->y + int(yscale * y), &r, &g, &b, &a);
			if (a)
				SDL_PutPixel (dst, dr->x + x, dr->y + y, r, g, b);
                }
        }

        return;
#undef gsx
#undef gsy
}


// CSDLGraphicsEngine

CSDLGraphicsEngine::CSDLGraphicsEngine(istream *aGfxInfo) :
									 mWidth(0), mHeight(0), mMapWidth(0), mMapHeight(0),
									 mSrcTileWidth(0), mSrcTileHeight(0), mDestTileWidth(0), mDestTileHeight(0), 
									 mTiles(NULL), 
									 mBots(NULL), mBullets(NULL)
{
	SDL_Surface *temp;
	
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	
	std::string filename;

	if (aGfxInfo)
		*aGfxInfo >> mWidth >> mHeight;
	else {
		mWidth = 800;
		mHeight = 800;
	}

	mScreen = SDL_SetVideoMode(mWidth, mHeight, 0, SDL_HWSURFACE);

	SDL_EnableUNICODE(1);
	SDL_WM_SetCaption("- -=[ Drunken Bot III ]=- -", "dbot");

	if (aGfxInfo)
		*aGfxInfo >> filename;
	mSrcTileWidth = mSrcTileHeight = 16;
	temp = SDL_LoadBMP(aGfxInfo ? filename.c_str() : "res/tiles.bmp");
	mTiles = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if (aGfxInfo)
		*aGfxInfo >> filename;
	temp = SDL_LoadBMP(aGfxInfo ? filename.c_str() : "res/bot.bmp");
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, 255, 239, 239));
	mBots = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);
	if (aGfxInfo)
		*aGfxInfo >> filename;
	temp = SDL_LoadBMP(aGfxInfo ? filename.c_str() : "res/bullet.bmp");
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, 255, 239, 239));
	mBullets = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);
}

CSDLGraphicsEngine::~CSDLGraphicsEngine()
{
	releaseObjects();
}

void CSDLGraphicsEngine::drawTilemap(char **aTilemap, int aWidth, int aHeight)
{
	mDestTileWidth = mWidth / aWidth; mDestTileHeight = mHeight / aHeight;
	mMapWidth = aWidth * mDestTileWidth;
	mMapHeight = aHeight * mDestTileHeight;
	SDL_Rect rect;
	SDL_Rect dest;
	dest.w = mDestTileWidth;
	dest.h = mDestTileHeight;
	dest.y = 0;
	dest.x = 0;

	for (int i = 0; i < aWidth; i++)
	{
		for (int j = 0; j < aHeight; j++)
		{
			char tile = (mActiveBot ?
				mActiveBot->botAI()->mTilemap->getTile(i - int(mActiveBot->spawningXPos() + .01), j - int(mActiveBot->spawningYPos() + .01)) :
				aTilemap[j][i]);
			int type = tile & KTileTypeMask;
			int tileNum = (tile & KTileIndexMask) >> KTileIndexShift;
			rect.x = tileNum * mSrcTileWidth;
			rect.y = type * mSrcTileHeight;
			rect.w = mSrcTileWidth;
			rect.h = mSrcTileHeight;
			if (mActiveBot && (tile & ETileFowMask))
			{
				rect.y += 4 * mSrcTileHeight;
			}
			SDL_ScaleBlit(mTiles, &rect, mScreen, &dest);
			dest.y += mDestTileHeight;
		}
		dest.y = 0;
		dest.x += mDestTileWidth;
	}
}

void CSDLGraphicsEngine::drawGameObj(const CGameObj *aGameObj)
{
	if (mActiveBot && mActiveBot->botAI()->mTilemap->getTile(int(aGameObj->xPos()) - mActiveBot->spawningXPos(), int(aGameObj->yPos()) - mActiveBot->spawningYPos()) && ETileFowMask)
		return;
	int type = (aGameObj->type() & KObjectTypeMask) >> KObjectTypeShift;
	int index = (aGameObj->type() & KObjectTypeMask) >> KObjectIndexShift;
//	int team = (aGameObj->type() >> KObjectTeamShift) & KObjectTypeMask;
	int frame = int(aGameObj->animationTimer() * 0)/*framenum*/;
	int dir = int((aGameObj->orientation() + PI / 8.f) / (PI / 4.f)) % 8;
	int w = int(mDestTileWidth * 2.f * aGameObj->radius());
	int h = int(mDestTileHeight * 2.f * aGameObj->radius());
	int x1 = int(aGameObj->xPos() * mDestTileWidth - w / 2);
	int y1 = int(aGameObj->yPos() * mDestTileHeight - h / 2);
	SDL_Rect rect;
	SDL_Rect rect2;
	rect2.x = x1;
	rect2.y = y1;
	rect2.w = w;
	rect2.h = h;
	if (type == EObjectBot)
	{
		rect.x = frame * mSrcTileWidth;
		rect.y = dir * mSrcTileHeight;
		rect.w = mSrcTileWidth;
		rect.h = mSrcTileHeight;
		SDL_ScaleBlit(mBots, &rect, mScreen, &rect2);
	}
	else if (type == EObjectBullet || type == EObjectExplosion)
							{
		rect.x = index * mSrcTileWidth;
		rect.y = (type == EObjectExplosion ? 8 : dir) * mSrcTileHeight;
		rect.w = mSrcTileWidth;
		rect.h = mSrcTileHeight;
		SDL_ScaleBlit(mBullets, &rect, mScreen, &rect2);
	}
}

char CSDLGraphicsEngine::flip()
{
	SDL_Flip(mScreen);
	SDL_Event event;
	while (SDL_PollEvent(&event))
		switch (event.type)
		{
			case SDL_KEYDOWN:
				return event.key.keysym.unicode;
				break;
			case SDL_QUIT:
				return 'q';
				break;
		}
	return '\0';
}
/*
bool CSDLGraphicsEngine::resize()
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
*/
void CSDLGraphicsEngine::releaseObjects()
{
	if (mTiles)
	{
		SDL_FreeSurface(mTiles);
		mTiles = NULL;
	}
	if (mBots)
	{
		SDL_FreeSurface(mBots);
		mBots = NULL;
	}
	if (mBullets)
	{
		SDL_FreeSurface(mBullets);
		mBullets = NULL;
	}
}
/*
HRESULT CSDLGraphicsEngine::restore()
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
	return ddrval;
}

bool CSDLGraphicsEngine::initFail(const char *aMsg)
{
	MessageBox(mHWnd, aMsg, "DirectDraw Graphics Engine", MB_OK);
	return false;
}
*/
#endif // SDL
