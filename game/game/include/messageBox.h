/** @file messageBox.h
 * Creates a messageBox that draws given messages to screen
 *
 * @author Anza
 * @version 0.1
 */

#ifndef	MESSAGEBOX_H
#define	MESSAGEBOX_H

#include <d3dx9.h>
#include <d3dxerr.h>
#include <string>
#include <time.h>
#include "d3dUtil.h"

using std::string;

struct OnGameMessage
{
	int		priority;
	string	data;
	time_t	time;
	int		expiry;
};

class CMessageBox
{
public:
			CMessageBox		(int aX1, int aY1, int aX2, int aY2, const int aAmount);
			~CMessageBox	();

			// adds message to messageBox. Empiry-time in seconds
	void	addMessage		(string* aMessage, int aExpiry);

	void	draw			();

private:
	int				mX1;
	int				mY1;
	int				mX2;
	int				mY2;
	const int		mAmount;
	OnGameMessage*	mMessages;
	RECT*			mRc;

	ID3DXFont*		mFont;
	string			mFontName;
	int				mFontSize;
	ID3DXSprite*	mTextSprite;
};


#endif	// MESSAGEBOX_H