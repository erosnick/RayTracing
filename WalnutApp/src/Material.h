#pragma once

#include <glm/glm.hpp>

struct Material
{
	glm::vec3 albedo{ 1.0f };
	float reflective = 0.0f;
};