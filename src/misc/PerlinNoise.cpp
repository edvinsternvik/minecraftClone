#include "PerlinNoise.h"
#include <math.h>
#include <cmath>
#include <random>
#include <algorithm>

void PerlinNoise::seed(int seed) {
	std::shuffle(p, p + 512, std::default_random_engine(seed));
}

float PerlinNoise::noise(float x, float y) const {
	int xInt = std::floor(x), yInt = std::floor(y);
	float xFloat = x - xInt, yFloat = y - yInt;
	int xGrid = xInt & 255, yGrid = yInt & 255;

	int point1 = p[p[xGrid    ] + yGrid    ];
	int point2 = p[p[xGrid + 1] + yGrid    ];
	int point3 = p[p[xGrid    ] + yGrid + 1];
	int point4 = p[p[xGrid + 1] + yGrid + 1];


	float grad1 = grad(point1, xFloat, yFloat);
	float grad2 = grad(point2, xFloat - 1, yFloat);
	float grad3 = grad(point3, xFloat, yFloat - 1);
	float grad4 = grad(point4, xFloat - 1, yFloat - 1);

	float xFaded = fade(xFloat), yFaded = fade(yFloat);

	float a = lerp(grad1, grad2, xFaded);
	float b = lerp(grad3, grad4, xFaded);
	float c = lerp(a, b, yFaded);

	return c;
}

float PerlinNoise::grad(int hash, float x, float y) const { // Get appropriate gradient vector and return dot product
	switch(hash & 3) {
	case 0:
		return x + y;
	case 1:
		return -x + y;
	case 2:
		return x - y;
	case 3:
		return -x - y;
	default:
		return x + y;
	}
}

float PerlinNoise::lerp(float a, float b, float t) const {
	return a + (b - a) * t;
}

float PerlinNoise::fade(float t) const {
	return t * t * t * ( t * ( t * 6 - 15) + 10);
}
