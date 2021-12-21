#pragma once
#include <iostream>
#include <memory>

class Window;
class World;
class Renderer;
class Physics;

class Application {
public:
	Application();
	~Application();
	void init();

	void run();

	inline static Application& getInstance() { return *s_instance; }
	inline Window* getWindow() const { return window.get(); }
	inline World* getWorld() const { return world.get(); }

private:
    std::unique_ptr<Window> window;
	std::unique_ptr<World> world;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Physics> physics;

private:
	static Application* s_instance;
};
