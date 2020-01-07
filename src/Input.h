#pragma once
#include "Keycodes.h"
#include "Math.h"

class Input {
public:
	static bool keyDown(int keycode);
	static bool mouseKeyDown(int keycode);
	static Vector2 mousePos();
	static Vector2 deltaMouse();
};