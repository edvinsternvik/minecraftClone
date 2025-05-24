#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void GameObject::move(Vector3 movement) {
    Vector3 rotation = getRotation();

    glm::mat4 movementMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    movementMatrix = glm::rotate(movementMatrix, glm::radians(-rotation.y), glm::vec3(0.0, 1.0, 0.0));
    movementMatrix = glm::rotate(movementMatrix, glm::radians(-rotation.z), glm::vec3(0.0, 0.0, 1.0));

    glm::vec3 pos = glm::mat3(movementMatrix) * glm::vec3(movement.x, movement.y, movement.z);
    localPosition.x += pos.x;
    localPosition.y += pos.y;
    localPosition.z += pos.z;
}
