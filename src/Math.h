#pragma once
#include <math.h>
#include <iostream>

class Vector3 {
public:
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3() : x(0.0), y(0.0), z(0.0) {}

	inline float length() {
		return std::sqrt(x * x + y * y + z * z);
	}

public:
	float x, y, z;

public:
	Vector3 operator + (Vector3 const &vec3) {
		return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
	}

	Vector3 operator - (Vector3 const &vec3) {
		return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
	}

	Vector3 operator * (Vector3 const &vec3) {
		return Vector3(x * vec3.x, y * vec3.y, z * vec3.z);
	}

	Vector3 operator / (Vector3 const &vec3) {
		return Vector3(x / vec3.x, y / vec3.y, z / vec3.z);
	}

	Vector3 operator * (float const &scalar) {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3 operator / (float const &scalar) {
		return Vector3(x / scalar, y / scalar, z / scalar);
	}
	
	friend std::ostream& operator<<(std::ostream& os, Vector3& v) {
		return os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
	}
};


class Vector2 {
public:
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2() : x(0.0), y(0.0) {}

public:
	float x, y;

public:
	Vector2 operator + (Vector2 const &vec2) {
		return Vector2(x + vec2.x, y + vec2.y);
	}

	Vector2 operator - (Vector2 const &vec2) {
		return Vector2(x - vec2.x, y - vec2.y);
	}

	Vector2 operator * (Vector2 const &vec2) {
		return Vector2(x * vec2.x, y * vec2.y);
	}

	Vector2 operator / (Vector2 const &vec2) {
		return Vector2(x / vec2.x, y / vec2.y);
	}
};

template<typename T>
int sign(T val) {
	return (T(0) < val) - (T(0) > val);
}

class Vector4i {
public:
	Vector4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	Vector4i() : x(0), y(0), z(0), w(0) {}

	inline float length() {
		return std::sqrt(x * x + y * y + z * z);
	}

public:
	int x, y, z, w;

public:
	Vector4i operator + (Vector4i const &vec4i) {
		return Vector4i(x + vec4i.x, y + vec4i.y, z + vec4i.z, vec4i.w + w);
	}

	Vector4i operator - (Vector4i const &vec4i) {
		return Vector4i(x - vec4i.x, y - vec4i.y, z - vec4i.z, vec4i.w - w);
	}

	Vector4i operator * (Vector4i const &vec4i) {
		return Vector4i(x * vec4i.x, y * vec4i.y, z * vec4i.z, vec4i.w * w);
	}

	Vector4i operator / (Vector4i const &vec4i) {
		return Vector4i(x / vec4i.x, y / vec4i.y, z / vec4i.z, vec4i.w / w);
	}

	Vector4i operator * (float const &scalar) {
		return Vector4i(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	Vector4i operator / (float const &scalar) {
		return Vector4i(x / scalar, y / scalar, z / scalar, w / scalar);
	}
	
	friend std::ostream& operator<<(std::ostream& os, Vector4i& v) {
		return os << "{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }";
	}
};