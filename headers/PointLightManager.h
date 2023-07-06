#ifndef POINT_LIGHT_MANAGER_H
#define POINT_LIGHT_MANAGER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

struct PointLight {
    std::string name;
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
	static std::vector<PointLight*> point_lights;
public:
	static void add_point_light(PointLight* point_light) {
		point_lights.push_back(point_light);
	}

	static std::vector<PointLight*> get_point_lights() {
		return point_lights;
	}

    static PointLight* get_point_light_by_name(std::string name_filter) {
        for (PointLight* point_light : point_lights) {
            if (point_light->name == name_filter) {
                return point_light;
            }
        }
        return nullptr;
    }
};

#endif