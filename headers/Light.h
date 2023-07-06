#ifndef LIGHT_H
#define LIGHT_H

#include "stb_image.h"

#include "Shader.h"

class Light {
private:
	unsigned int VBO{}, VAO{}, EBO{};
	unsigned int texture{};
	Shader* shader;
	float rotate_angle;

public:
	glm::vec3 scale_vec;
	glm::vec3 rotate_vec;
	glm::vec3 translate_vec;
    glm::vec3 default_ambient;
	std::string name;

	Light(std::string name,
          glm::vec3 scale_vec,
          glm::vec3 rotate_vec,
          glm::vec3 default_ambient,
          float rotate_angle,
          glm::vec3 translate_vec);

	void draw();
};
#endif