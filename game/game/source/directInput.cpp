#include "../include/directInput.h"

#pragma comment(lib, "dinput8")
#pragma comment(lib, "dxguid")

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

CDirectInput *directInput = NULL;

CDirectInput::CDirectInput()
{
	for (int i = 0; i < KEYBOARD_SIZE; i++)
	{
		mKeyboardTable.keyboardState[i]	= KEY_UP;
		mKeyboardTable.keyboardCheck[i]	= KEY_UP;
		mBoundKeys[i]					= KEY_UP;
	}
	mBoundKeys[MOVE_LEFT]	= DIK_A;
	mBoundKeys[MOVE_RIGHT]	= DIK_D;
	mBoundKeys[MOVE_UP]		= DIK_W;
	mBoundKeys[MOVE_DOWN]	= DIK_S;
	mBoundKeys[WRITE_TEXT]	= DIK_T;


	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
									IID_IDirectInput8, (void**)&mDI, NULL)))
		FreeDirectInput();

	if (FAILED(mDI->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL)))
		FreeDirectInput();
	if (FAILED(mDI->CreateDevice(GUID_SysMouse, &mMouse, NULL)))
		FreeDirectInput();

    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
    if (FAILED(mKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		FreeDirectInput();
    if (FAILED(mMouse->SetDataFormat(&c_dfDIMouse2)))
		FreeDirectInput();

/*
	// set buffered input
    DIPROPDWORD dipdw;

    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = 8;		// Arbitary buffer size

    if (FAILED(mKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		FreeDirectInput();
*/
	directInput = this;
}

CDirectInput::~CDirectInput()
{
	FreeDirectInput();
}

void CDirectInput::Activate()
{
	mKeyboard->Acquire();
	mMouse->Acquire();
}

void CDirectInput::ReadState()
{
	if (mKeyboard != NULL)
	{
		// TODO: Buffered input-read

		BYTE tempKeyboardState[KEYBOARD_SIZE];
		for (int i = 0; i < KEYBOARD_SIZE; i++)
			tempKeyboardState[i] = mKeyboardTable.keyboardState[i];

		// Get the input's device state, and update mKeyboardState
		ZeroMemory(mKeyboardTable.keyboardState, KEYBOARD_SIZE);
		if (!FAILED(mKeyboard->GetDeviceState(KEYBOARD_SIZE, mKeyboardTable.keyboardState)))
		{
			for (i = 0; i < KEYBOARD_SIZE; i++)
				if (tempKeyboardState[i] == KEY_UP && mKeyboardTable.keyboardState[i] == KEY_DOWN)
					mKeyboardTable.keyboardCheck[i] = KEY_DOWN;
		}
		else
		{
			// If input is lost then acquire and keep trying 
			while(mKeyboard->Acquire() == DIERR_INPUTLOST);
			return;
		}
	}

	if (mMouse != NULL)
	{
		// Get the input's device state, and update mMouseState
		ZeroMemory(&mMouseState, sizeof(mMouseState));
		if (FAILED(mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState)))
		{
			// If input is lost then acquire and keep trying 
			while(mMouse->Acquire() == DIERR_INPUTLOST);
			return;
		}
	}
}

bool CDirectInput::isPressed(GameAction aAction)
{
	int temp = getKey(aAction);
	if (temp == UNDEFINED)
		return false;

	if (mKeyboardTable.keyboardState[temp] == KEY_DOWN)
		return true;
	else return false;
}

bool CDirectInput::checkKey(GameAction aAction)
{
	int temp = getKey(aAction);
	if (temp == UNDEFINED)
		return false;

	if (mKeyboardTable.keyboardCheck[temp] == KEY_DOWN)
	{
		mKeyboardTable.keyboardCheck[temp] = KEY_UP;
		return true;
	}
	else return false;
}

const DIMOUSESTATE2* CDirectInput::getMouseState()
{
	return &mMouseState;
}

void CDirectInput::bindKey(GameAction aAction, int aKey)
{
	for (int i = 0; i < KEYBOARD_SIZE; i++)
		if (mBoundKeys[i] == aKey)
			mBoundKeys[i] = UNDEFINED;

	mBoundKeys[aAction] = aKey;
}

int CDirectInput::getKey(GameAction aAction)
{
	return mBoundKeys[aAction];
}

void CDirectInput::FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if(mKeyboard) mKeyboard->Unacquire();
    if(mMouse) mMouse->Unacquire();

    // Release any DirectInput objects.
    SAFE_RELEASE(mKeyboard);
    SAFE_RELEASE(mMouse);
    SAFE_RELEASE(mDI);
}
