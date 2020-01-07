#pragma once

class World;

class Physics {
public:
	Physics();
	~Physics();

	void update(float deltaTime, World* world);

private:
	float gravity = -0.982f * 0.5;
};