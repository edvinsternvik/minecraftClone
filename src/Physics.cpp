#include "Physics.h"
#include "World.h"
#include "Raycast.h"

Physics::Physics() {

}

Physics::~Physics() {

}

void Physics::update(float deltaTime, World* world) {

	for(GameObject* go : world->gameObjects) {
		// if(go->isGrounded) {
			float test = std::max(1.0 - deltaTime, 0.0);
			go->localVelocity.x *= test;
			go->localVelocity.z *= test;
		// }
		// go->localVelocity.z = 5.0;
		// if(!go->isGrounded) go->localVelocity.y += gravity * deltaTime;
		// float velDir = go->localVelocity.y > 0.0 ? 1.0 : -1.0;

		// Vector3 position = go->getPosition();
		
		// Vector3 rayDir = go->getVelocity();
		// if(rayDir.x == 0.0 && rayDir.y == 0.0 && rayDir.z == 0.0) continue;
		// rayDir = rayDir / rayDir.length();
		
		// float rayLength = rayDir.length() * deltaTime;
		// float smallestRayLength = rayLength;

		// go->isGrounded = false;
		// for(Vector3& rayPos : go->collisionRays) {
		// 	Raycast ray(position + rayPos - rayDir * 0.1, rayDir, rayLength + 0.1);
			
		// 	if(ray.hit) {
		// 		go->localVelocity.y = 0.0;
		// 		go->isGrounded = true;
		// 	}

		// 	if(ray.rayLength < smallestRayLength) {
		// 		smallestRayLength = ray.rayLength;
		// 	}
		// }

		// go->localPosition = go->localPosition + rayDir * smallestRayLength;

		// go->localVelocity.z = 0.25;
		go->localVelocity.y += gravity * deltaTime;

		Vector3 rayDir = go->getVelocity() * deltaTime;
		Vector3 rayDirSign(sign(rayDir.x), sign(rayDir.y), sign(rayDir.z));
		Vector3 rayLength(std::abs(rayDir.x), std::abs(rayDir.y), std::abs(rayDir.z));
		// Vector3 rayDirNormal;
		// float rayDirLength = rayDir.length();
		// if(rayDirLength) rayDirNormal = rayDir / rayDir.length();

		Vector3 movement = rayLength;
		bool hit = false;
		go->isGrounded = false;
		for(Vector3& rayPos : go->collisionRays) {
			Vector3 rayPosSign(sign(rayPos.x), sign(rayPos.y), sign(rayPos.z));
			float scalar = 0.01;

			Vector3 rayXdir(rayDirSign.x, 0.0, 0.0), rayYdir(0.0, rayDirSign.y, 0.0), rayZdir(0.0, 0.0, rayDirSign.z);
			Raycast rayX(go->getPosition() + rayPos + Vector3(rayPosSign.x, 0, 0) * scalar, rayXdir, rayLength.x + scalar);
			Raycast rayY(go->getPosition() + rayPos + Vector3(0, rayPosSign.y, 0) * scalar, rayYdir, rayLength.y + scalar);
			Raycast rayZ(go->getPosition() + rayPos + Vector3(0, 0, rayPosSign.z) * scalar, rayZdir, rayLength.z + scalar);

			rayX.rayLength = std::max(rayX.rayLength, 0.0f);
			rayY.rayLength = std::max(rayY.rayLength, 0.0f);
			rayZ.rayLength = std::max(rayZ.rayLength, 0.0f);


			if(rayX.hit) {
				if(rayX.rayLength < movement.x) movement.x = rayX.rayLength;
				go->localVelocity.x = 0.0;
				movement.x = 0.0;
			}
			if(rayY.hit) {
				if(rayY.rayLength < movement.y) movement.y = rayX.rayLength;
				if(!go->isGrounded && go->localVelocity.y < 0.0) {
					go->isGrounded = true;
				}
				go->localVelocity.y = 0.0;
				movement.y = 0.0;
				// if(sign(go->localVelocity.y) == rayDirSign.y) {
				// }
			}
			if(rayZ.hit) {
				if(rayZ.rayLength < movement.z) movement.z = rayX.rayLength;
				go->localVelocity.z = 0.0;
				movement.z = 0.0;
			}
		}

		go->localPosition = go->localPosition + movement * rayDirSign;
		// std::cout << go->localPosition << std::endl;
	}

}