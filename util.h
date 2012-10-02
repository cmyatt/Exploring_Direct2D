#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <wchar.h>

// Macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? true : false)


template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

std::wstring convertToWideString(const std::string& s);

#endif