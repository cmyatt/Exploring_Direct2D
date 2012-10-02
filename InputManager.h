#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAHER_H

#include "util.h"
#include "Moveable.h"
#include <map>

class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

private:
	//std::map<VK_CODE, Moveable *> listeners;
};

#endif
