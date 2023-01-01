#pragma once

#include <vector>
#include <memory>

class Scene
{
public:
	Scene();
	~Scene();

	const auto& getSpheres() const { return spheres; }

private:
	std::vector<std::shared_ptr<class Sphere>> spheres;
};