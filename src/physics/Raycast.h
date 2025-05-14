#pragma once
#include "../misc/Math.h"

class Raycast {
public:
	Raycast(Vector3 startPos, Vector3 direction, float length);

public:
	bool hit;
	Vector3 hitPosition;
	float rayLength;
	Vector3 hitNormal;

private:
	bool isSmallest(float val, Vector3& otherValues);
};
