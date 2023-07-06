#ifndef LIGHT_H
#define LIGHT_H

#include "stb_image.h"

#include "Shader.h"

class Light {
private:
	unsigned int VBO{}, VAO{}, EBO{};
	unsigned int texture{};
	Shader* shader;

	const char* texture_name;

	float rotate_angle;

public:
	glm::vec3 scale_vec;
	glm::vec3 rotate_vec;
	glm::vec3 translate_vec;
	std::string name;

	Light(std::string name,
          glm::vec3 scale_vec,
          glm::vec3 rotate_vec,
          float rotate_angle,
          glm::vec3 translate_vec, 
          const char* texture_name);

	void draw();
};
#endif