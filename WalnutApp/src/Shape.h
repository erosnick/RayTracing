#pragma once

#include "Intersection.h"

class Shape
{
public:
	virtual bool intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection) = 0;
};