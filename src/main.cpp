#include <iostream>
#include <fstream>
#include "../inc/engine.h"

using std::cout;
using std::endl;
using std::ifstream;


#ifndef WIN32


int main()
{
	ifstream map("res/map.txt");
	ifstream weapons("res/weapons.txt");

	CGameEngine *engine = new CGameEngine(&weapons, &map);

	map.close();
	weapons.close();

	while (engine->loop());
	char **results = engine->getResults(false);
	for (int i = 0; results[i]; i++)
		cout << results[i] << endl;
	return 1;
}


#else


#include "../inc/ddgraphicsengine.h"

CDDGraphicsEngine *gfxEngine = NULL;
bool active = true;

long FAR PASCAL	windowProc(HWND	aHWnd, UINT	aMsg, WPARAM aWParam, LPARAM aLParam)
{
	switch(aMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
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
	int width = 800, height = 600;

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
	hWnd = CreateWindow(KClassName,	KWindowName, WS_OVERLAPPEDWINDOW ^ WS_SIZEBOX ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, aHInst, NULL);
	if (hWnd)
	{
		ShowWindow(hWnd, aCmdShow);
		UpdateWindow(hWnd);
	}
	else
		return false;

	gfxEngine = new CDDGraphicsEngine(hWnd, width, height, NULL);

	ifstream map("res/map.txt");
	ifstream weapons("res/weapons.txt");
	CGameEngine *gameEngine = new CGameEngine(&weapons, &map);
	map.close();
	weapons.close();

	// Enter the message loop
	ZeroMemory(&msg, sizeof(msg));
	while(msg.message != WM_QUIT)
	{
		if (active) // Handle messages, and render the rest of the time
		{
			if (!(gotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)))
				if(!gameEngine->loop())
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

    delete gameEngine;
	delete gfxEngine;
	gfxEngine = NULL;
    
	UnregisterClass(KClassName,	aHInst);
	return msg.wParam;
}


#endif // WIN32