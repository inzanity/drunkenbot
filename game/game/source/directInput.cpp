#include "../include/directInput.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


CDirectInput::CDirectInput	(HWND *aHWnd)
{
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
									IID_IDirectInput8, (void**)&mDI, NULL)))
		FreeDirectInput();

	if (FAILED(mDI->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL)))
		FreeDirectInput();

    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
    if (FAILED(mKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		FreeDirectInput();

}
CDirectInput::~CDirectInput	()
{
	FreeDirectInput();
}

void CDirectInput::UpdateUI(HWND *aHWnd)
{
}

void CDirectInput::Activate()
{
	mKeyboard->Acquire();
}

void CDirectInput::FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if(mKeyboard) mKeyboard->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE(mKeyboard);
    SAFE_RELEASE(mDI);
}
