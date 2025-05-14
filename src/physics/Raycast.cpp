#include "Raycast.h"
#include "../Application.h"
#include "../world/World.h"

Raycast::Raycast(Vector3 startPos, Vector3 direction, float length) {
	World* world = Application::getInstance().getWorld();
	rayLength = 0.0;

	direction = direction / direction.length();

	hit = false;

	Vector3 rayPos = startPos;
	for(int i = 0; i < 100 && rayLength < length; ++i) {
		
		if(world->isSolid(std::floor(rayPos.x), std::floor(rayPos.y), std::floor(rayPos.z))) {
			hit = true;
			hitPosition = rayPos;

			Vector3 blockPos(rayPos.x - std::floor(rayPos.x), rayPos.y - std::floor(rayPos.y), rayPos.z - std::floor(rayPos.z));
			Vector3 invBlockPos = Vector3(1.0, 1.0, 1.0) - blockPos;
			if(isSmallest(blockPos.x, blockPos) && isSmallest(blockPos.x, invBlockPos)) {
				hitNormal = Vector3(-1.0, 0.0, 0.0);
			}
			else if(isSmallest(blockPos.y, blockPos) && isSmallest(blockPos.y, invBlockPos)) {
				hitNormal = Vector3(0.0, -1.0, 0.0);
			}
			else if(isSmallest(blockPos.z, blockPos) && isSmallest(blockPos.z, invBlockPos)) {
				hitNormal = Vector3(0.0, 0.0, -1.0);
			}
			else if(isSmallest(invBlockPos.x, blockPos) && isSmallest(invBlockPos.x, invBlockPos)) {
				hitNormal = Vector3(1.0, 0.0, 0.0);
			}
			else if(isSmallest(invBlockPos.y, blockPos) && isSmallest(invBlockPos.y, invBlockPos)) {
				hitNormal = Vector3(0.0, 1.0, 0.0);
			}
			else {
				hitNormal = Vector3(0.0, 0.0, 1.0);
			}

			return;
		}

		Vector3 localPos = rayPos;
		localPos.x = localPos.x - std::floor(localPos.x);
		localPos.y = localPos.y - std::floor(localPos.y);
		localPos.z = localPos.z - std::floor(localPos.z);

		// pos + scalar * dir = (Either 1.0 or 0.0 depending on direction)
		float scalarX = ((direction.x > 0.0 ? 1.0 : 0.0) - localPos.x) / direction.x; 
		float scalarY = ((direction.y > 0.0 ? 1.0 : 0.0) - localPos.y) / direction.y;
		float scalarZ = ((direction.z > 0.0 ? 1.0 : 0.0) - localPos.z) / direction.z;
		if(scalarX < 0.0) scalarX = 2.0;
		if(scalarY < 0.0) scalarY = 2.0;
		if(scalarZ < 0.0) scalarZ = 2.0;

		float smallestScalar = std::min(scalarX, std::min(scalarY, scalarZ)) + 0.0001;

		if(!(smallestScalar > 0.0)) { // Break if something has gone wrong
			break;
		}

		rayLength += smallestScalar;
		if(rayLength > length) rayLength = length;
		rayPos = startPos + direction * rayLength;
	}

	hitPosition = rayPos;
	rayLength = length;
}

bool Raycast::isSmallest(float value, Vector3& otherValues) {
	return (value <= otherValues.x && value <= otherValues.y && value <= otherValues.z);
}
