#include "stdafx.h"

#include <d3dx9math.h>
#include "..\include\d3dutil.h"

#pragma comment (lib, "d3dx9")
#pragma comment (lib, "d3d9")

CD3DObj *d3dObj = NULL;

CD3DObj::CD3DObj(HWND aHWnd)
{
	d3dObj = this;
	mHWnd = aHWnd;
	mD3D = NULL;
	mD3DDevice = NULL;
	mMatrixStack = NULL;
}

CD3DObj::~CD3DObj()
{
	release();
}

// Release d3d objects
void CD3DObj::release()
{
	if (mD3DDevice != NULL)
	{
		mD3DDevice->Release();
		mD3DDevice = NULL;
	}

	if (mD3D != NULL)
	{
		mD3D->Release();
		mD3D = NULL;
	}

	if (mMatrixStack != NULL)
	{
		delete mMatrixStack;
		mMatrixStack = NULL;
	}
}

bool CD3DObj::reset()
{
//	CAnimationStorage::getSingletonPtr()->releaseAll();
	if (mD3DDevice->Reset(&mD3DPresentParams) == D3D_OK)
	{
		init();
//		CController::getSingletonPtr()->environment()->reset();
//		CAnimationStorage::getSingletonPtr()->restoreAll();
		return 1;
	}
	return 0;
}

bool CD3DObj::operational()
{
	if (mD3DDevice && mD3DDevice->TestCooperativeLevel() == D3D_OK)
		return 1;
	return 0;
}

void CD3DObj::init()
{
	mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	mD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

// Error handling
bool CD3DObj::initFail(char *aMsg)
{
	char string[64];
	strcpy(string, "D3D init failed");
	release();
	if (aMsg)
	{
		strcat(string, ": ");
		strcat(string, aMsg);
	}
	MessageBox(mHWnd, string, "CD3DObj", MB_OK);
	return 0;
}

// Initialize d3d objects
bool CD3DObj::initFullScreen(int aWidth, int aHeight, D3DFORMAT aFormat)
{
	release();

	// Create the mD3D object.
	if((mD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return initFail("Direct3DCreate9");

	// Set up the structure used to create the mD3DDevice
	ZeroMemory(&mD3DPresentParams, sizeof(mD3DPresentParams));
	mD3DPresentParams.Windowed = FALSE;
	mD3DPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mD3DPresentParams.BackBufferWidth = aWidth;
	mD3DPresentParams.BackBufferHeight = aHeight;
	mD3DPresentParams.BackBufferFormat = aFormat;
	mD3DPresentParams.EnableAutoDepthStencil = TRUE;
	mD3DPresentParams.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the mD3DDevice
	if(mD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mHWnd,
							D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							&mD3DPresentParams, &mD3DDevice) != D3D_OK)
		return initFail("CreateDevice");

	if (D3DXCreateMatrixStack(0, &mMatrixStack) != D3D_OK)
		return initFail("D3DXCreateMatrixStack");

	init();
	return 1;
}

bool CD3DObj::initWindowed()
{
	release();

	// Create the mD3D object.
	if((mD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return initFail("Direct3DCreate9");

	// Get the current desktop display mode, so we can set up a back
	// buffer of the same format
	D3DDISPLAYMODE d3ddm;

	if(FAILED(mD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return initFail("GetAdapterDisplayMode failed");

	// Set up the structure used to create the mD3DDevice
	ZeroMemory(&mD3DPresentParams, sizeof(mD3DPresentParams));
	mD3DPresentParams.Windowed = TRUE;
	mD3DPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mD3DPresentParams.BackBufferFormat = d3ddm.Format;
	mD3DPresentParams.EnableAutoDepthStencil = TRUE;
	mD3DPresentParams.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the mD3DDevice
	if(mD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mHWnd,
							D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							&mD3DPresentParams, &mD3DDevice) != D3D_OK)
		return initFail("CreateDevice");

	if (D3DXCreateMatrixStack(0, &mMatrixStack) != D3D_OK)
		return initFail("D3DXCreateMatrixStack");

	init();
	return 1;
}

void CD3DObj::flip()
{
	if (mD3DDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
		if (mD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			reset();
}

int CD3DObj::width() const
{
	return mD3DPresentParams.BackBufferWidth;
}

int CD3DObj::height() const
{
	return mD3DPresentParams.BackBufferHeight;
}
