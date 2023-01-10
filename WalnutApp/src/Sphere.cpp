#include "Sphere.h"

bool Sphere::intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection)
{
	// (bx^2 + by^2)t^2 + 2(axbx + ayby)t + (ax^2 + ay^2 - r^2) = 0
	// a = ray origin
	// b = ray direction
	// r = radius
	// t = hit distance
	auto oc = ray.origin - center;
	auto a = glm::dot(ray.direction, ray.direction);
	auto b = 2.0f * glm::dot(oc, ray.direction);
	auto c = glm::dot(oc, oc) - radius * radius;

	// Quadratic formula discriminant
	// b^2 - 4ac;
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
	{
		intersection.hitDistance = -1.0f;

		return false;
	}

	float discriminantSqrt = std::sqrtf(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-b - discriminantSqrt) / (2.0f * a);
	if (root < tMin || tMax < root)
	{
		root = (-b + discriminantSqrt) / (2.0f * a);
		if (root < tMin || tMax < root)
		{
			intersection.hitDistance = -1.0f;
			return false;
		}
	}

	// (-b +- sqrt(discriminant)) / 2a
	float t0 = (-b - discriminantSqrt) / (2.0f * a);
	float t1 = (-b + discriminantSqrt) / (2.0f * a);

	auto hitPosition0 = ray.origin + ray.direction * t0;
	auto hitPosition1 = ray.origin + ray.direction * t1;

	auto normal = glm::normalize(hitPosition0 - center);

	auto color = normal * 0.5f + 0.5f;

	intersection.hitDistance = root;
	intersection.normal = normal;
	intersection.position = ray.origin + ray.direction * intersection.hitDistance;
	intersection.materialIndex = materialIndex;

	return true;
}
