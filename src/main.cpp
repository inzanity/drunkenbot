#include <iostream>
#include <fstream>
#include "../inc/engine.h"

using std::cout;
using std::endl;
using std::ifstream;


#ifndef WIN32

#include "../inc/textengine.h"


int main()
{
	ifstream map("res/tmap.txt");
	ifstream weapons("res/weapons.txt");

	CGameEngine *engine = new CGameEngine(&weapons, &map);

	CGraphicsEngine *gEngine = new CTextEngine();

	engine->setGraphicsEngine(gEngine);

	map.close();
	weapons.close();

	while (engine->loop())
		engine->draw(1.f, -1);
	char **results = engine->getResults(false);
	for (int i = 0; results[i]; i++)
		cout << results[i] << endl;
	return 1;
}


#else


#include "../inc/ddgraphicsengine.h"
#pragma comment(lib, "winmm")

CDDGraphicsEngine *gfxEngine = NULL;
bool active = true;

long FAR PASCAL	windowProc(HWND	aHWnd, UINT	aMsg, WPARAM aWParam, LPARAM aLParam)
{
	switch(aMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (gfxEngine)
			gfxEngine->resize();
		break;
	case WM_KEYDOWN:
		if (aWParam == VK_ESCAPE)
			active = !active;
	default:
		return DefWindowProc(aHWnd,	aMsg, aWParam, aLParam);
	}

	return NULL;
}

int	PASCAL WinMain(HINSTANCE aHInst, HINSTANCE aHInstPrev, LPSTR aCmdLine, int aCmdShow)
{
	const char	KClassName[]	= "DrunkenBot";
	const char	KWindowName[]	= "Drunken Bot III";
	char *windowName = new char[64];
	int frameCounter = 0;
	float fps;
	DWORD startTime = timeGetTime();

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
	hWnd = CreateWindow(KClassName,	KWindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 800, NULL, NULL, aHInst, NULL);
	if (hWnd)
	{
		ShowWindow(hWnd, aCmdShow);
		UpdateWindow(hWnd);
	}
	else
		return false;

	gfxEngine = new CDDGraphicsEngine(hWnd, NULL);

	ifstream map("res/map.txt");
	ifstream weapons("res/weapons.txt");
	CGameEngine *gameEngine = new CGameEngine(&weapons, &map);
	map.close();
	weapons.close();
	gameEngine->setGraphicsEngine(gfxEngine);

	// Enter the message loop
	ZeroMemory(&msg, sizeof(msg));
	while(msg.message != WM_QUIT)
	{
		if (active) // Handle messages, and render the rest of the time
		{
			if (!(gotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)))
			{
				if(!gameEngine->loop())
					break;
				gameEngine->draw(1.f, 0);
				gfxEngine->flip();
				if (++frameCounter >= 100)
				{
					frameCounter = 0;
					fps = 100000.f / float(timeGetTime() - startTime);
					sprintf(windowName, "%s, fps: %f", KWindowName, fps);
					SetWindowText(hWnd, windowName);
					startTime = timeGetTime();
				}
			}
		}
		else // Wait for messages
			gotMsg = GetMessage(&msg, NULL, 0U, 0U);

		if (gotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

    delete gameEngine;
	delete gfxEngine;
	gfxEngine = NULL;
    
	UnregisterClass(KClassName,	aHInst);
	return msg.wParam;
}


#endif // WIN32
