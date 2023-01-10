#pragma once

#include "Ray.h"
#include "Shape.h"

#include <glm/glm.hpp>

class Sphere : public Shape
{
public:
	Sphere()
	: center(0.0f), radius(0.5f), materialIndex{0}
	{}

	Sphere(const glm::vec3& inCenter, float inRadius, int32_t inMaterialIndex)
	: center(inCenter), radius(inRadius), materialIndex(inMaterialIndex)
	{}

	bool intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection) override;

public:
	glm::vec3 center{ 0.0f };
	float radius{ 0.5f };
	int32_t materialIndex = 0;
};