#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <stdexcept>

class Object;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    static Camera* instance;
    // camera Attributes
    glm::vec3 position;
    glm::vec2 forbidden_directions;
    glm::vec3 size;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    Object* colliding;
    float y;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 1.8f, 3.0f),
           glm::vec3 size = glm::vec3(1.0f, 4.0f, 1.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                   movement_speed(SPEED),
                                                   mouse_sensitivity(SENSITIVITY),
                                                   zoom(ZOOM)
    {
        if (!instance) {
            instance = this;
        }
        else throw std::runtime_error("Instance of camera already exists!");
        this->colliding = nullptr;
        this->position = position;
        this->y = position.y;
        this->world_up = up;
        this->yaw = yaw;
        this->size = size;
        this->pitch = pitch;
        update_camera_vectors();
    }
    // constructor with scalar values
    Camera(float posX,
           float posY,
           float posZ,
           float upX,
           float upY,
           float upZ,
           float yaw,
           float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
                          movement_speed(SPEED),
                          mouse_sensitivity(SENSITIVITY),
                          zoom(ZOOM)
    {

        if (!instance) {
            instance = this;
        }
        else throw std::runtime_error("Instance of camera already exists!");
        this->position = glm::vec3(posX, posY, posZ);
        this->world_up = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 get_view_matrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void on_keyboard_input(Camera_Movement direction, float delta_time, bool increased_movement_speed)
    {
        glm::vec3 old_position = position;
        float velocity = movement_speed * delta_time * (increased_movement_speed ? 2.0f : 1.0f);
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;

        if (colliding) {
            if ((forbidden_directions.x < 0 && old_position.x > position.x)
                || (forbidden_directions.x > 0 && old_position.x < position.x)) {
                position.x = old_position.x;
            }
            if ((forbidden_directions.y < 0 && old_position.z > position.z)
                || (forbidden_directions.y > 0 && old_position.z < position.z)) {
                position.z = old_position.z;
            }

            if (!check_collision(colliding)) {
                colliding = nullptr;
            }
        }
        else {
            float x = 0.0f;
            if (old_position.x < position.x) {
                x = 1.0f;
            }
            else if (old_position.x > position.x) {
                x = -1.0f;
            }
            float z = 0.0f;
            if (old_position.z < position.z) {
                z = 1.0f;
            }
            else if (old_position.z > position.z) {
                z = -1.0f;
            }

            forbidden_directions = glm::vec2(x, z);
        }
        position.y = y;
    }

    void on_mouse_movement(float x_offset, float y_offset, GLboolean constraint_pitch = true)
    {
        x_offset *= mouse_sensitivity;
        y_offset *= mouse_sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constraint_pitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        update_camera_vectors();
    }

    void on_mouse_scroll(float y_offset)
    {
        zoom -= (float)y_offset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    bool check_collision(Object* other) const;

private:
    void update_camera_vectors()
    {
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(new_front);
        this->right = glm::normalize(glm::cross(new_front, world_up));
        this->up = glm::normalize(glm::cross(right, new_front));
    }
};
#endif