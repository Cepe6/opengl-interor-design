#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include "headers/Light.h"
#include "headers/PointLightManager.h"
#include "headers/ShaderManager.h"
#include "headers/Camera.h"

float lamp_vertices[] = {
   -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

unsigned int lamp_indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

Light::Light(std::string name,
             glm::vec3 scale_vec,
             glm::vec3 rotate_vec,
             glm::vec3 default_ambient,
             float rotate_angle,
             glm::vec3 translate_vec) {
    this->scale_vec = scale_vec;
    this->rotate_vec = rotate_vec;
    this->rotate_angle = rotate_angle;
    this->translate_vec = translate_vec;
    this->default_ambient = default_ambient * 0.5f;
    this->name = std::move(name);
    this->shader = ShaderManager::get_shader_by_name("light");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lamp_vertices), lamp_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lamp_indices), lamp_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    shader->use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    PointLightManager::add_point_light(new PointLight({this->name,
                                                       true,
                                                       translate_vec,
                                                       this->default_ambient,
                                                       this->default_ambient * 2.0f,
                                                       glm::vec3(1.0f),
                                                       1.0f,
                                                       0.35,
                                                       0.44}));

    shader->setMat4("projection", projection);
}

void Light::draw() {
    shader->use();
    shader->setVec3("objectColor", 1.0f, 0.5f, 1.0f);
    shader->setVec3("lightColor",  1.0f, 0.5f, 1.0f);

    // make sure to initialize matrix to identity matrix first
    glm::mat4 model = glm::mat4(1.0f);

    // Handle transformations
    model = glm::translate(model, translate_vec);
    model = glm::rotate(model, glm::radians(rotate_angle), rotate_vec);
    model = glm::scale(model, scale_vec);

    // retrieve the matrix uniform locations
    // pass them to the shaders
    shader->setMat4("view", Camera::instance->get_view_matrix());
    shader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}