#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

class Camera : public GameObject {
public:
	Camera();

	virtual void init() override;
	virtual void update() override;

public:
	Vector2 mousePos;
	glm::mat4 viewMatrix;
};