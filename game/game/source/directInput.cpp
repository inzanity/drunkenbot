#include "../include/directInput.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

CDirectInput::CDirectInput()
{
	for (int i = 0; i < sizeof(mKeyboardState); i++)
		mKeyboardState[i] = 0;

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
	if (mKeyboard == NULL)
		return;

	// TODO: Buffered input-read

	// Get the input's device state, and update mKeyboardState
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	if (FAILED(mKeyboard->GetDeviceState(sizeof(mKeyboardState), mKeyboardState)))
	{
		// If input is lost then acquire and keep trying 
		while(mKeyboard->Acquire() == DIERR_INPUTLOST);
		return;
	}

	if (mMouse == NULL)
		return;

    ZeroMemory(&mMouseState, sizeof(mMouseState));
	if (FAILED(mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState)))
	{
		// If input is lost then acquire and keep trying 
		while(mMouse->Acquire() == DIERR_INPUTLOST);
		return;
	}
}

bool CDirectInput::testi()
{
	if (mKeyboardState[30] == 0x80)
		return true;
	else return false;
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
