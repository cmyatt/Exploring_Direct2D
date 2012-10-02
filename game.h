#pragma once
#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <WTypes.h>
#include <string>
#include <d2d1.h>
#include <wincodec.h>
#include <wincodecsdk.h>

// Program settings
const std::string TITLE = "Game";
const int SCREENW = 1024;
const int SCREENH = 768;
extern bool game_over;

// Helper functions
HRESULT load_bitmap_from_file(
    std::string filename,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap **bitmap);

void draw_text(const std::string& text, float x, float y);

// Game entry point functions
bool game_init(HWND window);
void game_run(HWND window);
void game_end(HWND window);

#endif