#pragma once
#include "Math.h"

class GameObject {
public:
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;

	void move(Vector3 movement);

	inline Vector3 getPosition() { 
		return localPosition;
	}
	inline Vector3 getRotation() { 
		return localRotation; 
	}
	inline Vector3 getScale() { 
		return localScale; 
	}
	inline Vector3 getVelocity() { 
		return localVelocity;
	}

public:
	Vector3 localPosition, localRotation, localScale, localVelocity;
	Vector3 colliderSize;
	bool isGrounded = false;
};
