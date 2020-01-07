#include "Application.h"
#include <chrono>

Application* Application::s_instance = nullptr;

Application::Application() {
    if(s_instance) {
        std::cout << "ERROR APPLICATION ALREADY EXISTS" << std::endl;
    }
    s_instance = this;
}

Application::~Application() {
    delete window;
    delete renderer;
    delete world; // Causes issues
    delete physics;
}

void Application::init() {
    window = new Window();
	renderer = new Renderer();
    world = new World();
    physics = new Physics();

	Camera* camera = world->createGameObject<Camera>();
	renderer->setCamera(camera);
}


void Application::run() {
    world->init();

    double average = 1.0;
    int fps = 0;
    double timer = 0.0;
    float deltaTime = 0.0;
	while (window->isOpen()) {
        // std::cout << "Loop" << std::endl;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        
        world->update();
        // std::cout << "world" << std::endl;
        physics->update(deltaTime, world);
        // std::cout << "physics" << std::endl;
		renderer->render(world);

        window->swapBuffers();
        window->handleEvents();

        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        auto test = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        timer += test.count();

        ++fps;

        if(timer > 100000.0)  {
            timer -= 1000000.0;
            std::cout << fps << std::endl;
            fps = 0;
        }

        deltaTime = test.count() / 100000.0;
    }
}