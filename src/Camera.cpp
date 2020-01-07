#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"
#include "Application.h"
#include <iostream>
#include "Raycast.h"

Camera::Camera() {
	float height = 1.8, halfHeight = height / 2.0, halfWidth = 0.5 / 2.0;
	collisionRays.push_back(Vector3(-halfWidth, -height, -halfWidth));
	collisionRays.push_back(Vector3( halfWidth, -height, -halfWidth));
	collisionRays.push_back(Vector3(-halfWidth, -height,  halfWidth));
	collisionRays.push_back(Vector3( halfWidth, -height,  halfWidth));

	collisionRays.push_back(Vector3(-halfWidth, 0.1, -halfWidth));
	collisionRays.push_back(Vector3( halfWidth, 0.1, -halfWidth));
	collisionRays.push_back(Vector3(-halfWidth, 0.1,  halfWidth));
	collisionRays.push_back(Vector3( halfWidth, 0.1,  halfWidth));

	collisionRays.push_back(Vector3(-halfWidth, -halfHeight, -halfWidth));
	collisionRays.push_back(Vector3( halfWidth, -halfHeight, -halfWidth));
	collisionRays.push_back(Vector3(-halfWidth, -halfHeight,  halfWidth));
	collisionRays.push_back(Vector3( halfWidth, -halfHeight,  halfWidth));
}

void Camera::init() {
	localVelocity.z = 0.0;
	mousePos = Input::mousePos();
	localPosition = Vector3(3.0, 76.0, 4.0);
	localRotation = Vector3(0.0, 0.0, 0.0);
}

void Camera::update() {
	Vector2 currentMousePos = Input::mousePos();
	Vector2 deltaMousePos = currentMousePos - mousePos;
	mousePos = currentMousePos;

	localRotation.y += deltaMousePos.x * 0.1;
	localRotation.x += deltaMousePos.y * 0.1;
    
	Vector3 movement;
	float movementSpeed = 0.05;
	if(Input::keyDown(KEY_LEFT_SHIFT)) {
		movementSpeed *= 2;
	}
	if(Input::keyDown(KEY_W)) {
		movement.z -= movementSpeed;
	}
	if(Input::keyDown(KEY_S)) {
		movement.z += movementSpeed;
	}
	if(Input::keyDown(KEY_A)) {
		movement.x -= movementSpeed;
	}
	if(Input::keyDown(KEY_D)) {
		movement.x += movementSpeed;
	}
	if(Input::keyDown(KEY_SPACE) && isGrounded) {
		movement.y = 1.1;
	}
	if(Input::keyDown(KEY_LEFT_CONTROL)) {
		movement.y -= movementSpeed;
	}

	Vector3 rotation = getRotation();
	float dirRad = glm::radians(rotation.y);
	float dirRadRight = glm::radians(rotation.y + 90.0);
	localVelocity.z += std::cos(dirRad) * movement.z;
	localVelocity.x += std::sin(dirRad) * -movement.z;

	localVelocity.z += std::cos(dirRadRight) * -movement.x;
	localVelocity.x += std::sin(dirRadRight) * movement.x;

	localVelocity.y += movement.y;

	Vector3 position = getPosition();
	viewMatrix = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	viewMatrix = glm::rotate(viewMatrix, dirRad, glm::vec3(0.0, 1.0, 0.0));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

	viewMatrix = glm::translate(viewMatrix, - glm::vec3(position.x, position.y, position.z));

	if(Input::mouseKeyDown(MOUSE_KEY_LEFT)) {
		Vector3 direction;
		direction.x = std::cos(glm::radians(rotation.y - 90.0)) * std::cos(glm::radians(rotation.x));
		direction.z = std::sin(glm::radians(rotation.y - 90.0)) * std::cos(glm::radians(rotation.x));
		direction.y = -std::sin(glm::radians(rotation.x));

		Raycast ray(position, direction, 4.0);
		if(ray.hit) {
			Application::getInstance().getWorld()->changeBlock(std::floor(ray.hitPosition.x), std::floor(ray.hitPosition.y), std::floor(ray.hitPosition.z), BlockId::Air);
		}
	}
}