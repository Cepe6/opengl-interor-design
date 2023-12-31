#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <iostream>
#include <vector>

#include "Shader.h"

class ShaderManager {
private:
	static std::vector<Shader*> shaders;
public:
	static void add_shader(Shader* shader) {
		shaders.push_back(shader);
	}

	static Shader* get_shader_by_name(std::string name) {
		for (Shader* shader : shaders) {
			if (shader->name == name) {
				return shader;
			}
		}
		return nullptr;
	}
};
#endif