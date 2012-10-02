#include "game.h"

bool game_over = false;

/**
 ** Window event callback function
 **/
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

/**
 ** Entry point for windows program
 **/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	// Set the new window's properties
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

	// Create window and refresh it
	HWND window = CreateWindow(
		TITLE.c_str(),					// window class
		TITLE.c_str(),					// title bar
		WS_OVERLAPPEDWINDOW,			// window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// position
		640, 480,						// dimensions
		NULL,							// parent window
		NULL,							// menu
		hInstance,						// application instance
		NULL);							// window parameters

	if (window == 0) return 0;

	// Display window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	// Initialize game
	if (!game_init(window)) return 0;

	// Main message loop
	MSG msg;
	while (!game_over)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Decode and pass messages to WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Process game loop
		game_run(window);
	}

	// Release resources and end program
	game_end(window);
	return msg.wParam;
}