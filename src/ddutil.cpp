#ifdef WIN32

/*==========================================================================
 *
 *  Copyright (C) 1995-1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include "../inc/ddutil.h"

/*
 *  DDLoadBitmap
 *
 *  create a DirectDrawSurface from a bitmap resource.
 *
 */
extern "C" IDirectDrawSurface * DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy)
{
    HBITMAP             hbm;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    IDirectDrawSurface *pdds;

    //
    //  try to load the bitmap as a resource, if that fails, try it as a file
    //
    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);

    if (hbm == NULL)
        hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

    //
    // create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;

    if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
        return NULL;

    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

    DeleteObject(hbm);

    return pdds;
}

/*
 *  DDReLoadBitmap
 *
 *  load a bitmap from a file or resource into a directdraw surface.
 *  normaly used to re-load a surface after a restore.
 *
 */
HRESULT DDReLoadBitmap(IDirectDrawSurface *pdds, LPCSTR szBitmap)
{
    HBITMAP             hbm;
    HRESULT             hr;

    //
    //  try to load the bitmap as a resource, if that fails, try it as a file
    //
    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    if (hbm == NULL)
        hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }

    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }


    DeleteObject(hbm);
    return hr;
}

/*
 *  DDCopyBitmap
 *
 *  draw a bitmap into a DirectDrawSurface
 *
 */
extern "C" HRESULT DDCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
    HDC                 hdcImage;
    HDC                 hdc;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    HRESULT             hr;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;

    //
    // make sure this surface is restored.
    //
    pdds->Restore();

    //
    //  select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);

    //
    // get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);    // get size of bitmap
    dx = dx == 0 ? bm.bmWidth  : dx;    // use the passed size, unless zero
    dy = dy == 0 ? bm.bmHeight : dy;

    //
    // get size of surface.
    //
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }

    DeleteDC(hdcImage);

    return hr;
}

//
//  DDLoadPalette
//
//  Create a DirectDraw palette object from a bitmap resoure
//
//  if the resource does not exist or NULL is passed create a
//  default 332 palette.
//
extern "C" IDirectDrawPalette * DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap)
{
    IDirectDrawPalette* ddpal;
    int                 i;
    int                 n;
    int                 fh;
    HRSRC               h;
    LPBITMAPINFOHEADER  lpbi;
    PALETTEENTRY        ape[256];
    RGBQUAD *           prgb;

    //
    // build a 332 palette as the default.
    //
    for (i=0; i<256; i++)
    {
        ape[i].peRed   = (BYTE)(((i >> 5) & 0x07) * 255 / 7);
        ape[i].peGreen = (BYTE)(((i >> 2) & 0x07) * 255 / 7);
        ape[i].peBlue  = (BYTE)(((i >> 0) & 0x03) * 255 / 3);
        ape[i].peFlags = (BYTE)0;
    }

    //
    // get a pointer to the bitmap resource.
    //
    if (szBitmap && (h = FindResource(NULL, szBitmap, RT_BITMAP)))
    {
        lpbi = (LPBITMAPINFOHEADER)LockResource(LoadResource(NULL, h));
        if (!lpbi)
            OutputDebugString("lock resource failed\n");
        prgb = (RGBQUAD*)((BYTE*)lpbi + lpbi->biSize);

        if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (lpbi->biBitCount > 8)
            n = 0;
        else if (lpbi->biClrUsed == 0)
            n = 1 << lpbi->biBitCount;
        else
            n = lpbi->biClrUsed;

        //
        //  a DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for(i=0; i<n; i++ )
        {
            ape[i].peRed   = prgb[i].rgbRed;
            ape[i].peGreen = prgb[i].rgbGreen;
            ape[i].peBlue  = prgb[i].rgbBlue;
            ape[i].peFlags = 0;
        }
    }
    else if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);

        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;

        //
        //  a DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for(i=0; i<n; i++ )
        {
            BYTE r = ape[i].peRed;
            ape[i].peRed  = ape[i].peBlue;
            ape[i].peBlue = r;
        }
    }

    pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL);

    return ddpal;
}

/*
 * DDColorMatch
 *
 * convert a RGB color to a pysical color.
 *
 * we do this by leting GDI SetPixel() do the color matching
 * then we lock the memory and see what it got mapped to.
 */
extern "C" DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC ddsd;
    HRESULT hres;

    //
    //  use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
        SetPixel(hdc, 0, 0, rgb);               // set our value
        pdds->ReleaseDC(hdc);
    }

    //
    // now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;

    if (hres == DD_OK)
    {
        dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
        dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
        pdds->Unlock(NULL);
    }

    //
    //  now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }

    return dw;
}

/*
 * DDSetColorKey
 *
 * set a color key for a surface, given a RGB.
 * if you pass CLR_INVALID as the color key, the pixel
 * in the upper-left corner will be used.
 */
extern "C" HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;

    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}





// CRect

CRect::CRect()
{
	left = top = right = bottom = 0;
}

CRect::CRect(RECT aRect)
{
	left = aRect.left;
	top = aRect.top;
	right = aRect.right;
	bottom = aRect.bottom;
}

CRect::CRect(int aLeft, int aTop, int aRight, int aBottom)
{
	left = aLeft;
	top = aTop;
	right = aRight;
	bottom = aBottom;
}

CRect::CRect(int aWidth, int aHeight)
{
	left = top = 0;
	right = aWidth;
	bottom = aHeight;
}

bool CRect::includes(int aX, int aY)
{
	if (aX >= left && aX < right && aY >= top && aY < bottom)
		return 1;
	return 0;
}

// CSurface

CSurface::CSurface()
{
	iBitmap = NULL;
	iSurface = NULL;
	iWidth = iHeight = 0;
	iRect.left = iRect.top = iRect.right = iRect.bottom = 0;
	iColorKey = 0;
}

CSurface::~CSurface()
{
	if (iBitmap)
		delete iBitmap;
	if(iSurface)
		iSurface->Release();
}

CSurface *CSurface::create(LPDIRECTDRAW aLpDD, int aWidth, int aHeight)
{
	CSurface *temp = new CSurface;
	DDSURFACEDESC ddsd;

	temp->iRect.right = temp->iWidth = aWidth;
	temp->iRect.bottom = temp->iHeight = aHeight;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = aWidth;
	ddsd.dwHeight = aHeight;

	if (aLpDD->CreateSurface(&ddsd, &temp->iSurface, NULL) != DD_OK)
	{
		delete temp;
		return NULL;
	}

	if (temp->iSurface->Restore() != DD_OK)
	{
		delete temp;
		return NULL;
	}

	return temp;
}

CSurface *CSurface::create(LPDIRECTDRAW aLpDD, char *aFileName, int aDX, int aDY)
{
	if (!aFileName)
		return NULL;
	CSurface *temp = new CSurface;
	
	temp->iBitmap = new char[strlen(aFileName) + 1];
	strcpy(temp->iBitmap, aFileName);

	temp->iSurface = DDLoadBitmap(aLpDD, temp->iBitmap, aDX, aDY);
	if(!temp->iSurface)
	{
		delete temp;
		return NULL;
	}
	temp->getSize();

	return temp;
}

CSurface *CSurface::create(LPDIRECTDRAWSURFACE aLpDDSSurface)
{
	if (!aLpDDSSurface)
		return NULL;

	CSurface *temp = new CSurface;
	temp->iSurface = aLpDDSSurface;
	temp->getSize();

	return temp;
}

// set colorkey for source-blits
void CSurface::setColorKey(COLORREF aRgb)
{
	if (DDSetColorKey(iSurface, aRgb) == DD_OK)
		iColorKey = 1;
}

// get Dc
HDC CSurface::getDc()
{
	HDC hdc;

	if (iSurface->GetDC(&hdc) == DD_OK)
		return hdc;
	return NULL;
}

// release Dc
void CSurface::releaseDc(HDC aHdc)
{
	iSurface->ReleaseDC(aHdc);
}

// blit from CSurface
void CSurface::blit(CSurface *aSrcSurface, CRect aSrcRect, CRect aDestRect, bool aUseColorKey)
{
	int flags = 0;
	HRESULT ddrval;

	if (aUseColorKey && aSrcSurface->iColorKey) flags = DDBLT_KEYSRC;

	while(1)
	{
		
		ddrval = iSurface->Blt(&aDestRect, aSrcSurface->iSurface, &aSrcRect, flags, NULL);
		if(ddrval == DD_OK)
			break;
		if(ddrval == DDERR_SURFACELOST)
		{
			if (restore() != DD_OK || aSrcSurface->restore() != DD_OK)
				break;
		}
		else if(ddrval != DDERR_WASSTILLDRAWING)
			break;
	}
}

// blit from LPDIRECTDRAWSURFACE
void CSurface::blit(LPDIRECTDRAWSURFACE aLpDDSSrc, CRect aSrcRect, CRect aDestRect, bool aUseColorKey)
{
	CSurface *temp = new CSurface;
	temp->create(aLpDDSSrc);
	blit(temp, aSrcRect, aDestRect, aUseColorKey);
	temp->iSurface = NULL;
	delete temp;
}

// blit from CSurface using BltFast
// includes automatic clipper
void CSurface::blitFast(CSurface *aSrcSurface, CRect aRect, int aX, int aY, bool aUseColorKey)
{
	int flags = 0;
	HRESULT ddrval;

	if (aUseColorKey && aSrcSurface->iColorKey) flags = DDBLTFAST_SRCCOLORKEY;

	if (aRect.left < 0) // clipper for src surface
	{
		aX -= aRect.left;
		aRect.left = 0;
	}
	if (aRect.right > aSrcSurface->iWidth)
		aRect.right = aSrcSurface->iWidth;
	if (aRect.top < 0)
	{
		aY -= aRect.top;
		aRect.top = 0;
	}
	if (aRect.bottom > aSrcSurface->iHeight)
		aRect.bottom = aSrcSurface->iHeight;

	if (aX < 0) // clipper for dest surface
	{
		aRect.left -= aX;
		aX = 0;
	}
	if (aX + (aRect.right - aRect.left) > iWidth)
		aRect.right = iWidth + aRect.left - aX;
	if (aY < 0)
	{
		aRect.top -= aY;
		aY = 0;
	}
	if (aY + (aRect.bottom - aRect.top) > iHeight)
		aRect.bottom = iHeight + aRect.top - aY;

	while(1)
	{
		ddrval = iSurface->BltFast(aX, aY, aSrcSurface->iSurface, &aRect, flags);
		if(ddrval == DD_OK)
			break;
		if(ddrval == DDERR_SURFACELOST)
		{
			if (restore() != DD_OK || aSrcSurface->restore() != DD_OK)
				break;
		}
		else if(ddrval != DDERR_WASSTILLDRAWING)
			break;
	}
}

// blit fast from LPDIRECTDRAWSURFACE
void CSurface::blitFast(LPDIRECTDRAWSURFACE aLpDDSSrc, CRect aRect, int aX, int aY, bool aUseColorKey)
{
	CSurface *temp = new CSurface;
	temp->create(aLpDDSSrc);
	blitFast(temp, aRect, aX, aY, aUseColorKey);
	temp->iSurface = NULL;
	delete temp;
}

void *CSurface::lock()
{
	void *ptr;
	DDSURFACEDESC temp;

	ZeroMemory(&temp, sizeof(temp));
	temp.dwSize = sizeof(temp);
	while (iSurface->Lock(NULL, &temp, 0, NULL) == DDERR_WASSTILLDRAWING);

	ptr = temp.lpSurface;

	return ptr;
}

void CSurface::unlock()
{
	iSurface->Unlock(NULL);
}

HRESULT CSurface::restore()
{
	HRESULT ddrval;

	ddrval = iSurface->Restore();
	if (ddrval == DD_OK && iBitmap)
		ddrval = DDReLoadBitmap(iSurface, iBitmap);

	return ddrval;
}

bool CSurface::resize(LPDIRECTDRAW aLpDD, int aWidth, int aHeight)
{
	DDSURFACEDESC ddsd;

	iRect.right = iWidth = aWidth;
	iRect.bottom = iHeight = aHeight;

	if (iSurface)
		iSurface->Release();

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = aWidth;
	ddsd.dwHeight = aHeight;

	if (aLpDD->CreateSurface(&ddsd, &iSurface, NULL) != DD_OK)
		return 0;

	if (iSurface->Restore() != DD_OK)
		return 0;

	return 1;
}

void CSurface::getSize()
{
	DDSURFACEDESC DDSDesc;
	memset(&DDSDesc, 0, sizeof(DDSURFACEDESC));
	DDSDesc.dwSize = sizeof(DDSURFACEDESC);
	DDSDesc.dwFlags =  DDSD_WIDTH | DDSD_HEIGHT | DDSD_PITCH;
	iSurface->GetSurfaceDesc(&DDSDesc);
	iWidth = DDSDesc.dwWidth;
	iHeight = DDSDesc.dwHeight;
	iRect.right = iWidth; iRect.bottom = iHeight;
}

#endif // WIN32
