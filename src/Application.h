#pragma once
#include <iostream>

#include "Window.h"
#include "World.h"
#include "Renderer.h"
#include "Physics.h"

class Application {
public:
	Application();
	~Application();
	void init();

	void run();

	inline static Application& getInstance() { return *s_instance; }
	inline Window* getWindow() const { return window; }
	inline World* getWorld() const { return world; }

private:
	Window* window;
	World* world;
	Renderer* renderer;
	Physics* physics;

private:
	static Application* s_instance;
};