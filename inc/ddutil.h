#ifdef WIN32

/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

extern IDirectDrawPalette * DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap);
extern IDirectDrawSurface * DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy);
extern HRESULT              DDReLoadBitmap(IDirectDrawSurface *pdds, LPCSTR szBitmap);
extern HRESULT              DDCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
extern DWORD                DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb);
extern HRESULT              DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

class CRect : public RECT
{
public:
	CRect();
	CRect(RECT aRect);
	CRect(int aLeft, int aTop, int aRight, int aBottom);
	CRect(int aWidth, int aHeight);
	bool includes(int aX, int aY);
};

class CSurface
{
public:
	LPDIRECTDRAWSURFACE iSurface;
	char *iBitmap;
	int iWidth, iHeight;
	CRect iRect;
	bool iColorKey;

private:
	CSurface();

public:
	~CSurface();
	static CSurface *create(LPDIRECTDRAW aLpDD, int aWidth, int aHeight);
	static CSurface *create(LPDIRECTDRAW aLpDD, char *aFileName, int aDX = 0, int aDY = 0);
	static CSurface *create(LPDIRECTDRAWSURFACE aLpDDSSurface);
	void setColorKey(COLORREF aRgb = CLR_INVALID);
	HDC getDc();
	void releaseDc(HDC aHdc);
	void blit(CSurface *aSrcSurface, CRect aSrcRect, CRect aDestRect, bool aUseColorKey = 1);
	void blit(LPDIRECTDRAWSURFACE aLpDDSSrc, CRect aSrcRect, CRect aDestRect, bool aUseColorKey = 1);
	void blitFast(CSurface *aSrcSurface, CRect aRect, int aX, int aY, bool aUseColorKey = 1);
	void blitFast(LPDIRECTDRAWSURFACE aLpDDSSrc, CRect aRect, int aX, int aY, bool aUseColorKey = 1);
	void *lock();
	void unlock();
	HRESULT restore();
	bool resize(LPDIRECTDRAW aLpDD, int aWidth, int aHeight);
	void getSize();
};

#endif // WIN32
