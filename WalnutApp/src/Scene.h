#pragma once

#include <vector>
#include <memory>

#include "Material.h"

class Scene
{
public:
	Scene();
	~Scene();

	const auto& getSpheres() const { return spheres; }
	const auto& getMaterials() const { return materials; }

	auto& getMaterials() { return materials; }
private:
	std::vector<std::shared_ptr<class Sphere>> spheres;
	std::vector<Material> materials;
};