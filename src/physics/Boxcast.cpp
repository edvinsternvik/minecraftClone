#include "Boxcast.h"

const int MAX_ITERATIONS = 100;
const float EPS = 0.0001;

Boxcast::Boxcast(
    Vector3 start,
    Vector3 box_min,
    Vector3 box_max,
    Vector3 dir,
    float max_len,
    const World* world
) {
    hit = false;
    hit_normal = Vector3(0.0, 0.0, 0.0);
    cast_length = 0;

    Vector3 dir_sign(sign(dir.x), sign(dir.y), sign(dir.z));
    if(dir_sign.x == 0.0 && dir_sign.y == 0.0 && dir_sign.z == 0.0) return;

    for(int iteration = 0; iteration < MAX_ITERATIONS && cast_length < max_len; ++iteration) {
        Vector3 pos = start;
        if(dir_sign.x > 0.0) pos.x += box_max.x;
        else if(dir_sign.x < 0.0) pos.x += box_min.x;
        if(dir_sign.y > 0.0) pos.y += box_max.y;
        else if(dir_sign.y < 0.0) pos.y += box_min.y;
        if(dir_sign.z > 0.0) pos.z += box_max.z;
        else if(dir_sign.z < 0.0) pos.z += box_min.z;

        float dx = 2.0;
        if(dir.x > 0.0) dx = std::floor(pos.x + 1.0) - pos.x;
        else if(dir.x < 0.0) dx = pos.x - std::ceil(pos.x - 1.0);

        float dy = 2.0;
        if(dir.y > 0.0) dy = std::floor(pos.y + 1.0) - pos.y;
        else if(dir.y < 0.0) dy = pos.y - std::ceil(pos.y - 1.0);

        float dz = 2.0;
        if(dir.z > 0.0) dz = std::floor(pos.z + 1.0) - pos.z;
        else if(dir.z < 0.0) dz = pos.z - std::ceil(pos.z - 1.0);

        float delta = 0.0;
        if(dx <= dy && dx <= dz) {
            delta = dx;
            hit_normal = Vector3(-dir_sign.x, 0.0, 0.0);
        }
        else if(dy <= dx && dy <= dz) {
            delta = dy;
            hit_normal = Vector3(0.0, -dir_sign.y, 0.0);
        }
        else {
            delta = dz;
            hit_normal = Vector3(0.0, 0.0, -dir_sign.z);
        }

        if(delta == 2.0 || delta <= 0.0) {
            cast_length = 0;
            hit_normal = Vector3(0.0, 0.0, 0.0);
            return;
        }

        start = start + dir_sign * delta;
        cast_length += delta;

        Vector3 c_min = start + dir_sign * EPS + box_min;
        Vector3 c_max = start + dir_sign * EPS + box_max;
        for(int cx = std::floor(c_min.x); cx <= std::floor(c_max.x); ++cx) {
            for(int cy = std::floor(c_min.y); cy <= std::floor(c_max.y); ++cy) {
                for(int cz = std::floor(c_min.z); cz <= std::floor(c_max.z); ++cz) {
                    if(!world->isSolid(cx, cy, cz)) continue;

                    hit = true;
                    cast_length = std::max((float)0.0, cast_length);
                    return;
                }
            }
        }

    }

    cast_length = std::min(cast_length, max_len);
}
