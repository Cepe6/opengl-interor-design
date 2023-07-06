#ifndef OBJECT_H
#define OBJECT_H

#include "Shader.h"
#include "stb_image.h"

class Object {
private:
	unsigned int VBO, VAO, EBO;
	unsigned int texture;
	Shader* shader;

	const char* texture_name;

	float rotate_angle;

	void load_texture();
	void light();

public:
	glm::vec3 scale_vec;
	glm::vec3 rotate_vec;
	glm::vec3 translate_vec;
	std::string name;

	Object(std::string name,
           glm::vec3 scale_vec,
           glm::vec3 rotate_vec,
           float rotate_angle,
           glm::vec3 translate_vec,
           const char* texture_name);

	void draw();
	void free();
};
#endif