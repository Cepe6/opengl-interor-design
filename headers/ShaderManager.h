#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <iostream>
#include <vector>

#include "Shader.h"

class ShaderManager {
private:
	static std::vector<Shader*> shaders;
public:
	static void addShader(Shader* shader) {
		shaders.push_back(shader);
	}

	static Shader* getShaderByName(std::string name) {
		for (Shader* shader : shaders) {
			if (shader->name == name) {
				return shader;
			}
		}
		return NULL;
	}
};
#endif