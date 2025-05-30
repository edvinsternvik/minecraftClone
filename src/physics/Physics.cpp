#include "Physics.h"
#include "../world/World.h"
#include "Boxcast.h"
#include "../Application.h"

const float EPS = 0.0001;

Physics::Physics() {

}

Physics::~Physics() {

}

void Physics::update(float delta_time, World* world) {

    for(auto& go : world->gameObjects) {
        go->localVelocity.y += gravity * delta_time;
        go->localVelocity.x *= std::max(0.0, 1.0 - delta_time);
        go->localVelocity.z *= std::max(0.0, 1.0 - delta_time);
        go->isGrounded = false;

        for(int i = 0; i < 3; ++i) {
            Vector3 move = go->getVelocity() * delta_time;
            float move_len = move.length();
            Vector3 move_sign(sign(move.x), sign(move.y), sign(move.z));
            Vector3 move_dir = move / move_len;

            Boxcast box_cast(
                go->getPosition(),
                go->collider_min,
                go->collider_max,
                move_dir,
                100.0,
                Application::getInstance().getWorld()
            );
            box_cast.cast_length = std::max((float)0.0, box_cast.cast_length - EPS);

            if(box_cast.hit) {
                move_len = std::min(move_len, box_cast.cast_length);
                go->localPosition = go->localPosition + move_dir * move_len;

                if(box_cast.cast_length <= move_len) {
                    Vector3 hit_n = box_cast.hit_normal;
                    hit_n = Vector3(std::abs(hit_n.x), std::abs(hit_n.y), std::abs(hit_n.z));

                    if(box_cast.hit_normal.y > 0.0 && hit_n.y >= hit_n.x && hit_n.y >= hit_n.z) go->isGrounded = true;
                    
                    if(hit_n.x >= hit_n.y && hit_n.x >= hit_n.z) go->localVelocity.x = 0.0;
                    if(hit_n.y >= hit_n.x && hit_n.y >= hit_n.z) go->localVelocity.y = 0.0;
                    if(hit_n.z >= hit_n.x && hit_n.z >= hit_n.y) go->localVelocity.z = 0.0;
                }
                else break;
            }
            else {
                go->localPosition = go->localPosition + move;
                break;
            }
        }
    }

}
