/** @file directInput.h
 * Uses DirectInput to handle controllers such as keyboard and mouse
 *
 * @author Anza
 * @version 0.1
 */

#ifndef	DIRECTINPUT_H
#define	DIRECTINPUT_H

#define DIRECTINPUT_VERSION 0x0800
#define	KEY_DOWN			0x80
#define	KEY_UP				0x00
#define KEYBOARD_SIZE		256

enum MouseState{MOUSE_BUTTON_UP, MOUSE_BUTTON_DOWN, MOUSE_BUTTON_CHECKED};

#include <windows.h>
#include <dinput.h>
#include "def.h"

struct KeyboardTable
{
	// indicates the current state of keyboard
	BYTE	keyboardState[KEYBOARD_SIZE];
	// flags every key if pressed down once
	BYTE	keyboardCheck[KEYBOARD_SIZE];
};



/**
 * General DirectInput interface.
 */
class CDirectInput
{
public:
							CDirectInput	();
							~CDirectInput	();
	void					Activate		();
	void					ReadState		();

	// checks if action key hold down
	bool					isPressed		(GameAction aAction);
	// checks if action key pressed down and flags it checked
	bool					checkKey		(GameAction aAction);

	// returns mouse state
	const DIMOUSESTATE2*	getMouseState	();
	// returns x mouse coordinate
	int						getMouseX		();
	// returns y mouse coordinate
	int						getMouseY		();
	// checks if mouse button is down and flags it checked
	bool					checkMouseButton(int aIndex);


	// binds a key to an action
	void					bindKey			(GameAction aAction, int aKey);


private:
	int						getKey			(GameAction aAction);

	void					FreeDirectInput();

	LPDIRECTINPUT8			mDI;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8	mKeyboard;	// The keyboard device 
	LPDIRECTINPUTDEVICE8	mMouse;		// The mouse device     
	KeyboardTable			mKeyboardTable;
	MouseState				mMouseButtonTable[3];
	DIMOUSESTATE2			mMouseState;
	int						mMouseX, mMouseY;
	int						mBoundKeys[KEYBOARD_SIZE];
};

extern CDirectInput *directInput;

#endif // DIRECTINPUT_H