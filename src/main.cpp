#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../inc/engine.h"

using std::cout;
using std::endl;
using std::ifstream;


#ifndef WIN32
#include <unistd.h>
#include <signal.h>

#ifdef SDL
#include "../inc/sdlgraphicsengine.h"
#else
#include "../inc/textengine.h"
#endif

bool gameRunning;

void signalh(int)
{
	gameRunning = false;
}

int main(int argc, char **argv)
{
	gameRunning = true;
	signal(SIGINT, signalh);
	ifstream map("res/map.txt");
	ifstream weapons("res/weapons.txt");
#ifdef SDL
	char rv;
	ifstream gfxopts("res/sdlgfx.txt");
#endif
	char **results = NULL;

	srand(argc > 1?atoi(argv[1]):3);

	CGameEngine *engine = new CGameEngine(&weapons, &map);

#ifdef SDL
	CSDLGraphicsEngine *gEngine = new CSDLGraphicsEngine(&gfxopts);
#else
	CTextEngine *gEngine = new CTextEngine();
#endif

	engine->setGraphicsEngine((CGraphicsEngine *)gEngine);

	map.close();
	weapons.close();

	int i = argc > 2 ? atoi(argv[2]) : 0;
	int j = 0;

	while (gameRunning & engine->loop())
	{
		engine->draw(1.f, i);
		usleep(20000);
#ifdef SDL
		if (gameRunning)
		{
			rv = gEngine->flip();
			gameRunning = !(rv == 'q');
			i = (i + (rv == 'n')) & 7;
		}
#endif
		if (!(j = (j + 1 & 15)))
		{
			results = engine->getResults(false, results);
#ifndef SDL
			for (int x = 0; results[x]; x++)
				cout << "\033[" << x + 2 << ";55H" << results[x] << endl;
#endif
		}
	}
	delete gEngine;
	results = engine->getResults(false, results);
	if (results)
	{
		for (i = 0; results[i]; i++)
		{
			cout << results[i] << endl;
			delete [] results[i];
		}
	delete [] results;
}
	delete engine;
	return 0;
}


#else


#include "../inc/ddgraphicsengine.h"
#include <richedit.h>
#pragma comment(lib, "winmm")

CDDGraphicsEngine *gfxEngine = NULL;
bool active = true;
HWND hWnd = NULL;
HWND hResults = NULL;
char **results = NULL;

long FAR PASCAL	windowProc(HWND	aHWnd, UINT	aMsg, WPARAM aWParam, LPARAM aLParam)
{
	switch(aMsg)
	{
	case WM_PAINT:
		if (aHWnd == hResults)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hResults, &ps);
			RECT rect;
			GetClientRect(hResults, &rect);
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			for (int i = 0; results && results[i]; i++)
				TextOut(hdc, 5, i * 20 + 5, results[i], strlen(results[i]));
			EndPaint(hResults, NULL);
		}
		else
			return DefWindowProc(aHWnd,	aMsg, aWParam, aLParam);
		break;
	case WM_DESTROY:
		if (aHWnd == hWnd)
			PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (aHWnd == hWnd)
			if (gfxEngine)
				gfxEngine->resize();
		break;
	case WM_KEYDOWN:
		if (aHWnd == hWnd)
			if (aWParam == VK_ESCAPE)
				active = !active;
	default:
		return DefWindowProc(aHWnd,	aMsg, aWParam, aLParam);
	}

	return NULL;
}

int	PASCAL WinMain(HINSTANCE aHInst, HINSTANCE aHInstPrev, LPSTR aCmdLine, int aCmdShow)
{
	const char	KClassName[]		= "DrunkenBot";
	const char	KResultClassName[]	= "DrunkenBotResults";
	const char	KWindowName[]		= "Drunken Bot III";
	const char	KResultWindowName[]	= "Results";
	HMODULE		mHRichEdit			= LoadLibrary("riched32.dll");
	char *windowName = new char[64];
	int frameCounter = 0;
	float fps;
	DWORD startTime = timeGetTime();

	WNDCLASS	wc;
	WNDCLASS	wc2;
	MSG			msg;
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

		wc2.style = CS_HREDRAW |	CS_VREDRAW;
		wc2.lpfnWndProc = windowProc;
		wc2.cbClsExtra =	0;
		wc2.cbWndExtra =	0;
		wc2.hInstance = aHInst;
		wc2.hIcon = NULL;
		wc2.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc2.lpszMenuName	= NULL;
		wc2.lpszClassName = KResultClassName;
		if (!RegisterClass(&wc2))
			return 0;
	}

	// Create window
	hWnd = CreateWindow(KClassName,	KWindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 800, NULL, NULL, aHInst, NULL);
	if (hWnd)
	{
		ShowWindow(hWnd, aCmdShow);
		UpdateWindow(hWnd);
		hResults = CreateWindow(KResultClassName, KResultWindowName, WS_OVERLAPPED|WS_VISIBLE, 0, 0, 320, 200, NULL, NULL, NULL, NULL);
		if (hResults)
			ShowWindow(hResults, SW_SHOW);
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
				gameEngine->draw(1.f, -1);
				gfxEngine->flip();
				if (++frameCounter >= 100)
				{
					fps = 100000.f / float(timeGetTime() - startTime);
					frameCounter = 0;
					results = gameEngine->getResults(false, results);
					InvalidateRect(hResults, NULL, false);
					UpdateWindow(hResults);
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
