#include "../include/messageBox.h"
#include <assert.h>

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

CMessageBox::CMessageBox(int aX1, int aY1, int aX2, int aY2, const int aAmount) :
	mAmount(aAmount), mX1(aX1), mY1(aY1), mX2(aX2), mY2(aY2)
{
	mMessages	= new OnGameMessage[mAmount];
	for (int i = 0; i < mAmount; i++)
	{
		mMessages[i].priority	= mAmount - 1 - i;
		mMessages[i].data		= "";
		mMessages[i].time		= 0;
		mMessages[i].expiry		= 0;
	}
	mRc			= new RECT[mAmount];

	mFont		= NULL;
	mFontName	= "Times New Roman";
	mFontSize	= 0.9;

//    HDC hDC = GetDC( NULL );
//    int nLogPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
//    ReleaseDC( NULL, hDC );

    int nHeight = -mFontSize * (mY1 - mY2);
    if(FAILED(D3DXCreateFont(d3dObj->mD3DDevice,	// D3D device
                         nHeight,					// Height
                         0,							// Width
                         FW_BOLD,					// Weight
                         1,							// MipLevels, 0 = autogen mipmaps
                         FALSE,						// Italic
                         DEFAULT_CHARSET,			// CharSet
                         OUT_DEFAULT_PRECIS,		// OutputPrecision
                         DEFAULT_QUALITY,			// Quality
                         DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
                         mFontName.c_str(),			// pFaceName
                         &mFont)))					// ppFont
        return;

	int foo = mFont->OnResetDevice();

    if(FAILED(D3DXCreateSprite(d3dObj->mD3DDevice, &mTextSprite)))
        return;

	d3dObj->mD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

CMessageBox::~CMessageBox()
{
	delete []mMessages;
	mMessages = NULL;
	delete []mRc;
	mRc = NULL;

	SAFE_RELEASE(mFont);
	SAFE_RELEASE(mTextSprite);

}

void CMessageBox::addMessage(string* aMessage, int aExpiry)
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
	mMessages[i].expiry		= aExpiry;

	time (&(mMessages[i].time));

	for (i++;i < mAmount; i++)
		mMessages[i].priority	+= 1;
}

void CMessageBox::draw()
{
//	RECT	rc[mAmount];
	int		height = (mY2 - mY1) / mAmount;

	mTextSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	int		counter	 = 0;
	for (int i = 0; i < mAmount; i++)
	{
		if (mMessages[i].data != "" && (time(NULL) - mMessages[i].time) < mMessages[i].expiry)
		{
			SetRect(&(mRc[counter]), mX1, mY1+(height*counter), mX2, mY1+(height*(counter+1)));
			mFont->DrawText(mTextSprite, mMessages[i].data.c_str(), -1, &(mRc[counter]), DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
			counter++;
		}
	}
	mTextSprite->End();
}



