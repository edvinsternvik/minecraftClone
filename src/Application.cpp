#include "Application.h"
#include <chrono>

#include "misc/Window.h"
#include "world/World.h"
#include "rendering/Renderer.h"
#include "physics/Physics.h"

Application* Application::s_instance = nullptr;

Application::Application() {
    if(s_instance) {
        std::cout << "ERROR APPLICATION ALREADY EXISTS" << std::endl;
    }
    s_instance = this;
}

Application::~Application() {
}

void Application::init() {
    window = std::make_unique<Window>();
    renderer = std::make_unique<Renderer>();
    world = std::make_unique<World>(123456);
    physics = std::make_unique<Physics>();
    camera = world->createGameObject<Camera>();
}


void Application::run() {
    world->init();

    double average = 1.0;
    int fps = 0;
    double timer = 0.0;
    float deltaTime = 0.0;
    while (window->isOpen()) {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        world->update_chunks(camera->getPosition().x, camera->getPosition().z);
        world->update(deltaTime);
        physics->update(deltaTime, world.get());
        renderer->prepare_chunks(world.get());
        renderer->render(world.get(), camera);

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
