/** @file directInput.h
 * Uses DirectInput to handle controllers such as keyboard and mouse
 *
 * @author Anza
 * @version 0.1
 */

#ifndef	DIRECTINPUT_H
#define	DIRECTINPUT_H

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <dinput.h>

/**
 * General DirectInput interface.
 */
class CDirectInput
{
public:
			CDirectInput	();
			~CDirectInput	();
	void	Activate		();
	void	ReadState		();

	bool	testi			();

private:
	void	FreeDirectInput();

	LPDIRECTINPUT8			mDI;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8	mKeyboard;	// The keyboard device 
	LPDIRECTINPUTDEVICE8	mMouse;		// The mouse device     
	BYTE					mKeyboardState[256];
	DIMOUSESTATE2			mMouseState;
};

#endif // DIRECTINPUT_H