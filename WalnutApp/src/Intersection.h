#pragma once

#include <glm/glm.hpp>

#include "Material.h"

struct Intersection
{
	float hitDistance;
	glm::vec3 position;
	glm::vec3 normal;
	int32_t materialIndex;
	uint32_t objectIndex;
};