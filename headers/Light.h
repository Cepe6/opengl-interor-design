#ifndef LIGHT_H
#define LIGHT_H

#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"

class Light {
private:
	unsigned int VBO{}, VAO{}, EBO{};
	unsigned int texture{};
	Shader* shader;

	const char* texture_name;

	float rotateAngle;

public:
	glm::vec3 scaleVec;
	glm::vec3 rotateVec;
	glm::vec3 translateVec;
	std::string name;

	Light(std::string name, glm::vec3 scaleVec, glm::vec3 rotateVec, float rotateAngle, glm::vec3 transalteVec, const char* texture_name);

	void draw();
};
#endif