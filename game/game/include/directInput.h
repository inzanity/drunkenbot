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
			CDirectInput	(HWND *aHWnd);
			~CDirectInput	();
	void	UpdateUI		(HWND *aHWnd);
	void	Activate		();

private:
	void	FreeDirectInput();

	LPDIRECTINPUT8       mDI;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 mKeyboard;	// The keyboard device 
};

#endif // DIRECTINPUT_H