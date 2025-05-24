#include "Physics.h"
#include "../world/World.h"
#include "Raycast.h"

Physics::Physics() {

}

Physics::~Physics() {

}

void Physics::update(float deltaTime, World* world) {

    for(auto& go : world->gameObjects) {
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

        for(Vector3& vertexPos : collisionVerticies) {
            Vector3 currentPos = go->getPosition() + vertexPos;

            Vector3 blockPos(std::floor(currentPos.x), std::floor(currentPos.y), std::floor(currentPos.z));
            Vector3 local = currentPos - blockPos;

            Vector3 xLocal(local.x, 0.5, 0.5), yLocal(0.5, local.y, 0.5), zLocal(0.5, 0.5, local.z);

            Raycast xRay(blockPos + xLocal, Vector3(rayDirSign.x, 0.0, 0.0), rayLength.x);
            Raycast yRay(blockPos + yLocal, Vector3(0.0, rayDirSign.y, 0.0), rayLength.y);
            Raycast zRay(blockPos + zLocal, Vector3(0.0, 0.0, rayDirSign.z), rayLength.z);


            xRay.rayLength = std::max(xRay.rayLength - 0.0001, 0.0);
            yRay.rayLength = std::max(yRay.rayLength - 0.0001, 0.0);
            zRay.rayLength = std::max(zRay.rayLength - 0.0001, 0.0);

            float xFinalRayLength = xRay.rayLength;
            float yFinalRayLength = yRay.rayLength;
            float zFinalRayLength = zRay.rayLength;

            if(xRay.hit) {
                go->localVelocity.x = 0.0;

                float xRayLength = std::round(local.x + xRay.rayLength * rayDirSign.x);
                xFinalRayLength = std::max(std::abs(xRayLength - local.x) - 0.0001, 0.0);
            }

            if(yRay.hit) {
                go->localVelocity.y = 0.0;
                if(vertexPos.y < 0.0) go->isGrounded = true;

                float yRayLength = std::round(local.y + yRay.rayLength * rayDirSign.y);
                yFinalRayLength = std::max(std::abs(yRayLength - local.y) - 0.0001, 0.0);
            }

            if(zRay.hit) {
                go->localVelocity.z = 0.0;

                float zRayLength = std::round(local.z + zRay.rayLength * rayDirSign.z);
                zFinalRayLength = std::max(std::abs(zRayLength - local.z) - 0.0001, 0.0);
            }

            Vector3 newMovement = movement;

            if(xFinalRayLength < newMovement.x) newMovement.x = xFinalRayLength;
            if(yFinalRayLength < newMovement.y) newMovement.y = yFinalRayLength;
            if(zFinalRayLength < newMovement.z) newMovement.z = zFinalRayLength;

            movement = newMovement;
        }

        Vector3 oldPos = go->localPosition;
        Vector3 newPos = go->localPosition + movement * rayDirSign;

        for(Vector3& vertexPos : collisionVerticies) { // Fix diagonals
            Vector3 oldVertPos = oldPos + vertexPos;
            Vector3 newVertPos = newPos + vertexPos;

            Vector3 deltaPos = newVertPos - oldVertPos;
            float movementRayLength = deltaPos.length();

            Raycast ray(oldVertPos, deltaPos, movementRayLength);
            // ray.rayLength = std::max(ray.rayLength - 0.0001, 0.0);

            if(ray.hit) {
                Vector3 oldBlockPos(std::floor(oldVertPos.x), std::floor(oldVertPos.y), std::floor(oldVertPos.z));
                Vector3 newBlockPos(std::floor(newVertPos.x), std::floor(newVertPos.y), std::floor(newVertPos.z));

                if(oldBlockPos.x != newBlockPos.x) {
                    movement.x = 0.0;
                    go->localVelocity.x = 0.0;
                }
                // if(oldBlockPos.y != newBlockPos.y) {
                //     movement.y = 0.0;
                //     go->localVelocity.y = 0.0;
                // }
                if(oldBlockPos.z != newBlockPos.z) {
                    movement.z = 0.0;
                    go->localVelocity.z = 0.0;
                }

            }
        }

        go->localPosition = go->localPosition + movement * rayDirSign;

    }

}
