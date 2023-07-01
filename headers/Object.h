#ifndef OBJECT_H
#define OBJECT_H

#include "Shader.h"
#include "stb_image.h"

#include <GLFW/glfw3.h>

class Object {
private:
	unsigned int VBO, VAO, EBO;
	unsigned int texture;
	Shader* shader;

	const char* texture_name;

	float rotateAngle;

	void loadTexture();
	void light();

public:
	glm::vec3 scaleVec;
	glm::vec3 rotateVec;
	glm::vec3 translateVec;
	std::string name;

	Object(std::string name, glm::vec3 scaleVec, glm::vec3 rotateVec, float rotateAngle, glm::vec3 transalteVec, const char* texture_name);

	void draw();
	void free();
};
#endif