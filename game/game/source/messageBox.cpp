#include "../include/messageBox.h"
#include <assert.h>

CMessageBox::CMessageBox(IDirect3DDevice9* aD3dDevice, int aX1, int aY1, int aX2, int aY2, int aAmount)
{
	mX1			= aX1;
	mY1			= aY1;
	mX2			= aX2;
	mY2			= aY2;
	mAmount		= aAmount;
	mMessages	= new OnGameMessage[mAmount];
	for (int i = 0; i < mAmount; i++)
	{
		mMessages[i].priority	= i;
		mMessages[i].data		= "";
		mMessages[i].time		= 0;
	}
	mFont		= NULL;
	mFontName	= "Arial";
	mFontSize	= 10;

//    HDC hDC = GetDC( NULL );
//    int nLogPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
//    ReleaseDC( NULL, hDC );

    int nHeight = -mFontSize * (mY1 - mY2)/72;
    if(FAILED(D3DXCreateFont(aD3dDevice,		// D3D device
                         nHeight,				// Height
                         0,						// Width
                         FW_BOLD,				// Weight
                         1,						// MipLevels, 0 = autogen mipmaps
                         FALSE,					// Italic
                         DEFAULT_CHARSET,		// CharSet
                         OUT_DEFAULT_PRECIS,	// OutputPrecision
                         DEFAULT_QUALITY,		// Quality
                         DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
                         mFontName.c_str(),		// pFaceName
                         &mFont)))				// ppFont
        return;

	int foo = mFont->OnResetDevice();

	aD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

CMessageBox::~CMessageBox()
{
	delete []mMessages;
	mMessages = NULL;
}

void CMessageBox::addMessage(string* aMessage)
{
	for (int i = 0; i < mAmount; i++)
	{
		if (mMessages[i].priority == mAmount-1)
			break;
		mMessages[i].priority	+= 1;
	}
	assert (i < mAmount);	// data structure failure

	mMessages[i].data		= *aMessage;
	mMessages[i].priority	= 0;
	time (&(mMessages[i].time));

	for (i++;i < mAmount; i++)
		mMessages[i].priority	+= 1;
}

void CMessageBox::draw(IDirect3DDevice9* aD3dDevice)
{
	RECT	rc;
	string	text = "TESTI";

    SetRect(&rc, mX1, mY1, mX2, mY2);
    int foo = mFont->DrawText(NULL, text.c_str(), 5, &rc, DT_NOCLIP, D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.0f ));
//    mFont->DrawText(NULL, text.c_str(), -1, &rc, 0, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ));
}



