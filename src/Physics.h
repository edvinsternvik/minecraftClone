#pragma once

class World;

class Physics {
public:
	Physics();
	~Physics();

	void update(float deltaTime, World* world);

private:
	double gravity = -0.982 * 0.5;
};