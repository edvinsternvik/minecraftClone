#pragma once
#include "../misc/Math.h"
#include "../world/World.h"

class Boxcast {
public:
    Boxcast(Vector3 start, Vector3 box_min, Vector3 box_max, Vector3 dir, float max_len, const World* world);

public:
    bool hit;
    float cast_length;
    Vector3 hit_normal;
};
