#ifndef POINT_LIGHT_MANAGER_H
#define POINT_LIGHT_MANAGER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

struct PointLight {
	bool on;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

class PointLightManager {
private:
	static std::vector<PointLight*> pointLights;
public:
	static void addPointLight(PointLight* pointLight) {
		pointLights.push_back(pointLight);
	}

	static std::vector<PointLight*> getPointLights() {
		return pointLights;
	}
};

#endif