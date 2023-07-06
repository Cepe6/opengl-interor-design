#include "headers/Camera.h"

Camera* Camera::instance = nullptr;

bool Camera::check_collision(Object* other) const // AABB - AABB collision
{
    glm::vec3 pos = glm::vec3(this->position.x - this->size.x / 2,
                              this->position.y - this->size.y / 2,
                              this->position.z - this->size.z / 2);

    glm::vec3 obj_pos = glm::vec3(other->translate_vec.x - other->scale_vec.x / 2,
                                  other->translate_vec.y - other->scale_vec.y / 2,
                                  other->translate_vec.z - other->scale_vec.z / 2);

    // collision x-axis?
    bool x_collision = pos.x + this->size.x >= obj_pos.x &&
                       obj_pos.x + other->scale_vec.x >= pos.x;
    // collision y-axis?
    bool y_collision = pos.y + this->size.y >= obj_pos.y &&
                       obj_pos.y + other->scale_vec.y >= pos.y;
    // collision z-axis?
    bool z_collision = pos.z + this->size.z >= obj_pos.z &&
                       obj_pos.z + other->scale_vec.z >= pos.z;

    // collision only if on both axes
    return x_collision && y_collision && z_collision;
}