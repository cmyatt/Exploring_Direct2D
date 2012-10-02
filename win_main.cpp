#include "game.h"

bool game_over = false;


LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		game_over = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize		 = sizeof(wc);
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WinProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = NULL;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = TITLE.c_str();
	wc.hIconSm		 = NULL;
	RegisterClassEx(&wc);

	HWND window = CreateWindow(
		TITLE.c_str(),				// window class
		TITLE.c_str(),				// title bar
		WS_OVERLAPPEDWINDOW,			// window style
		CW_USEDEFAULT, CW_USEDEFAULT,		// position
		640, 480,				// dimensions
		NULL,					// parent window
		NULL,					// menu
		hInstance,				// application instance
		NULL);					// window parameters

	if (window == 0) return 0;

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	if (!game_init(window)) return 0;

	MSG msg;
	while (!game_over)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		game_run(window);
	}

	game_end(window);
	return msg.wParam;
}