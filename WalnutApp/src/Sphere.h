#pragma once

#include "Ray.h"
#include "Shape.h"
#include "Material.h"

#include <glm/glm.hpp>

class Sphere : public Shape
{
public:
	Sphere()
	: center(0.0f), radius(0.5f), material{}
	{}

	Sphere(const glm::vec3& inCenter, float inRadius, const Material& inMaterial)
	: center(inCenter), radius(inRadius), material(inMaterial)
	{}

	bool intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection) override;

public:
	glm::vec3 center{ 0.0f };
	float radius{ 0.5f };
	Material material{};
};