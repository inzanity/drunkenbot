#include <windows.h>
#include "../include/d3dUtil.h"
#include "../include/animationStorage.h"
#include "../include/game.h"

#define		KScreenWidth	640
#define		KScreenHeight	480

const char	KClassName[]	= "Avalance";
const char	KWindowName[]	= "Avalance: Battle over Platonia";

BOOL active	= TRUE;

LRESULT FAR PASCAL	windowProc(HWND	aHWnd, UINT	aMsg, WPARAM aWParam, LPARAM aLParam)
{
	switch(aMsg)
	{
	case WM_ACTIVATEAPP:
		active = (BOOL)aWParam;
		break;

//	case WM_SETCURSOR:
//		SetCursor(NULL);
//		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(aHWnd,	aMsg, aWParam, aLParam);
	}

	return NULL;
}

int	PASCAL WinMain(HINSTANCE aHInst, HINSTANCE aHInstPrev, LPSTR aCmdLine, int aCmdShow)
{
	WNDCLASS	wc;
	MSG			msg;
	HWND		hWnd;
	BOOL		gotMsg;

	if (!aHInstPrev)
	{
		wc.style = CS_HREDRAW |	CS_VREDRAW;
		wc.lpfnWndProc = windowProc;
		wc.cbClsExtra =	0;
		wc.cbWndExtra =	0;
		wc.hInstance = aHInst;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName	= NULL;
		wc.lpszClassName = KClassName;
		if (!RegisterClass(&wc))
			return 0;
	}

	// Create window
	hWnd = CreateWindow(KClassName,	KWindowName, WS_OVERLAPPEDWINDOW ^ WS_SIZEBOX ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, KScreenWidth, KScreenHeight, NULL, NULL, aHInst, NULL);
//	hWnd = CreateWindow(KClassName,	KWindowName, WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, aHInst, NULL);
	if (hWnd)
	{
		ShowWindow(hWnd, aCmdShow);
		UpdateWindow(hWnd);
	}
	else
		return false;

	CD3DObj *d3dObj = new CD3DObj(hWnd);
	if (!d3dObj->initWindowed())
	{
		delete d3dObj;
		return false;
	}
	CAnimationStorage *animationStorage = new CAnimationStorage();
	CGame *game = new CGame();
	if (!game->init())
	{
		delete game;
		delete animationStorage;
		delete d3dObj;
		return false;
	}

	// Enter the message loop
	ZeroMemory(&msg, sizeof(msg));
	while(msg.message != WM_QUIT)
	{
		if (active) // Handle messages, and render the rest of the time
		{
			if (!(gotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)))
				if(!game->loop())
					break;
		}
		else // Wait for messages
			gotMsg = GetMessage(&msg, NULL, 0U, 0U);

		if (gotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	delete game;
	delete animationStorage;
	delete d3dObj;
    
	UnregisterClass(KClassName,	aHInst);
	return (int)msg.wParam;
}
