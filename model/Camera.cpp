#include "headers/Camera.h"

Camera* Camera::instance = 0;

bool Camera::CheckCollision(Object* other) const // AABB - AABB collision
{
    glm::vec3 pos = glm::vec3(this->Position.x - this->Size.x / 2,
                              this->Position.y - this->Size.y / 2,
                              this->Position.z - this->Size.z / 2);

    glm::vec3 thatPos = glm::vec3(other->translateVec.x - other->scaleVec.x / 2,
                                  other->translateVec.y - other->scaleVec.y / 2,
                                  other->translateVec.z - other->scaleVec.z / 2);

    // collision x-axis?
    bool collisionX = pos.x + this->Size.x >= thatPos.x &&
        thatPos.x + other->scaleVec.x >= pos.x;
    // collision y-axis?
    bool collisionY = pos.y + this->Size.y >= thatPos.y &&
        thatPos.y + other->scaleVec.y >= pos.y;
    // collision z-axis?
    bool collisionZ = pos.z + this->Size.z >= thatPos.z &&
        thatPos.z + other->scaleVec.z >= pos.z;

    // collision only if on both axes
    return collisionX && collisionY && collisionZ;
}