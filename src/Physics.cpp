#include "Physics.h"
#include "World.h"
#include "Raycast.h"

Physics::Physics() {

}

Physics::~Physics() {

}

void Physics::update(float deltaTime, World* world) {

	for(GameObject* go : world->gameObjects) {
		float test = std::max(1.0 - deltaTime, 0.0);
		go->localVelocity.x *= test;
		go->localVelocity.z *= test;

		go->localVelocity.y += gravity * deltaTime;

		Vector3 rayDir = go->getVelocity() * deltaTime;
		Vector3 rayDirSign(sign(rayDir.x), sign(rayDir.y), sign(rayDir.z));
		Vector3 rayLength(std::abs(rayDir.x), std::abs(rayDir.y), std::abs(rayDir.z));

		Vector3 movement = rayLength;
		bool hit = false;
		go->isGrounded = false;
		Vector3 position = go->getPosition();

		
		std::vector<Vector3> collisionVerticies;
		Vector3 hCollSize = go->colliderSize * 0.5;
		collisionVerticies.push_back(Vector3(-hCollSize.x, -hCollSize.y, -hCollSize.z));
		collisionVerticies.push_back(Vector3( hCollSize.x, -hCollSize.y, -hCollSize.z));
		collisionVerticies.push_back(Vector3(-hCollSize.x, -hCollSize.y,  hCollSize.z));
		collisionVerticies.push_back(Vector3( hCollSize.x, -hCollSize.y,  hCollSize.z));

		collisionVerticies.push_back(Vector3(-hCollSize.x, hCollSize.y, -hCollSize.z));
		collisionVerticies.push_back(Vector3( hCollSize.x, hCollSize.y, -hCollSize.z));
		collisionVerticies.push_back(Vector3(-hCollSize.x, hCollSize.y,  hCollSize.z));
		collisionVerticies.push_back(Vector3( hCollSize.x, hCollSize.y,  hCollSize.z));

		Vector3 deltaPos = position - Vector3(std::floor(position.x), std::floor(position.y), std::floor(position.z));


		for(Vector3& vertexPos : collisionVerticies) {
			Vector3 currentPos = position + vertexPos;

			Vector3 blockPos(std::floor(currentPos.x), std::floor(currentPos.y), std::floor(currentPos.z));
			Vector3 local = currentPos - blockPos;

			Vector3 xLocal(local.x, 0.5, 0.5), yLocal(0.5, local.y, 0.5), zLocal(0.5, 0.5, local.z);

			Raycast xRay(blockPos + xLocal, Vector3(rayDirSign.x, 0.0, 0.0), rayLength.x);
			Raycast yRay(blockPos + yLocal, Vector3(0.0, rayDirSign.y, 0.0), rayLength.y);
			Raycast zRay(blockPos + zLocal, Vector3(0.0, 0.0, rayDirSign.z), rayLength.z);

			if(xRay.rayLength == 0 && yRay.rayLength == 0 && zRay.rayLength == 0) {
				movement = rayLength * -1.0;
				go->localVelocity.x = 0.0;
				go->localVelocity.z = 0.0;
			}
			else {

				xRay.rayLength = std::max(xRay.rayLength - 0.00011, 0.0);
				yRay.rayLength = std::max(yRay.rayLength - 0.00011, 0.0);
				zRay.rayLength = std::max(zRay.rayLength - 0.00011, 0.0);
				if(xRay.rayLength == 0.0) {
					go->localVelocity.x = 0.0;
				}

				if(yRay.rayLength == 0.0) {
					go->localVelocity.y = 0.0;
					go->isGrounded = true;
				}

				if(zRay.rayLength == 0.0) {
					go->localVelocity.z = 0.0;
				}


				if(xRay.rayLength < movement.x) movement.x = xRay.rayLength;
				if(yRay.rayLength < movement.y) movement.y = yRay.rayLength;
				if(zRay.rayLength < movement.z) movement.z = zRay.rayLength;
			}
		}

		Vector3 newPos = go->localPosition + movement * rayDirSign;

		go->localPosition = newPos;
	}

}