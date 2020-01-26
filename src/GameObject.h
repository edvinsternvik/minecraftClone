#pragma once
#include "Math.h"
#include "vector"

class GameObject {
public:
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;

	void move(Vector3 movement);

	void setParent(GameObject* parent);

	inline Vector3 getPosition() { 
		if(m_parent != nullptr) return localPosition + m_parent->getPosition();
		return localPosition;
	}
	inline Vector3 getRotation() { 
		if(m_parent != nullptr) return localRotation + m_parent->getRotation();
		return localRotation; 
	}
	inline Vector3 getScale() { 
		if(m_parent != nullptr) return localScale + m_parent->getScale();
		return localScale; 
	}
	inline Vector3 getVelocity() { 
		if(m_parent != nullptr) return localVelocity + m_parent->getVelocity();
		return localVelocity;
	}

public:
	Vector3 localPosition, localRotation, localScale, localVelocity;
	Vector3 colliderSize;
	bool isGrounded = false;

private:
	GameObject* m_parent = nullptr;
};