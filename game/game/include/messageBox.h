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

using std::string;

struct OnGameMessage
{
	int		priority;
	string	data;
	time_t	time;
};

class CMessageBox
{
public:
			CMessageBox		(IDirect3DDevice9* aD3dDevice, int aX1, int aY1, int aX2, int aY2, int aAmount);
			~CMessageBox	();

	void	addMessage		(string* aMessage);

	void	draw			(IDirect3DDevice9* aD3dDevice);

private:
	int				mX1;
	int				mY1;
	int				mX2;
	int				mY2;
	int				mAmount;
	OnGameMessage*	mMessages;

	ID3DXFont*		mFont;
	string			mFontName;
	int				mFontSize;
};


#endif	// MESSAGEBOX_H