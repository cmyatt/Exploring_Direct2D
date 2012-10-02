#include <iostream>
#include <time.h>
#include <d2d1.h>
#include <dwrite.h>
#include "game.h"
#include "Log.h"
#include "Poly2D.h"
#include "util.h"
#include "BoundingBox.h"
#include "AppManager.h"
#include "Sprite.h"
using namespace std;

// Game objects
ID2D1Bitmap *text = NULL;
Vector2D *start_point = NULL;
BoundingBox *box2 = NULL;
BoundingBox *box = NULL;
Sprite *tux = NULL;
shared_ptr<AppManager> manager = nullptr;

/**
 ** Startup and loading code here
 **/
bool game_init(HWND window)
{
	manager = AppManager::getInstance(window);
	if (manager == nullptr)
		return false;

	// Create tux bitmap
	ID2D1Bitmap *tux_img = NULL;
	if (!manager->loadBitmap("tux-large.bmp", 640, 480, &tux_img))
		return false;
	if ((tux = new Sprite(tux_img)) == NULL)
		return false;
	
	// Create text bitmap
	if (!manager->newTextStyle("dark", "Arial", 15.0f, BOLD, D2D1::ColorF::DarkSlateBlue))
		return false;
	manager->textToBitmap("Hello World! Here's some TEXT :)", "dark", &text);
	//if (!manager->textToBitmap("Hello World! Here's some TEXT :)", "dark", &text))
		//return false;

	// Create a pentagon
	if ((box2 = new BoundingBox()) == NULL)
		return false;

	Vector2D v1(0, 0), v2(30, -20), v3(60, 0), v4(45, 30), v5(15, 30);
	box2->addVertex(v1);
	box2->addVertex(v2);
	box2->addVertex(v3);
	box2->addVertex(v4);
	box2->addVertex(v5);
	box2->move(Vector2D(300, 150));

	if (!SUCCEEDED(box2->create(manager, D2D1::ColorF::Red)))
		return false;

	// Create spikey-polygon
	if ((box = new BoundingBox()) == NULL)
		return false;
	Vector2D u1(0, 0), u2(50, -20), u3(60, -10), u4(110, -30), u5(100, -40), u6(200, -20), u7(125, 50), u8(15, 50);
	box->addVertex(u1);
	box->addVertex(u2);
	box->addVertex(u3);
	box->addVertex(u4);
	box->addVertex(u5);
	box->addVertex(u6);
	box->addVertex(u7);
	box->addVertex(u8);

	if (!SUCCEEDED(box->create(manager, D2D1::ColorF::Blue)))
		return false;

	manager->addDrawable(tux);
	manager->addDrawable(box2);
	manager->addDrawable(box);

	return true;
}

/**
 ** Update function called from inside main loop
 **/
void game_run(HWND window)
{
	// Make sure game not over and Direct3D device still valid
	if (game_over)
		return;

	manager->update();

	/*ID2D1HwndRenderTarget *t = manager->getRenderTarget();
	t->BeginDraw();
	t->DrawBitmap(text);
	t->EndDraw();*/

	// Check for escape key (exits program)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(window, WM_DESTROY, NULL, NULL);

	Vector2D move(0, 0);

	if (KEY_DOWN(VK_UP))
		move.add(Vector2D(0, -1));
	else if (KEY_DOWN(VK_DOWN))
		move.add(Vector2D(0, 1));

	if (KEY_DOWN(VK_LEFT))
		move.add(Vector2D(-1, 0));
	else if (KEY_DOWN(VK_RIGHT))
		move.add(Vector2D(1, 0));

	box->move(move);
	if (box->collidesWith(*box2))
		box->move(move.mult(-1));
}

/**
 ** Shutdown code
 **/
void game_end(HWND window)
{
	if (box2) delete box2;
	if (box) delete box;
	if (tux) delete tux;
	SafeRelease(&text);
}